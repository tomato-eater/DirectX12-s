#include "Input.h"
#include <string>

//入力情報更新
void Input::Update() noexcept {
	std::memcpy(prevState.data(), state.data(), state.size());
	mouse = {};
}

//入力状態更新
//キー情報
void Input::UpdateState(void* pState) noexcept {
	std::memcpy(state.data(), pState, state.size());
}

//入力情報取得
//取得したい情報
//押されているなら、true
[[nodiscard]] bool Input::PressKey(const uint16_t key) const noexcept {
	return (state[key] & 0x80);
}

//入力情報取得
//取得したい情報
//押されたら、true
[[nodiscard]] bool Input::TriggerKey(const uint16_t key) const noexcept {
	return ((state[key] & 0x80) && !(prevState[key] & 0x80));
}



//マウスの移動量取得
//マウスの移動量
void Input::UpdateMouse(float x, float y) noexcept {
	mouse = { x,y };
}

//マウス移動量取得
//マウス移動量
std::array<float, 2> Input::GetMouseMove() noexcept {
	return mouse;
}

