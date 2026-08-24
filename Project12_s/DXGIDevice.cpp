#include "DXGIDevice.h"

#include <cassert>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

//DXGIファクトリーの作成
//作成成功時、true
[[nodiscard]] bool DXGIDevice::CreateFactory() noexcept {
	UINT factoryFlags = 0;

# if defined(_DEBUG)
	//DXGIデバッグレイヤーを有効化
	factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
# endif

	if (CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory)) != S_OK) {
		return false;
	}

	return true;
}

//DXGIアダプターの作成
//作成成功時、true
[[nodiscard]] bool DXGIDevice::CreateAdapter() noexcept {
	Microsoft::WRL::ComPtr<IDXGIAdapter4> ada{};
	//アダプターの列挙
	for (UINT i = 0; factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_UNSPECIFIED, IID_PPV_ARGS(&ada)) != DXGI_ERROR_NOT_FOUND; i++) {
		DXGI_ADAPTER_DESC3 desc{};
		ada->GetDesc3(&desc);

		//ソフトウェアアダプターはスキップ
		if (desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE) {
			continue;
		}

		//D3D12 が使用できないアダプターはスキップ
		if (FAILED(D3D12CreateDevice(ada.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
			continue;
		}

		//アダプターを保存
		if (SUCCEEDED(ada.As(&adapter))) {
			return true;
		}
	}
	assert(false && "DXGI-アダプター作成_失敗");
	return false;
}

//D3D12 デバイスの作成に使用するレベル
D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_2,
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0
};

//デバイスの作成
//作成成功時、true
[[nodiscard]] bool DXGIDevice::CreateDevice() noexcept {

	//levelsの順に確認
	for (auto& level : levels) {
		//デバイスの作成
		if (FAILED(D3D12CreateDevice(adapter.Get(), level, IID_PPV_ARGS(&device)))) {
			continue;
		}
		return true;
	}

	assert(false && "デバイス作成_失敗");
	return false;
}

//ファクトリー、アダプター、デバイスの作成
//全て作成成功時、true
[[nodiscard]] bool DXGIDevice::Create() noexcept {
#if _DEBUG  //デバッグレイヤーの有効化
	Microsoft::WRL::ComPtr<ID3D12Debug> debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
		debug->EnableDebugLayer();
	}
#endif

	//DXGIファクトリーの作成
	if (!CreateFactory()) {
		assert(false && "DXGIファクトリー作成_失敗");
		return false;
	}

	//DXGIアダプターの作成
	if (!CreateAdapter()) {
		assert(false && "DXGIアダプター作成_失敗");
		return false;
	}

	//D3D12デバイスの作成
	if (!CreateDevice()) {
		assert(false && "D3D12デバイス作成_失敗");
		return false;
	}

	return true;
}

//DXGIファクトリー取得
//DXGIファクトリーのポインター
[[nodiscard]] IDXGIFactory6* DXGIDevice::GetFactory() const noexcept {
	if (!factory) {
		assert(false && "DXGIファクトリー配布_失敗");
		return nullptr;
	}
	return factory.Get();
}

//DXGIアダプターの取得
//DXGIアダプターのポインター
[[nodiscard]] IDXGIAdapter4* DXGIDevice::GetAdapter() const noexcept {
	return adapter.Get();
}

//D3D12デバイスの取得
//D3D12デバイスのポインター
[[nodiscard]] ID3D12Device* DXGIDevice::GetDevice() const noexcept {
	return device.Get();
}