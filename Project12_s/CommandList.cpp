#include "CommandList.h"

#include <cassert>

//リスト作成
//DXGIデバイス参照　設定したい属性
//作成成功時、true
[[nodiscard]] bool CommandList::Create(const DXGIDevice& dxgiDevice, const CommandAllocator& allocator, const D3D12_COMMAND_LIST_TYPE type) noexcept {
	//コマンドリスト作成
	if (FAILED(dxgiDevice.GetDevice()->CreateCommandList(0, type, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)))) {
		assert(false && "コマンドリスト作成_失敗");
		return false;
	}
	//初期状態でクローズ
	commandList->Close();
	return true;
}

//リストの取得
//リストのポインター
[[nodiscard]] ID3D12GraphicsCommandList* CommandList::Get() const noexcept {
	if (!commandList) {
		assert(false && "コマンドリスト配布_失敗");
		return nullptr;
	}
	return commandList.Get();
}

//リソースバリア変更
void CommandList::ResourceBarrier(const Microsoft::WRL::ComPtr<ID3D12Resource> resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after) noexcept {
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;

	commandList.Get()->ResourceBarrier(1, &barrier);
}
