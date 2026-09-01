#pragma once

#include "DXGIDevice.h"
#include <cstdint>

//フェンス管理クラス
class Fence final {
	Microsoft::WRL::ComPtr<ID3D12Fence> fence{};	//フェンス
	HANDLE event{};

public:
	Fence() = default;
	~Fence() = default;

	//フェンス_イベント作成
	//DXGIデバイス参照
	//両者作成成功時、true
	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice) noexcept;

	//フェンスの確認
	//フェンス値
	void Check(const uint64_t value) const noexcept;

	//フェンスの取得
	//フェンスのポインター
	[[nodiscard]] ID3D12Fence* GetFence() const noexcept;
};