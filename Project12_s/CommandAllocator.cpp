#include "CommandAllocator.h"

#include <cassert>

//アロケーター作成(複数)
//DXGIデバイス参照　設定したい属性
//作成成功時、true
[[nodiscard]] bool CommandAllocator::Create(const DXGIDevice& dxgiDevice, const D3D12_COMMAND_LIST_TYPE type) noexcept {
	//コマンドアロケーター作成
	if (FAILED(dxgiDevice.GetDevice()->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)))) {
		assert(false && "コマンドアロケーター作成_失敗");
		return false;
	}
	return true;
}

//アロケーターの取得
//アロケーターインデックス
//アロケーターのポインター
[[nodiscard]] ID3D12CommandAllocator* CommandAllocator::Get() const noexcept {
	if (!commandAllocator) {
		assert(false && "コマンドアロケーター配布_失敗");
		return nullptr;
	}
	return commandAllocator.Get();
}