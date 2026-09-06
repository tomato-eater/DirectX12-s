#pragma once

#include <wrl/client.h>
#include <d3d12.h>

#include "../Window/Window.h"
#include "DXGIDevice.h"

//深度バッファー管理クラス
class DepthBuffer final {
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer{};	//デプスバッファ―
	UINT heapNum{};		//ヒープ番号
	D3D12_CPU_DESCRIPTOR_HANDLE handle{};	//ハンドル

public:
	DepthBuffer() = default;
	~DepthBuffer() = default;

	//深度バッファー作成
	//ウィンドウ参照　DXGIデバイス参照
	//作成成功時、true
	[[nodiscard]] bool Create(const Window& window, const DXGIDevice& dxgiDevice) noexcept;

	//深度ハンドルの取得
	//深度ハンドル
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() const noexcept;
};

