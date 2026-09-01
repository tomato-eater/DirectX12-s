#pragma once

#include <vector>
#include "SwapChain.h"

//レンダ―ターゲット管理クラス
class RenderTarget final {
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> renderTargets{};	//レンダ―ターゲット
	std::vector<UINT> rtvHeapNum{};	//rtvヒープ番号

public:
	RenderTarget() = default;
	~RenderTarget() = default;

	//バックバッファー用レンダ―ターゲット作成
	//DXGIデバイス参照　スワップチェイン参照
	//作成成功時、true
	[[nodiscard]] bool CreateBack(const DXGIDevice& dxgiDevice, const SwapChain& swapChain) noexcept;

	//レンダ―ターゲットの取得
	//レンダ―ターゲットインデックス
	//レンダ―ターゲットのポインター
	[[nodiscard]] ID3D12Resource* Get(const UINT index) const noexcept;

	//CPU用ハンドルを取得(RTV)
	//レンダ―ターゲットインデックス　DXGIデバイス参照
	//CPU用ハンドル(RTV)
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCPU(const UINT index, const DXGIDevice& dxgiDevice) const noexcept;

	//GPU用ハンドルの取得(CSU)
	//レンダ―ターゲットインデックス
	//GPU用ハンドル(CSU)
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetHandleGPU(const UINT index) const noexcept;
};