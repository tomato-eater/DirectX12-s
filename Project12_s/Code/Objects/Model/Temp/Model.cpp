#include "Model.h"

#include <cassert>
#include <DirectXTex.h>

#include "../../../DirectX/Heap.h"

#pragma comment(lib, "DirectXTex.lib")

//ヒープの設定
D3D12_HEAP_PROPERTIES SetHeapProp(const D3D12_HEAP_TYPE type) {
    D3D12_HEAP_PROPERTIES prop{};
    prop.Type = type;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask = 0;
    prop.VisibleNodeMask = 0;
    return prop;
}

//リソースの設定
D3D12_RESOURCE_DESC SetResourceDesc(const UINT64 size) {
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc = { 1, 0 };
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;
    return desc;
}

//バッファー作成
//DXGIデバイス参照　コマンドセット_フェンス参照　データポインター　データサイズ　頂点/指数_バッファー　アップロード用バッファ―
//作成成功時、true
[[nodiscard]] bool Model::CreateBuffer(const DXGIDevice& dxgiDevice, const Comm_Fence& comm_fence, const void* pData, const UINT64 dataSize, Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, Microsoft::WRL::ComPtr<ID3D12Resource>& upBuffer) noexcept {
    auto hProp = SetHeapProp(D3D12_HEAP_TYPE_DEFAULT);
    auto rDesc = SetResourceDesc(dataSize);
    //バッファー作成
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&buffer)))) {
        assert(false && "バッファー作成_失敗");
        return false;
    }

    auto upProp = SetHeapProp(D3D12_HEAP_TYPE_UPLOAD);
    //アップロードバッファー作成
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&upProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&upBuffer)))) {
        assert(false && "アップロードバッファー作成_失敗");
        return false;
    }
    //マップ
    void* pMapData{};
    if (FAILED(upBuffer->Map(0, nullptr, &pMapData))) {
        assert(false && "マップ_失敗");
        return false;
    }
    std::memcpy(pMapData, pData, dataSize);
    upBuffer->Unmap(0, nullptr);
    //コピー
    comm_fence.List().ResourceBarrier(buffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
    comm_fence.List().Get()->CopyBufferRegion(buffer.Get(), 0, upBuffer.Get(), 0, dataSize);
    comm_fence.List().ResourceBarrier(buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

    return true;
}

//テクスチャ―バッファー作成
//DXGIデバイス参照　コマンドセット_フェンス参照　テクスチャーパス　アップロード用バッファ―
//作成成功時、true
[[nodiscard]] bool Model::CreateTexture(const DXGIDevice& dxgiDevice, const Comm_Fence& comm_fence, const wchar_t* path, Microsoft::WRL::ComPtr<ID3D12Resource>& upBuffer) noexcept {
    DirectX::TexMetadata metadata{};		//テクスチャーデータ
    DirectX::ScratchImage scratchImage{};	//画像データ管理
    //画像読み込み
    if (FAILED(DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS_NONE, &metadata, scratchImage))) {
        assert(false && "テクスチャー読み込み_失敗");
        return false;
    }
    const auto image = scratchImage.GetImage(0, 0, 0);
    //ヒープの設定
    auto hTexProp = SetHeapProp(D3D12_HEAP_TYPE_DEFAULT);
    //リソースの設定
    D3D12_RESOURCE_DESC rDesc{};
    rDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
    rDesc.Width = metadata.width;
    rDesc.Height = metadata.height;
    rDesc.DepthOrArraySize = metadata.arraySize;
    rDesc.MipLevels = metadata.mipLevels;
    rDesc.Format = metadata.format;
    rDesc.SampleDesc = { 1, 0 };
    rDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    //テクスチャーバッファー作成
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&hTexProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&textureBuffer)))) {
        assert(false && "頂点バッファー作成_失敗");
        return false;
    }
    //画像情報を取得
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout{};
    UINT numRows = 0;
    UINT64 rowSizeInBytes = 0;
    UINT64 totalBytes = 0;
    dxgiDevice.GetDevice()->GetCopyableFootprints(&rDesc, 0, 1, 0, &layout, &numRows, &rowSizeInBytes, &totalBytes);

    auto upProp = SetHeapProp(D3D12_HEAP_TYPE_UPLOAD);
    rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    rDesc.Width = ((image->rowPitch + 255) & ~255) * image->height;
    rDesc.Height = 1;
    rDesc.DepthOrArraySize = 1;
    rDesc.MipLevels = 1;
    rDesc.Format = DXGI_FORMAT_UNKNOWN;
    rDesc.SampleDesc = { 1, 0 };
    rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    //アップロード用バッファー作成
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&upProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&upBuffer)))) {
        assert(false && "アップロード用バッファー作成_失敗");
        return false;
    }
    //マップ
    BYTE* pMapTex{};
    if (FAILED(upBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pMapTex)))) {
        assert(false && "マップ_失敗");
        return false;
    }
    const BYTE* pSrcMem = image->pixels;
    for (UINT y = 0; y < numRows; ++y) {
        std::memcpy(pMapTex + (y * layout.Footprint.RowPitch), pSrcMem + (y * image->rowPitch), rowSizeInBytes);
    }
    upBuffer->Unmap(0, nullptr);
    //アップロードロケーション
    D3D12_TEXTURE_COPY_LOCATION upLocation{};
    upLocation.pResource = upBuffer.Get();
    upLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    upLocation.PlacedFootprint = layout;
    //テクスチャ―ロケーション
    D3D12_TEXTURE_COPY_LOCATION texLocation{};
    texLocation.pResource = textureBuffer.Get();
    texLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    texLocation.SubresourceIndex = 0;
    //コピー
    comm_fence.List().Get()->CopyTextureRegion(&texLocation, 0, 0, 0, &upLocation, nullptr);
    comm_fence.List().ResourceBarrier(textureBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    //シェーダーとの紐づけ
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = metadata.format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    auto handle = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->GetCPUDescriptorHandleForHeapStart();
    const auto heapNumOp = HeapManager::Ins().GetNum(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    if (!heapNumOp.has_value()) {
        assert(false && "CSUヒープ確保_失敗");
        return false;
    }
    heapNum.emplace_back(heapNumOp.value());
    handle.ptr += heapNumOp.value() * dxgiDevice.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    dxgiDevice.GetDevice()->CreateShaderResourceView(textureBuffer.Get(), &srvDesc, handle);

    return true;
}