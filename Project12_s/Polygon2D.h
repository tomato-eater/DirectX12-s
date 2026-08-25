#pragma once

#include "Comm_Fence.h"
#include <vector>

//二次元オブジェクト管理クラス
class Polygon2D final{
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer{};	//頂点バッファー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};			//頂点バッファービュー

	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer{};		//指数バッファー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};				//指数バッファービュー

	UINT indexCount{};	//指数数

	Microsoft::WRL::ComPtr<ID3D12Resource> textureBuffer{};	//テクスチャーバッファー

	std::vector<UINT> heapNum{};	//使用しているヒープの番号


	//頂点バッファー等作成
	//DXGIデバイス参照
	//作成成功時、true
	[[nodiscard]] bool CreateVertex(const DXGIDevice& dxgiDevice) noexcept;

	//指数バッファー等作成
	//DXGIデバイス参照
	//作成成功時、true
	[[nodiscard]] bool CreateIndex(const DXGIDevice& dxgiDevice) noexcept;

	//テクスチャ―バッファー
	//DXGIデバイス参照　コマンドセット_フェンス参照
	//作成成功時、true
	[[nodiscard]] bool CreateTexture(const DXGIDevice& dxgiDevice, Comm_Fence& comm_fence) noexcept;

public:
	Polygon2D() = default;
	~Polygon2D() = default;

	//頂点_指数_バッファー作成
	//DXGIデバイス参照　コマンドセット_フェンス参照
	//作成成功時、true
	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice, Comm_Fence& comm_fence) noexcept;

	//オブジェクト描画
	//DXGIデバイス参照　コマンドリスト参照
	void Draw(const DXGIDevice& dxgiDevice, const CommandList& list) noexcept;
};