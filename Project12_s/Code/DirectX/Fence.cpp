#include "Fence.h"

#include <cassert>

//フェンス作成
//DXGIデバイス参照
//両者作成成功時、true
[[nodiscard]] bool Fence::Create(const DXGIDevice& dxgiDevice) noexcept {
	//フェンス作成
	if (FAILED(dxgiDevice.GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)))) {
		assert(false && "フェンス作成_失敗");
		return false;
	}
	//イベント作成
	event = CreateEvent(nullptr, false, false, L"WAIT_GPU");
	if (!event) {
		assert(false && "イベント作成_失敗");
		return false;
	}
	return true;
}

//フェンスの確認
//フェンス値
void Fence::Check(const uint64_t value) const noexcept {
	if (!fence) {
		assert(false && "フェンス_未作成");
		return;
	}
	//フェンス値が設定された値以上か
	if (fence->GetCompletedValue() < value) {
		fence->SetEventOnCompletion(value, event);
		WaitForSingleObject(event, INFINITE);
	}
}

//フェンスの取得
//フェンスのポインター
[[nodiscard]] ID3D12Fence* Fence::GetFence() const noexcept {
	if (!fence) {
		assert(false && "フェンス配布_失敗");
		return nullptr;
	}
	return fence.Get();
}