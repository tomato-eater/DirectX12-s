#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <vector>

#include "../../../DirectX/Comm_Fence.h"

//描画モデルのテンプレート
class Model {
protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer{};	//頂点バッファー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};			//頂点バッファービュー

	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer{};	//指数バッファー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};				//指数バッファービュー

	UINT indexCount{};	//インデックス数

	Microsoft::WRL::ComPtr<ID3D12Resource> textureBuffer{};	//テクスチャーバッファー

	std::vector<UINT> heapNum{};	//使用しているヒープの番号

public:
	Model() = default;
	~Model() = default;

	//バッファー作成
	//DXGIデバイス参照　コマンドセット_フェンス参照　データポインター　データサイズ　頂点/指数_バッファー　アップロード用バッファ―
	//作成成功時、true
	[[nodiscard]] bool CreateBuffer(const DXGIDevice& dxgiDevice, const Comm_Fence& comm_fence, const void* pData, const UINT64 dataSize, Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, Microsoft::WRL::ComPtr<ID3D12Resource>& upBuffer) noexcept;

	//テクスチャーバッファー作成
	//DXGIデバイス参照　コマンドセット_フェンス参照　テクスチャーパス　アップロード用バッファ―
	//作成成功時、true
	[[nodiscard]] bool CreateTexture(const DXGIDevice& dxgiDevice, const Comm_Fence& comm_fence, const wchar_t* path, Microsoft::WRL::ComPtr<ID3D12Resource>& upBuffer) noexcept;

};

