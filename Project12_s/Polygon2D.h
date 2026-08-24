#pragma once

#include "CommandList.h"
#include <vector>

class Polygon2D final{
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuff{};	//頂点バッファー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};			//頂点バッファービュー

	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff{};		//指数バッファー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};				//指数バッファービュー

	UINT indexCount{};	//指数数

	Microsoft::WRL::ComPtr<ID3D12Resource> textureBuffer{};	//テクスチャーバッファー
	D3D12_TEXTURE_COPY_LOCATION textureLocation{};			//テクスチャー用転送定義

	Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer{};  //アップロードバッファー
	D3D12_TEXTURE_COPY_LOCATION uploadLocation{};			//アップロード用転送定義

	std::vector<UINT> heapNum{};	//使用しているヒープの番号


	[[nodiscard]] bool CreateVertex(const DXGIDevice& dxgiDevice) noexcept;



	[[nodiscard]] bool CreateIndex(const DXGIDevice& dxgiDevice) noexcept;



public:
	Polygon2D() = default;
	~Polygon2D() = default;

	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice) noexcept;


	void Draw(const CommandList& list) noexcept;
};