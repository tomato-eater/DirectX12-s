#pragma once

#include "DXGIDevice.h"

//コマンドキュー管理クラス
class CommandQueue final{
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue{};	//コマンドキュー

public:
	CommandQueue() = default;
	~CommandQueue() = default;

	//キュー作成
	//DXGIデバイスの参照
	//作成成功時、true
	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice) noexcept;

	//キューの取得
	//キューのポインター
	[[nodiscard]] ID3D12CommandQueue* Get() const noexcept;
};

