#pragma once

#include <Windows.h>
#include <array>

//入力管理クラス　シングルトン
class Input final {
	Input() = default;
	~Input() = default;

	Input(const Input& r) = delete;
	Input& operator=(const Input& r) = delete;
	Input(Input&& r) = delete;
	Input& operator=(Input&& r) = delete;


	std::array<byte, 256> state{};		//キー情報配列
	std::array<byte, 256> prevState{};	//キー情報配列

	std::array<float, 2> mouse{};

public:

	//インスタンス取得
	//インスタンス参照
	static Input& Ins() noexcept {
		static Input instance;
		return instance;
	}


	//入力情報更新
	void Update() noexcept;



	//入力状態更新
	//キー情報
	void UpdateState(void* pState) noexcept;

	//入力情報取得
	//取得したい情報
	//押されているなら、true
	[[nodiscard]] bool PressKey(const uint16_t key) const noexcept;

	//入力情報取得
	//取得したい情報
	//押されたら、true
	[[nodiscard]] bool TriggerKey(const uint16_t key) const noexcept;


	//マウス移動量登録
	//マウス移動量
	void UpdateMouse(float x, float y) noexcept;

	//マウス移動量取得
	//マウス移動量
	std::array<float, 2> GetMouseMove() noexcept;


};