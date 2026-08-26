#include "GameObject.h"

//カメラ作成
//作成成功時、true
[[nodiscard]] bool GameObject::Create(const DXGIDevice& dxgiDevice) noexcept {
	//定数バッファー作成
	if (!constant.CreateConstance(dxgiDevice, sizeof(ObjectData))) {
		assert(false && "定数バッファー作成_失敗");
		return false;
	}
	return true;
}

//座標等の設定
//場所　回転　尺度
void GameObject::Set(DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 r, DirectX::XMFLOAT3 s) noexcept {
	position = p;
	rotation = r;
	scale = s;
}

//更新
void GameObject::Update() noexcept {


	DirectX::XMMATRIX matS = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX matR = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	DirectX::XMMATRIX matP = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	world = matS * matR * matP;
}

//マップ
void GameObject::Map(const DXGIDevice& dxgiDevice, const CommandList& list) noexcept {
	constant.UpdateConst(ObjectData{ DirectX::XMMatrixTranspose(world) });

	auto handle = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += constant.GetNum() * dxgiDevice.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	list.Get()->SetGraphicsRootDescriptorTable(1, handle);
}