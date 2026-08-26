#include "Camera.h"

#include "Heap.h"

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
//場所　焦点　上　ウィンドウ参照
void Camera::Set(DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 f, DirectX::XMFLOAT3 t, const Window& window) noexcept {
	position = p;
	focus = f;
	top = t;

	const auto size = window.GetSize();

	proj = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XM_PIDIV4,			// 視野角
		static_cast<float>(size.first) / static_cast<float>(size.second),   // アスペクト比
		0.1f,						// ニアクリップ
		100.0f						// ファークリップ
	);
}


//更新
void Camera::Update() noexcept {


	//ビューの計算
	view = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&position),
		DirectX::XMLoadFloat3(&focus),
		DirectX::XMLoadFloat3(&top)
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