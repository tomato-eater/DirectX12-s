#pragma once

#include "Window.h"
#include "DXGIDevice.h"
#include "Comm_Fence.h"
#include "Swap_Target.h"

class Main final {
	Window window{};				//ウィンドウ管理インスタンス
	DXGIDevice dxgiDevice{};		//DXGI_ファクトリー_アダプターとD3D12デバイス管理インスタンス
	Comm_Fence comm_fence{};		//コマンド_キュー_アロケーター リスト_イベント管理インスタンス
	Swap_Target swap_target{};		////スワップチェイン_レンダ―ターゲット(バックバッファ)管理インスタンス

public:
	Main() = default;
	~Main() = default;

	//初期化
	[[nodiscard]] bool Initialize(const HINSTANCE hInstance) noexcept;

	//ループ	
	void Loop() noexcept;
};

