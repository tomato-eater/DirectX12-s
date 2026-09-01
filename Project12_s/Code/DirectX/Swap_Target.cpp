#include "Swap_Target.h"

#include <cassert>

//スワップチェインとレンダ―ターゲット(バックバッファ)作成
//ウィンドウ参照　DXGIデバイス参照　コマンドグループ参照　ヒープ参照
//両者作成成功時、true
[[nodiscard]] bool Swap_Target::Create(const Window& window, const DXGIDevice& dxgiDevice, const Comm_Fence& command) noexcept {
    //スワップチェイン作成
    if (!swapChain.Create(window, command.GetBuffCount(), dxgiDevice, command.Queue())) {
        assert(false && "スワップチェイン作成_失敗");
        return false;
    }
    //レンダ―ターゲット(バックバッファ)作成
    if (!renderTarget.CreateBack(dxgiDevice, swapChain)) {
        assert(false && "レンダ―ターゲット(バックバッファ)作成_失敗");
        return false;
    }
    return true;
}

//スワップチェインの取得
//スワップチェインインスタンス
[[nodiscard]] SwapChain Swap_Target::GetSwap() const noexcept {
    return swapChain;
}

//レンダ―ターゲットの取得
//レンダ―ターゲットインスタンス
[[nodiscard]] RenderTarget Swap_Target::GetTarget() const noexcept {
    return renderTarget;
}

//CPU用ハンドルを取得(RTV)
//レンダ―ターゲットインデックス　DXGIデバイス参照　ヒープ参照
//CPU用ハンドル(RTV)
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE Swap_Target::GetHandleCPU(const UINT index, const DXGIDevice& dxgiDevice) const noexcept {
    return renderTarget.GetHandleCPU(index, dxgiDevice);
}