#include "CommandQueue.h"

#include <cassert>

//キュー作成
//DXGIデバイスの参照
//作成成功時、true
[[nodiscard]] bool CommandQueue::Create(const DXGIDevice& dxgiDevice) noexcept {
	//キューの設定
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	//キュー作成
	if (FAILED(dxgiDevice.GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)))) {
		assert(false && "コマンドキュー作成_失敗");
		return false;
	}
	return true;
}

//キューの取得
//キューのポインター
[[nodiscard]] ID3D12CommandQueue* CommandQueue::Get() const noexcept {
	if (!commandQueue) {
		assert(false && "コマンドキュー配布_失敗");
		return nullptr;
	}
	return commandQueue.Get();
}