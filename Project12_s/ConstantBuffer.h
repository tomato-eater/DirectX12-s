#pragma once

#include "Heap.h"
#include <DirectXMath.h>

#include <cassert>

//カメラ
struct CameraData {
	DirectX::XMMATRIX dView;
	DirectX::XMMATRIX dProjection;
};

//オブジェクト
struct ObjectData
{
	DirectX::XMMATRIX dWorld;
};

//定数バッファー管理クラス
template <class T>
struct ConstantBuffer final{
	ConstantBuffer() = default;
	~ConstantBuffer() = default;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer{};	//定数バッファー
	UINT heapNum{};	//ヒープ番号

	T* pMapData{};	//マップ


	//定数バッファー作成
	//DXGIデバイス参照
	//作成成功時、true
	[[nodiscard]] bool CreateConstance(const DXGIDevice& dxgiDevice, UINT dataSize) noexcept {
		//サイズをアライメント
		const auto size = (dataSize + 255) & ~255;
		//ヒープ設定
		D3D12_HEAP_PROPERTIES hProp{};
		hProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		hProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		hProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		hProp.CreationNodeMask = 0;
		hProp.VisibleNodeMask = 0;
		//リソース設定
		D3D12_RESOURCE_DESC rDesc{};
		rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		rDesc.Width = size;
		rDesc.Height = 1;
		rDesc.DepthOrArraySize = 1;
		rDesc.MipLevels = 1;
		rDesc.Format = DXGI_FORMAT_UNKNOWN;
		rDesc.SampleDesc = { 1, 0 };
		rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		//バッファー作成
		if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuffer)))) {
			assert(false && "定数バッファー作成_失敗");
			return false;
		}
		if (FAILED(constBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pMapData)))) {
			assert(false && "定数バッファーマップ_失敗");
			return false;
		}

		// コンスタントバッファビューの設定
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
		cbvDesc.BufferLocation = constBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = size;
		//ヒープと紐づけ
		const auto heapNumOp = HeapManager::Ins().GetNum(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		if (!heapNumOp.has_value()) {
			assert(false && "CSUヒープ確保_失敗");
			return false;
		}
		heapNum = heapNumOp.value();
		auto handle = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += heapNum * dxgiDevice.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		dxgiDevice.GetDevice()->CreateConstantBufferView(&cbvDesc, handle);

		return true;
	}

	//マップ
	//更新データ
	template <class T>
	void UpdateConst(const T& data) noexcept {
		if (pMapData)
			*pMapData = data;
	}

	//ヒープ番号取得
	//ヒープ番号
	[[nodiscard]] UINT GetNum() const noexcept {
		return heapNum;
	}
};