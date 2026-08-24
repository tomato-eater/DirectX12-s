#include "SwapChain.h"

#include <cassert>

//スワップチェイン作成
//ウィンドウの参照　バッファー数　DXGIデバイスの参照　コマンドグループの参照
//作成成功時、true
[[nodiscard]] bool SwapChain::Create(const Window& window, const uint8_t bufferCount, const DXGIDevice& dxgiDevice, const CommandQueue& queue) noexcept {
    //サイズの取得
    const auto size = window.GetSize();

    //スワップチェインの設定
    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.BufferCount = bufferCount;
    desc.Width = size.first;
    desc.Height = size.second;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Stereo = FALSE;
    desc.Scaling = DXGI_SCALING_STRETCH;
    desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> swap1{};
    //スワップチェインの作成
    if (FAILED(dxgiDevice.GetFactory()->CreateSwapChainForHwnd(queue.Get(), window.GetHandle(), &desc, nullptr, nullptr, &swap1))) {
        assert(false && "スワップチェイン作成_失敗");
        return false;
    }
    //スワップチェインアップデート
    if (FAILED(swap1->QueryInterface(IID_PPV_ARGS(&swapChain)))) {
        assert(false && "スワップチェイン更新_失敗");
        return false;
    }

    return true;
}

//スワップチェインの取得
//スワップチェインのポインター
[[nodiscard]] IDXGISwapChain4* SwapChain::Get() const noexcept {
    if (!swapChain) {
        assert(false && "スワップチェイン配布_失敗");
        return nullptr;
    }
    return swapChain.Get();
}