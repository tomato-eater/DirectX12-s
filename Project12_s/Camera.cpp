#include "Camera.h"

#include "Heap.h"

#include "Input.h"

//カメラ作成
//作成成功時、true
[[nodiscard]] bool Camera::Create(const DXGIDevice& dxgiDevice) noexcept {
	//定数バッファー作成
	if (!constant.CreateConstance(dxgiDevice, sizeof(CameraData))) {
		assert(false && "定数バッファー作成_失敗");
		return false;
	}
	return true;
}

//座標等の設定
//ターゲットとの距離
void Camera::Set(const float r, const Window& window) noexcept {
	radius = r;
	const auto size = window.GetSize();

	proj = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XM_PIDIV4,			// 視野角
		static_cast<float>(size.first) / static_cast<float>(size.second),   // アスペクト比
		0.1f,						// ニアクリップ
		100.0f						// ファークリップ
	);
}


//更新
//プレイヤーの位置
void Camera::Update(const DirectX::XMFLOAT3 playerPos) noexcept {
	
	//focus = playerPos;

	auto move = Input::Ins().GetMouseMove();
	yaw += move.at(0) * 0.01f;
	pitch += move.at(1) * -0.01f;

	float limit = DirectX::XMConvertToRadians(70.0f);
	if (pitch > limit) pitch = limit;
	if (pitch < -limit) pitch = -limit;

	DirectX::XMVECTOR target = DirectX::XMLoadFloat3(&playerPos);

	auto cosPitch = cosf(pitch);
	auto sinPitch = sinf(pitch);
	auto cosYaw = cosf(yaw);
	auto sinYaw = sinf(yaw);

	DirectX::XMVECTOR cameraOffset = DirectX::XMVectorSet(
		radius * cosPitch * sinYaw,	//X
		radius * sinPitch,			//Y
		radius * cosPitch * cosYaw,	//Z
		0.0f
	);

	DirectX::XMVECTOR pos = DirectX::XMVectorAdd(target, cameraOffset);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


	//ビューの計算
	view = DirectX::XMMatrixLookAtLH(
		pos,
		target,
		up
		);
}

//マップ
//DXGIデバイス参照　コマンドリスト参照
void Camera::Map(const DXGIDevice& dxgiDevice, const CommandList& list) noexcept {
	constant.UpdateConst(CameraData{ DirectX::XMMatrixTranspose(view), DirectX::XMMatrixTranspose(proj) });

	auto handle = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += constant.GetNum() * dxgiDevice.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	list.Get()->SetGraphicsRootConstantBufferView(0, constant.constBuffer->GetGPUVirtualAddress());
}

//ヒープ番号取得
[[nodiscard]] UINT Camera::GetNum() const noexcept {
	return constant.GetNum();
}