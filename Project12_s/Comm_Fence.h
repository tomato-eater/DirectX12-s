#pragma once

#include <array>

#include "CommandQueue.h"
#include "CommandList.h"
#include "Fence.h"
#include "SwapChain.h"

//コマンド_キュー_アロケーター_リスト フェンス管理クラス
class Comm_Fence final {
	static constexpr uint8_t bufferCount = 2;				//バッファー数
	CommandQueue queue{};									//コマンドキュー管理インスタンス
	std::array<CommandAllocator, bufferCount> allocators{};	//コマンドアロケーター管理インスタンス(複数)
	CommandList list{};										//コマンドリスト管理インスタンス

	Fence fence{};								//フェンス管理インスタンス
	std::array<uint64_t, bufferCount> values{};	//フェンス値
	uint64_t next = 1;							//次のフェンス値

public:
	Comm_Fence() = default;
	~Comm_Fence() = default;

	//コマンド_キュー_アロケーター_リスト フェンス作成
	//DXGIデバイスの参照　リストの属性
	//全て作成成功で、true
	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice, const D3D12_COMMAND_LIST_TYPE type) noexcept;

	//バッファー数取得
	//バッファー数
	[[nodiscard]] uint8_t GetBuffCount() const noexcept;

	//キューの取得
	//キューのインスタンス
	[[nodiscard]] CommandQueue Queue() const noexcept;

	//アロケーターの取得
	//アロケーターインデックス
	//アロケーターのインスタンス
	[[nodiscard]] CommandAllocator Allocator(uint8_t index) const noexcept;

	//リストの取得
	//リストのインスタンス
	[[nodiscard]] CommandList List() const noexcept;

	//フェンス待機
	void WaitFence() noexcept;

	//フェンスの確認　アロケーターとリストのリセット
	//アロケーターインデックス
	void Reset(uint8_t index) noexcept;

	//ビューポート_シザー矩形の設定
	//縦横サイズ
	void ViewportRect(const std::pair<uint16_t, uint16_t> size) noexcept;

	//コマンドリスト閉鎖　リストをキューに送信
	//スワップチェイン参照　バックバッファーインデックス
	void Execute() noexcept;

	//プレゼント　フェンス更新
	//スワップチェイン参照　バックバッファーインデックス
	void Present(const SwapChain& swapChain, const uint8_t backBuffIdx) noexcept;
};

