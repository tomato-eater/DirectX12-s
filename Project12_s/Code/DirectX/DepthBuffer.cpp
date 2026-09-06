#include "DepthBuffer.h"

#include "Heap.h"

#include <cassert>

//深度バッファー作成
//ウィンドウ参照　DXGIデバイス参照
//作成成功時、true
[[nodiscard]] bool DepthBuffer::Create(const Window& window, const DXGIDevice& dxgiDevice) noexcept {
	auto size = window.GetSize();
	//プロパティーの準備
	D3D12_HEAP_PROPERTIES hProp{
		.Type = D3D12_HEAP_TYPE_DEFAULT,
		.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN
	};
	//リソースの準備
	D3D12_RESOURCE_DESC rDesc{
		.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		.Width = size.first,
		.Height = size.second,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.Format = DXGI_FORMAT_D32_FLOAT,
		.SampleDesc = { 1,0 },
		.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
		.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	};
	//バリューの準備
	D3D12_CLEAR_VALUE value{};			
	value.DepthStencil.Depth = 1.0f;
	value.Format = DXGI_FORMAT_D32_FLOAT;
	//デプスバッファ作成
	if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &value, IID_PPV_ARGS(&buffer)))) {
		assert(false && "深度バッファー作成_失敗");
		return false;
	}
	D3D12_DEPTH_STENCIL_VIEW_DESC dsv{ 
		.Format = DXGI_FORMAT_D32_FLOAT,
		.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
		.Flags = D3D12_DSV_FLAG_NONE	
	};

	handle = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV)->GetCPUDescriptorHandleForHeapStart();
	auto heapNumOp = HeapManager::Ins().GetNum(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	if (!heapNumOp.has_value()) {
		assert(false && "DSVヒープ確保_失敗");
		return false;
	}
	heapNum = heapNumOp.value();
	handle.ptr += heapNum * dxgiDevice.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	//深度ビュー作成
	dxgiDevice.GetDevice()->CreateDepthStencilView(buffer.Get(), &dsv, handle);

	return true;
}

//深度ハンドルの取得
//深度ハンドル
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE DepthBuffer::GetHandle() const noexcept {
	if (handle.ptr < 0) {
		assert(false && "深度バッファー_未作成");
		return D3D12_CPU_DESCRIPTOR_HANDLE{};
	}
	return handle;
}