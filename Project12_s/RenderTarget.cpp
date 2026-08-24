#include "RenderTarget.h"

#include "Heap.h"

#include <cassert>

//バックバッファー用レンダ―ターゲット作成
//DXGIデバイス参照　スワップチェイン参照
//作成成功時、true
[[nodiscard]] bool RenderTarget::CreateBack(const DXGIDevice& dxgiDevice, const SwapChain& swapChain) noexcept {
	DXGI_SWAP_CHAIN_DESC1 desc1{};
	swapChain.Get()->GetDesc1(&desc1);

	//サイズを確保
	renderTargets.resize(desc1.BufferCount);
	//バックバッファー取得
	for (int i = 0; i < renderTargets.size(); i++) {
		if (FAILED(swapChain.Get()->GetBuffer(i, IID_PPV_ARGS(&renderTargets.at(i))))) {
			assert(false && "バックバッファー取得_失敗");
			return false;
		}
	}

	//レンダ―ターゲットの設定
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//ヒープ取得
	const auto h = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//ヒープサイズ取得
	const auto size = dxgiDevice.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	for (auto& target : renderTargets) {
		//ヒープ番号取得
		const auto idx = HeapManager::Ins().GetNum(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		if (!idx.has_value()) {
			assert(false && "RTVヒープ確保_失敗");
			return false;
		}
		const auto index = idx.value();
		//ハンドル取得
		auto handle = h->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += index * size;
		//レンダ―ターゲットビュー作成
		dxgiDevice.GetDevice()->CreateRenderTargetView(target.Get(), &desc, handle);
		rtvHeapNum.emplace_back(index);
	}

	return true;
}

//レンダ―ターゲットの取得
//レンダ―ターゲットインデックス
//レンダ―ターゲットのポインター
[[nodiscard]] ID3D12Resource* RenderTarget::Get(const UINT index) const noexcept {
	if (index < 0 || renderTargets.size() <= index) {
		assert(false && "レンダーターゲット_範囲外");
		return nullptr;
	}
	if (!renderTargets.at(index)) {
		assert(false && "レンダ―ターゲット配布_失敗");
		return nullptr;
	}

	return renderTargets.at(index).Get();
}

//CPU用ハンドルを取得(RTV)
//レンダ―ターゲットインデックス　DXGIデバイス参照
//CPU用ハンドル(RTV)
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::GetHandleCPU(const UINT index, const DXGIDevice& dxgiDevice) const noexcept {
	if (index < 0 || renderTargets.size() <= index) {
		assert(false && "レンダーターゲット_範囲外");
		return D3D12_CPU_DESCRIPTOR_HANDLE();
	}

	auto handle = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += rtvHeapNum.at(index) * dxgiDevice.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	return handle;
}