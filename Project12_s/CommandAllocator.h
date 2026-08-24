#pragma once

#include "DXGIDevice.h"

//コマンドアロケーター管理クラス
class CommandAllocator final {
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator{};	//コマンドアロケーター

public:
	CommandAllocator() = default;
	~CommandAllocator() = default;

	//アロケーター作成(複数)
	//DXGIデバイス参照　設定したい属性
	//作成成功時、true
	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice, const D3D12_COMMAND_LIST_TYPE type) noexcept;

	//アロケーターの取得
	//アロケーターインデックス
	//アロケーターのポインター
	[[nodiscard]] ID3D12CommandAllocator* Get() const noexcept;
};

