#pragma once

#include "CommandAllocator.h"

//コマンドリスト管理クラス
class CommandList final {
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList{};	//コマンドリスト

public:
	CommandList() = default;
	~CommandList() = default;

	//リスト作成
	//DXGIデバイス参照　コマンドアロケーター参照　設定したい属性
	//作成成功時、true
	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice, const CommandAllocator& allocator, const D3D12_COMMAND_LIST_TYPE type) noexcept;

	//リストの取得
	//リストのポインター
	[[nodiscard]] ID3D12GraphicsCommandList* Get() const noexcept;

	//リソースバリア変更
	void ResourceBarrier(const Microsoft::WRL::ComPtr<ID3D12Resource> resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after) noexcept;
};

