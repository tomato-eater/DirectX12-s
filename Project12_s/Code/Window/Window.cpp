#include "Window.h"

#include "../Input/Input.h"
#include <vector>

#include <cassert>

//カーソル幽閉
void LockCursor(const HWND& hwnd) {
	if (!hwnd) {
		ClipCursor(nullptr);
		return;
	}
	RECT rect{};
	//描画範囲取得
	GetClientRect(hwnd, &rect);
	//スクリーン座標に変換
	POINT LT = { rect.left, rect.top };
	POINT RB = { rect.right, rect.bottom };
	ClientToScreen(hwnd, &LT);
	ClientToScreen(hwnd, &RB);
	//再設定
	RECT clipRect = { LT.x, LT.y, RB.x, RB.y };
	//制限
	ClipCursor(&clipRect);
}

//ウィンドウプロシージャ
LRESULT CALLBACK MyWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	auto mouseX = LOWORD(lParam);
	auto mouseY = HIWORD(lParam);

	switch (uMsg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) != WA_INACTIVE) {
			LockCursor(hwnd);
		}
		else {
			LockCursor(nullptr);
		}
		break;

	case WM_DESTROY:	//ウィンドウが破棄されたときの処理
		LockCursor(nullptr);
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:		//ウィンドウが閉じられたときの処理
		DestroyWindow(hwnd);
		return 0;


	case WM_KEYDOWN:	//キーが押されたときの処理
		if (wParam == VK_ESCAPE) {	//ESC キーが押された場合
			LockCursor(nullptr);
		}
		return 0;

	case WM_LBUTTONDOWN: // 左クリック時
	case WM_RBUTTONDOWN: // 右クリック時
		if (GetActiveWindow() == hwnd) {
			LockCursor(hwnd);
		}
		break;

	case WM_INPUT:	//インプット
		UINT dwSize = 0;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

		if (dwSize > 0) {
			std::vector<char> buff(dwSize);
			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buff.data(), &dwSize, sizeof(RAWINPUTHEADER)) == dwSize) {
				auto& raw = *(RAWINPUT*)buff.data();
				if (raw.header.dwType == RIM_TYPEMOUSE) {
					// マウスの相対的な移動量（加速無しの生の数値）
					LONG x = raw.data.mouse.lLastX;
					LONG y = raw.data.mouse.lLastY;

					if (x != 0 || y != 0) {
						Input::Ins().UpdateMouse(x, y);
					}
				}
			}
			buff.clear();
		}
		break;
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

	//ウィンドウ生成
	if (FAILED(handle = CreateWindowEx(
		0, title, title,
		WS_OVERLAPPEDWINDOW,
		workSpace.first, workSpace.second,
		size.first, size.second,
		nullptr, nullptr,
		hInstance, nullptr)
	)) {
		assert(false && "ウィンドウ生成_失敗");
		return false;
	}
	//RawInput登録
	RAWINPUTDEVICE rawInput{};
	rawInput.usUsagePage = 0x01;
	rawInput.usUsage = 0x02;
	rawInput.dwFlags = RIDEV_INPUTSINK;
	rawInput.hwndTarget = handle;
	if (!RegisterRawInputDevices(&rawInput, 1, sizeof(rawInput))) {
		assert(false && "RawInputデバイス登録_失敗");
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

	Input::Ins().Update();

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		//キー情報取得
		static byte key[256]{};
		if (GetKeyboardState(key)) {
			Input::Ins().UpdateState(key);
		}

	}

	return true;
}
