#include "Comm_Fence.h"

#include <cassert>

//コマンド_キュー_アロケーター_リスト フェンス作成
//DXGIデバイスの参照　リストの属性
//全て作成成功で、true
[[nodiscard]] bool Comm_Fence::Create(const DXGIDevice& dxgiDevice, const D3D12_COMMAND_LIST_TYPE type) noexcept {
	//コマンドキュー作成
	if (!queue.Create(dxgiDevice)) {
		assert(false && "コマンドキュー作成_失敗");
		return false;
	}
	//コマンドアロケーター作成(複数)
	for (auto& allocator : allocators) {
		if (!allocator.Create(dxgiDevice, type)) {
			assert(false && "コマンドアロケーター作成_失敗");
			return false;
		}
	}
	//コマンドリスト作成
	if (!list.Create(dxgiDevice, allocators.at(0), type)) {
		assert(false && "コマンドリスト作成_失敗");
		return false;
	}
	//フェンス作成
	if (!fence.Create(dxgiDevice)) {
		assert(false && "フェンス作成_失敗");
		return false;
	}
	return true;
}

//バッファー数取得
//バッファー数
[[nodiscard]] uint8_t Comm_Fence::GetBuffCount() const noexcept {
	return bufferCount;
}

//キューの取得
//キューのインスタンス
[[nodiscard]] CommandQueue Comm_Fence::Queue() const noexcept {
	return queue;
}

//アロケーターの取得
//アロケーターインデックス
//アロケーターのインスタンス
[[nodiscard]] CommandAllocator Comm_Fence::Allocator(const uint8_t index) const noexcept {
	if (index < 0 || allocators.size() <= index) {
		assert(false && "コマンドアロケーター_範囲外を指定");
		return CommandAllocator{};
	}
	return allocators.at(index);
}

//リストの取得
//リストのインスタンス
[[nodiscard]] CommandList Comm_Fence::List() const noexcept {
	return list;
}

//フェンス待機
void Comm_Fence::WaitFence() noexcept {
	queue.Get()->Signal(fence.GetFence(), next);
	fence.Check(next);
	values = { next, next };
	next++;
}

//フェンスの確認　アロケーターとリストのリセット
//アロケーターインデックス
void Comm_Fence::Reset(const uint8_t index) noexcept {
	if (index < 0 || values.size() <= index) {
		assert(false && "フェンス値_範囲外");
		return;
	}
	//フェンスの確認
	fence.Check(values.at(index));

	if (index < 0 || allocators.size() <= index) {
		assert(false && "コマンドアロケータリセット_失敗");
		return;
	}
	//リセット
	allocators.at(index).Get()->Reset();
	list.Get()->Reset(allocators.at(index).Get(), nullptr);
}

//ビューポート_シザー矩形の設定
//縦横サイズ
void Comm_Fence::ViewportRect(const std::pair<uint16_t, uint16_t> size) noexcept {
	//ビューボードの設定
	D3D12_VIEWPORT viewport{
		0.0f, 0.0f,
		static_cast<float>(size.first), static_cast<float>(size.second),
		0.0f, 1.0f
	};
	list.Get()->RSSetViewports(1, &viewport);

	//シザー矩形の設定
	D3D12_RECT rect = {
		0, 0,
		static_cast<INT64>(size.first), static_cast<UINT64>(size.second)
	};
	list.Get()->RSSetScissorRects(1, &rect);
}

void Comm_Fence::Execute() noexcept {
	//コマンドリスト閉鎖
	list.Get()->Close();
	//リスト→キューに送信
	ID3D12CommandList* ppLists[] = { list.Get() };
	queue.Get()->ExecuteCommandLists(_countof(ppLists), ppLists);
}

//コマンドリスト閉鎖　リストをキューに送信　フェンス更新
//スワップチェイン参照　バックバッファーインデックス
void Comm_Fence::Present(const SwapChain& swapChain, const uint8_t backBuffIdx) noexcept {
	//プレゼント
	swapChain.Get()->Present(1, 0);
	//フェンス更新
	queue.Get()->Signal(fence.GetFence(), next);
	values.at(backBuffIdx) = next++;
}