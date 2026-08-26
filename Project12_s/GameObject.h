#pragma once

#include "ConstantBuffer.h"

#include "DXGIDevice.h"
#include "CommandList.h"

//プレイヤー管理クラス
class GameObject final {
	ConstantBuffer<ObjectData> constant{};	//定数バッファー管理インスタンス

	DirectX::XMMATRIX world{};	//行列

	DirectX::XMFLOAT3 position{};	//位置
	DirectX::XMFLOAT3 rotation{};	//回転
	DirectX::XMFLOAT3 scale{};		//尺度

public:
	GameObject() = default;
	~GameObject() = default;

	//カメラ作成
	//作成成功時、true
	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice) noexcept;

	//座標等の設定
	//場所　回転　尺度
	void Set(DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 r, DirectX::XMFLOAT3 s) noexcept;

	//更新
	void Update() noexcept;

	//マップ
	//DXGIデバイス参照
	void Map(const DXGIDevice& dxgiDevice, const CommandList& list) noexcept;

	//ヒープ番号取得
	[[nodiscard]] UINT GetNum() const noexcept;
};