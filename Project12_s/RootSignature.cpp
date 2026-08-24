#include "RootSignature.h"

#include <cassert>

//ルートシグネチャー作成
//ルートシグネチャーの設定　DXGIデバイス参照
//作成成功時、true
[[nodiscard]] bool RootSignature::Create(const D3D12_ROOT_SIGNATURE_DESC& rDesc, const DXGIDevice& dxgiDevice) noexcept {

	//ルートシグネチャーのシリアライズ
	Microsoft::WRL::ComPtr<ID3D10Blob> rootBlob{};
	Microsoft::WRL::ComPtr<ID3D10Blob> error{};
	if (FAILED(D3D12SerializeRootSignature(&rDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootBlob, &error))) {
		if (error) {
			const char* msg = static_cast<const char*>(error->GetBufferPointer());
			OutputDebugStringA(msg);
			OutputDebugStringA("\n");
		}
		assert(false && "ルートシグネチャーのシリアライズ_失敗");
		return false;
	}
	//ルートシグネチャーの作成
	if (FAILED(dxgiDevice.GetDevice()->CreateRootSignature(0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature)))) {
		assert(false && "ルートシグネチャー作成_失敗");
		return false;
	}
	return true;
}

//ルートシグネチャーの取得
//ルートシグネチャーのポインター
[[nodiscard]] ID3D12RootSignature* RootSignature::Get() const noexcept {
	if (!rootSignature) {
		assert(false && "ルートシグネチャー配布_失敗");
		return nullptr;
	}
	return rootSignature.Get();
}