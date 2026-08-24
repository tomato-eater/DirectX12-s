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