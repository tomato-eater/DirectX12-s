#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>

#include "DXGIDevice.h"

//ヒープ管理クラス　シングルトン
class HeapManager final {
	HeapManager() = default;
	~HeapManager() = default;

	//ヒープ制御クラス
	class Heap final {
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap{};	//ヒープ
		std::vector<bool> free{};	//空きヒープ

	public:
		Heap() = default;
		~Heap() = default;

		//ヒープ作成
		//デバイスのポインター　ヒープ属性　ヒープ数　シェーダーからの参照を可能にするか
		//作成成功時、true
		[[nodiscard]] bool Set(ID3D12Device* device, const D3D12_DESCRIPTOR_HEAP_TYPE type, const UINT num, const bool shaderVisible) noexcept;

		//ヒープの取得
		//ヒープのポインター
		[[nodiscard]] ID3D12DescriptorHeap* GetHeap() const noexcept;

		//空きがあるヒープ番号を取得
		//ヒープ番号
		[[nodiscard]] std::optional<UINT> GetNum() noexcept;

		//ヒープを解放
		//解放したい番号
		void Release(UINT num) noexcept;
	};

	std::unordered_map<UINT, std::unique_ptr<Heap>> map{};	//ヒープマップ

	//ヒープ作成
	//デバイスのポインター　ヒープ属性　ヒープ数　シェーダーからの参照を可能にするか
	//作成成功時、true
	[[nodiscard]] bool Set(ID3D12Device* device, const D3D12_DESCRIPTOR_HEAP_TYPE type, const UINT num, const bool shaderVisible) noexcept;

public:
	HeapManager(const HeapManager&) = default;
	HeapManager& operator=(const HeapManager&) = default;
	HeapManager(HeapManager&&) = default;
	HeapManager& operator=(HeapManager&&) = default;

	//インスタンス取得
	//インスタンス参照
	static HeapManager& Ins() {
		static HeapManager instance;
		return instance;
	}

	//必要なヒープ作成
	//DXGIデバイス参照
	//全て作成成功時、true
	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice) noexcept;

	//ヒープの取得
	//取得したいヒープの属性
	//ヒープのポインター
	[[nodiscard]] ID3D12DescriptorHeap* GetHeap(const D3D12_DESCRIPTOR_HEAP_TYPE type) const noexcept;

	//ヒープ番号取得
	//取得したいヒープの属性
	//ヒープの番号
	[[nodiscard]] std::optional<UINT> GetNum(const D3D12_DESCRIPTOR_HEAP_TYPE type) const noexcept;

	//ヒープを解放
	//解放したいヒープ属性　解放したい番号
	void Release(const D3D12_DESCRIPTOR_HEAP_TYPE type, UINT num) const noexcept;
};