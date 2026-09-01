#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

//DirectX Graphics Infrastructure (DXGI) デバイス管理クラス
class DXGIDevice final {
	Microsoft::WRL::ComPtr<IDXGIFactory6> factory{};	//DXGIファクトリー
	Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter{};	//DXGIアダプター
	Microsoft::WRL::ComPtr<ID3D12Device> device{};		//D3D12デバイス

	//DXGIファクトリーの作成
	//作成成功時、true
	[[nodiscard]] bool CreateFactory() noexcept;

	//DXGIアダプターの作成
	//作成成功時、true
	[[nodiscard]] bool CreateAdapter() noexcept;

	//D3D12デバイスの作成
	//作成成功時、true
	[[nodiscard]] bool CreateDevice() noexcept;

public:
	DXGIDevice() = default;
	~DXGIDevice() = default;

	//ファクトリー、アダプター、デバイスの作成
	//全て作成成功時、true
	[[nodiscard]] bool Create() noexcept;

	//DXGIファクトリー取得
	//DXGIファクトリーのポインター
	[[nodiscard]] IDXGIFactory6* GetFactory() const noexcept;

	//DXGIアダプターの取得
	//DXGIアダプターのポインター
	[[nodiscard]] IDXGIAdapter4* GetAdapter() const noexcept;

	//D3D12デバイスの取得
	//D3D12デバイスのポインター
	[[nodiscard]] ID3D12Device* GetDevice() const noexcept;
};