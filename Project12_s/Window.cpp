#include "Window.h"

#include <cassert>

//ウィンドウプロシージャ
LRESULT CALLBACK MyWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg)
	{
	case WM_DESTROY:	//ウィンドウが破棄されたときの処理
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:		//ウィンドウが閉じられたときの処理
		DestroyWindow(hwnd);
		return 0;

	case WM_SIZE:		//ウィンドウサイズが変更されたときの処理

		return 0;

	case WM_KEYDOWN:	//キーが押されたときの処理
		if (wParam == VK_ESCAPE) {	//ESC キーが押された場合
			DestroyWindow(hwnd);
		}
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
};

//ウィンドウ生成
//インスタンスハンドル　タイトル　(ウィンドウサイズ)
//生成成功時、true
[[nodiscard]] bool Window::Create(const HINSTANCE hInstance, const wchar_t* title, const std::pair<uint16_t, uint16_t> s) noexcept {
	RECT windowSize;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &windowSize, 0);
	std::pair<uint16_t, uint16_t> workSpace = { windowSize.right - windowSize.left, windowSize.bottom - windowSize.top };

	//ウィンドウサイズの指定がない場合はFullScreenで作成する
	size = s == std::pair<uint16_t, uint16_t>{} ? workSpace : s;
	//ウィンドウ出力時に中心に来るように計算
	workSpace = { (workSpace.first - size.first) * 0.5f, (workSpace.second - size.second) * 0.5f };

	//ウィンドウクラスの登録
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MyWindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = title;
	RegisterClassExW(&wc);

	//ウィンドウの生成
	if (FAILED(handle = CreateWindowEx(
		0, title, title,
		WS_OVERLAPPEDWINDOW,
		workSpace.first, workSpace.second,
		size.first, size.second,
		nullptr, nullptr,
		hInstance, this)
	)) {
		return false;
	}

	//ウィンドウの表示
	ShowWindow(handle, s == std::pair<uint16_t, uint16_t>{} ? SW_MAXIMIZE : SW_SHOW);

	//ウィンドウを更新
	UpdateWindow(handle);

	return true;
}

//ウィンドウハンドル取得
//ウィンドウハンドル
[[nodiscard]] HWND Window::GetHandle() const noexcept {
	if (!handle) {
		assert(false && "ウィンドウハンドル配布_失敗");
		PostQuitMessage(0);
		return nullptr;
	}
	return handle;
}

//ウィンドウサイズ取得
//ウィンドウサイズ
[[nodiscard]] std::pair<uint16_t, uint16_t> Window::GetSize() const noexcept {
	return size;
}

//メッセージループ
[[nodiscard]] bool Window::messageLoop() const noexcept {
	MSG msg{};

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);


	}

	return true;
}
