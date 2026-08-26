#pragma once

#include "ConstantBuffer.h"
#include "Window.h"

#include "DXGIDevice.h"
#include "CommandList.h"

//カメラ管理クラス
class Camera final {
	ConstantBuffer<CameraData> constant{};	//定数バッファー管理インスタンス

	DirectX::XMMATRIX view{};	//ビュー行列
	DirectX::XMMATRIX proj{};	//射影行列

	DirectX::XMFLOAT3 position{};	//位置
	DirectX::XMFLOAT3 focus{};		//焦点
	DirectX::XMFLOAT3 top{};		//上

public:
	Camera() = default;
	~Camera() = default;

	//カメラ作成
	//作成成功時、true
	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice) noexcept;

	//座標等の設定
	//場所　焦点　上　ウィンドウ参照
	void Set(DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 f, DirectX::XMFLOAT3 t, const Window& window) noexcept;

	//更新
	void Update() noexcept;

	//マップ
	//DXGIデバイス参照　コマンドリスト参照
	void Map(const DXGIDevice& dxgiDevice, const CommandList& list) noexcept;

	//ヒープ番号取得
	[[nodiscard]] UINT GetNum() const noexcept;
};

