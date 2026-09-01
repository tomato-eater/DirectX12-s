#pragma once

#include "../Const/ConstantBuffer.h"
#include "../../Window/Window.h"

#include "../../DirectX/DXGIDevice.h"
#include "../../DirectX/CommandList.h"

//カメラ管理クラス
class Camera final {
	ConstantBuffer<CameraData> constant{};	//定数バッファー管理インスタンス

	DirectX::XMMATRIX view{};	//ビュー行列
	DirectX::XMMATRIX proj{};	//射影行列

	float yaw{};	//左右
	float pitch{};	//上下
	float radius{};	//距離

public:
	Camera() = default;
	~Camera() = default;

	//カメラ作成
	//作成成功時、true
	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice) noexcept;

	//座標等の設定
	//ターゲットとの距離
	void Set(const float r, const Window& window) noexcept;

	//更新
	//プレイヤーの位置
	void Update(const DirectX::XMFLOAT3 playerPos) noexcept;

	//マップ
	//DXGIデバイス参照　コマンドリスト参照
	void Map(const DXGIDevice& dxgiDevice, const CommandList& list) noexcept;

	//ヒープ番号取得
	[[nodiscard]] UINT GetNum() const noexcept;
};

