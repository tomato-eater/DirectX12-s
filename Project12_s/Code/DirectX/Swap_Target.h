#pragma once

#include "RenderTarget.h"

#include "Comm_Fence.h"

//スワップチェイン_レンダ―ターゲット管理クラス
class Swap_Target final {
	SwapChain swapChain{};		//スワップチェイン管理クラスインスタンス

	RenderTarget renderTarget{};//レンダ―ターゲット(バックバッファ)管理クラスインスタンス

public:
	Swap_Target() = default;
	~Swap_Target() = default;

	//スワップチェインとレンダ―ターゲット(バックバッファ)作成
	//ウィンドウ参照　DXGIデバイス参照　コマンドグループ参照　ヒープ参照
	//両者作成成功時、true
	[[nodiscard]] bool Create(const Window& window, const DXGIDevice& dxgiDevice, const Comm_Fence& command) noexcept;

	//スワップチェインの取得
	//スワップチェインインスタンス
	[[nodiscard]] SwapChain GetSwap() const noexcept;

	//レンダ―ターゲットの取得
	//レンダ―ターゲットインスタンス
	[[nodiscard]] RenderTarget GetTarget() const noexcept;

	//CPU用ハンドルを取得(RTV)
	//レンダ―ターゲットインデックス　DXGIデバイス参照　ヒープ参照
	//CPU用ハンドル(RTV)
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCPU(const UINT index, const DXGIDevice& dxgiDevice) const noexcept;

	//GPU用ハンドルの取得(CSU)
	//レンダ―ターゲットインデックス　ヒープ参照
	//GPU用ハンドル(CSU)
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetHandleGPU(const UINT index) const noexcept;
};