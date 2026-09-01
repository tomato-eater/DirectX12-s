#pragma once

#include <Windows.h>
#include <utility>

//ウィンドウ管理クラス
class Window final {
	HWND handle{};		//ウィンドウハンドル
	std::pair<uint16_t, uint16_t> size{};	//ウィンドウサイズ

public:
	Window() = default;
	~Window() = default;

	//ウィンドウ生成
	//インスタンスハンドル　タイトル　(ウィンドウサイズ)
	//生成成功時、true
	[[nodiscard]] bool Create(const HINSTANCE hInstance, const wchar_t* title, const std::pair<uint16_t, uint16_t> s = {}) noexcept;

	//ウィンドウハンドル取得
	//ウィンドウハンドル
	[[nodiscard]] HWND GetHandle() const noexcept;

	//ウィンドウサイズ取得
	//ウィンドウサイズ
	[[nodiscard]] std::pair<uint16_t, uint16_t> GetSize() const noexcept;

	//メッセージループ
	[[nodiscard]] bool messageLoop() const noexcept;
};

