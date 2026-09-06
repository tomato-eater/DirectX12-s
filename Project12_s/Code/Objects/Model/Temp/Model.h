#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <vector>

#include <DirectXTex.h>

#include "../../../DirectX/Comm_Fence.h"
#include <optional>

//描画パーツ
struct RenderMesh {
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};	//頂点バッファービュー
	D3D12_VERTEX_BUFFER_VIEW texCoordView{};		//UVバッファービュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};		//指数バッファービュー
	UINT indexCount{};	//インデックス数
	std::optional<UINT> heapNum{};	//使用しているヒープの番号
};

//描画モデルのテンプレート
class Model {
protected:
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> buffer{};	//頂点_指数バッファー
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> textureBuffer{};	//テクスチャーバッファー

	std::vector<RenderMesh> renMesh{};	//描画情報の塊

public:
	Model() = default;
	~Model() = default;

	//バッファー作成
	//DXGIデバイス参照　コマンドセット_フェンス参照　データポインター　データサイズ　頂点/指数_バッファー　アップロード用バッファ―
	//作成成功時、true
	[[nodiscard]] bool CreateBuffer(const DXGIDevice& dxgiDevice, const Comm_Fence& comm_fence, const void* pData, const UINT64 dataSize, Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, Microsoft::WRL::ComPtr<ID3D12Resource>& upBuffer) noexcept;

	//テクスチャーバッファー作成
	//DXGIデバイス参照　コマンドセット_フェンス参照　メタデータ　イメージ　テクスチャ―バッファー　アップロード用バッファ―
	//作成成功時、true
	[[nodiscard]] bool CreateTexture(const DXGIDevice& dxgiDevice, const Comm_Fence& comm_fence, const DirectX::TexMetadata& metadata, const DirectX::Image& image, Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, Microsoft::WRL::ComPtr<ID3D12Resource>& upBuffe, std::optional<UINT>& heapNum) noexcept;

};

