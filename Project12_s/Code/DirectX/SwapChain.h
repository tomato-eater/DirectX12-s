#pragma once

#include "../Window/Window.h"
#include "CommandQueue.h"

//スワップチェイン管理クラス
class SwapChain final {
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;	//スワップチェイン

public:
	SwapChain() = default;
	~SwapChain() = default;

	//スワップチェイン作成
	//ウィンドウの参照　バッファ―数　DXGIデバイスの参照　コマンドグループの参照
	//作成成功時、true
	[[nodiscard]] bool Create(const Window& window, const uint8_t bufferCount, const DXGIDevice& dxgiDevice, const CommandQueue& queue) noexcept;

	//スワップチェインの取得
	//スワップチェインのポインター
	[[nodiscard]] IDXGISwapChain4* Get() const noexcept;

};