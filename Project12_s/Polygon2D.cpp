#include "Polygon2D.h"

#include "Layout.h"
#include "Heap.h"

#include <DirectXTex.h>

#include <cassert>

#pragma comment(lib, "DirectXTex.lib")
/*
頂点バッファー　遅い方
    Model2D::Vertex vertices[]{
        {{-0.4f, -0.7f,  0.0}, {0.0f, 1.0f}},
        {{-0.4f,  0.7f,  0.0}, {0.0f, 0.0f}},
        {{ 0.4f, -0.7f,  0.0}, {1.0f, 1.0f}},
        {{ 0.4f,  0.7f,  0.0}, {1.0f, 0.0f}},
    };
    const auto vertexSize = static_cast<UINT64>(sizeof(vertices));
    //ヒープの設定
    D3D12_HEAP_PROPERTIES hProp{};
    hProp.Type = D3D12_HEAP_TYPE_GPU_UPLOAD;
    hProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    hProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    hProp.CreationNodeMask = 0;
    hProp.VisibleNodeMask = 0;
    //リソースの設定
    D3D12_RESOURCE_DESC rDesc{};
    rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    rDesc.Width = vertexSize;
    rDesc.Height = 1;
    rDesc.DepthOrArraySize = 1;
    rDesc.MipLevels = 1;
    rDesc.Format = DXGI_FORMAT_UNKNOWN;
    rDesc.SampleDesc = { 1, 0 };
    rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    //頂点バッファー作成
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&vertexBuffer)))) {
        assert(false && "頂点バッファー作成_失敗");
        return false;
    }
    //マップ
    void* pMapData{};
    if (FAILED(vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pMapData)))) {
        assert(false && "マップ_失敗");
        return false;
    }
    std::memcpy(pMapData, &vertices, vertexSize);
    vertexBuffer->Unmap(0, nullptr);

    vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
    vertexBufferView.SizeInBytes = static_cast<UINT>(vertexSize);
    vertexBufferView.StrideInBytes = sizeof(vertices[0]);

指数バッファー　遅い方
    unsigned short indices[] = {
      0, 1, 2, 2, 1, 3
    };
    indexCount = _countof(indices);
    const auto indexSize = static_cast<UINT64>(sizeof(indices));
    //ヒープの設定
    D3D12_HEAP_PROPERTIES hProp{};
    hProp.Type = D3D12_HEAP_TYPE_GPU_UPLOAD;
    hProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    hProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    hProp.CreationNodeMask = 0;
    hProp.VisibleNodeMask = 0;
    //リソースの設定
    D3D12_RESOURCE_DESC rDesc{};
    rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    rDesc.Width = indexSize;
    rDesc.Height = 1;
    rDesc.DepthOrArraySize = 1;
    rDesc.MipLevels = 1;
    rDesc.Format = DXGI_FORMAT_UNKNOWN;
    rDesc.SampleDesc = { 1, 0 };
    rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    //頂点バッファー作成
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&indexBuffer)))) {
        assert(false && "頂点バッファー作成_失敗");
        return false;
    }
    //マップ
    void* pMapData{};
    if (FAILED(indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pMapData)))) {
        assert(false && "マップ_失敗");
        return false;
    }
    std::memcpy(pMapData, &indices, indexSize);
    indexBuffer->Unmap(0, nullptr);

    indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
    indexBufferView.Format = DXGI_FORMAT_R16_UINT;
    indexBufferView.SizeInBytes = static_cast<UINT>(indexSize);

テクスチャ―　遅い方
    DirectX::TexMetadata metadata{};		//テクスチャーデータ
    DirectX::ScratchImage scratchImage{};	//画像データ管理
    //画像読み込み
    if (FAILED(DirectX::LoadFromWICFile(L"テクスチャの位置", DirectX::WIC_FLAGS_NONE, &metadata, scratchImage))) {
        assert(false && "テクスチャー読み込み_失敗");
        return false;
    }
    const auto image = scratchImage.GetImage(0, 0, 0);
    //ヒープの設定
    D3D12_HEAP_PROPERTIES hProp{};
    hProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    hProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    hProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
    hProp.CreationNodeMask = 0;
    hProp.VisibleNodeMask = 0;
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
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&textureBuffer)))) {
        assert(false && "頂点バッファー作成_失敗");
        return false;
    }
    if (FAILED(textureBuffer->WriteToSubresource(0, nullptr, image->pixels, image->rowPitch, image->slicePitch))) {
        assert(false && "テクスチャ―転送_失敗");
        return false;
    }

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = metadata.format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    auto handle = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->GetCPUDescriptorHandleForHeapStart();
    const auto heapNumOp = HeapManager::Ins().GetNum(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    if (!heapNumOp.has_value()) {
        assert(false && "ヒープ番号取得_失敗");
        return false;
    }
    heapNum.emplace_back(heapNumOp.value());
    handle.ptr += heapNumOp.value() * dxgiDevice.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    dxgiDevice.GetDevice()->CreateShaderResourceView(textureBuffer.Get(), &srvDesc, handle);
*/

D3D12_HEAP_PROPERTIES SetHeapProp(const D3D12_HEAP_TYPE type) {
    D3D12_HEAP_PROPERTIES prop{};
    prop.Type = type;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask = 0;
    prop.VisibleNodeMask = 0;
    return prop;
}

//頂点_指数_バッファー作成
//DXGIデバイス参照　コマンドセット_フェンス参照
//作成成功時、true
[[nodiscard]] bool Polygon2D::Create(const DXGIDevice& dxgiDevice, Comm_Fence& comm_fence) noexcept {
    //コマンドリストリセット
    comm_fence.Reset(0);
//頂点バッファー作成
    Structure:: Model2D::Vertex vertices[]{
       {{-1.0f, -1.0f,  0.0}, {0.0f, 1.0f}},
       {{-1.0f,  1.0f,  0.0}, {0.0f, 0.0f}},
       {{ 1.0f, -1.0f,  0.0}, {1.0f, 1.0f}},
       {{ 1.0f,  1.0f,  0.0}, {1.0f, 0.0f}},
    };
    const auto vertexSize = static_cast<UINT64>(sizeof(vertices));
    //ヒープの設定
    auto hVertexProp = SetHeapProp(D3D12_HEAP_TYPE_DEFAULT);
    //リソースの設定
    D3D12_RESOURCE_DESC rDesc{};
    rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    rDesc.Width = vertexSize;
    rDesc.Height = 1;
    rDesc.DepthOrArraySize = 1;
    rDesc.MipLevels = 1;
    rDesc.Format = DXGI_FORMAT_UNKNOWN;
    rDesc.SampleDesc = { 1, 0 };
    rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    //頂点バッファー作成
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&hVertexProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&vertexBuffer)))) {
        assert(false && "頂点バッファー作成_失敗");
        return false;
    }
    //ヒープの設定
    auto upVertexProp = SetHeapProp(D3D12_HEAP_TYPE_UPLOAD);
    //更新用バッファー作成
    Microsoft::WRL::ComPtr<ID3D12Resource> upVertexBuffer{};
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&upVertexProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&upVertexBuffer)))) {
        assert(false && "更新用頂点バッファー作成_失敗");
        return false;
    }
    //マップ
    void* pMapVertex{};
    if (FAILED(upVertexBuffer->Map(0, nullptr, &pMapVertex))) {
        assert(false && "マップ_失敗");
        return false;
    }
    std::memcpy(pMapVertex, vertices, vertexSize);
    upVertexBuffer->Unmap(0, nullptr);
    //コピー
    comm_fence.List().ResourceBarrier(vertexBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
    comm_fence.List().Get()->CopyBufferRegion(vertexBuffer.Get(), 0, upVertexBuffer.Get(), 0, vertexSize);
    comm_fence.List().ResourceBarrier(vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
    //バッファービュー
    vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
    vertexBufferView.SizeInBytes = static_cast<UINT>(vertexSize);
    vertexBufferView.StrideInBytes = sizeof(vertices[0]);

//指数バッファー作成
    unsigned short indices[] = {
      0, 1, 2, 2, 1, 3
    };
    indexCount = _countof(indices);
    const auto indexSize = static_cast<UINT64>(sizeof(indices));
    //ヒープの設定
    auto hIndexProp = SetHeapProp(D3D12_HEAP_TYPE_DEFAULT);
    //リソースの設定
    rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    rDesc.Width = indexSize;
    rDesc.Height = 1;
    rDesc.DepthOrArraySize = 1;
    rDesc.MipLevels = 1;
    rDesc.Format = DXGI_FORMAT_UNKNOWN;
    rDesc.SampleDesc = { 1, 0 };
    rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    //指数バッファー作成
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&hIndexProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&indexBuffer)))) {
        assert(false && "指数バッファー作成_失敗");
        return false;
    }
    //ヒープの設定
    auto upIndexProp = SetHeapProp(D3D12_HEAP_TYPE_UPLOAD);
    //更新用バッファー作成
    Microsoft::WRL::ComPtr<ID3D12Resource> upIndexBuffer{};
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&upIndexProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&upIndexBuffer)))) {
        assert(false && "更新用指数バッファー作成_失敗");
        return false;
    }
    //マップ
    void* pMapIndex{};
    if (FAILED(upIndexBuffer->Map(0, nullptr, &pMapIndex))) {
        assert(false && "マップ_失敗");
        return false;
    }
    std::memcpy(pMapIndex, indices, indexSize);
    upIndexBuffer->Unmap(0, nullptr);
    //コピー
    comm_fence.List().ResourceBarrier(indexBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
    comm_fence.List().Get()->CopyBufferRegion(indexBuffer.Get(), 0, upIndexBuffer.Get(), 0, indexSize);
    comm_fence.List().ResourceBarrier(indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
    //バッファービュー
    indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
    indexBufferView.SizeInBytes = static_cast<UINT>(indexSize);
    indexBufferView.Format = DXGI_FORMAT_R16_UINT;

//テクスチャ―バッファー作成
    DirectX::TexMetadata metadata{};		//テクスチャーデータ
    DirectX::ScratchImage scratchImage{};	//画像データ管理
    //画像読み込み
    if (FAILED(DirectX::LoadFromWICFile(L"Assets/Image/TestImage2.png", DirectX::WIC_FLAGS_NONE, &metadata, scratchImage))) {
        assert(false && "テクスチャー読み込み_失敗");
        return false;
    }
    const auto image = scratchImage.GetImage(0, 0, 0);
    //ヒープの設定
    auto hTexProp = SetHeapProp(D3D12_HEAP_TYPE_DEFAULT);
    //リソースの設定
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
    //アップロード用ヒープの設定
    auto upTexProp = SetHeapProp(D3D12_HEAP_TYPE_UPLOAD);
    //リソースの設定
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
    Microsoft::WRL::ComPtr<ID3D12Resource> upTexBuffer{};
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&upTexProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&upTexBuffer)))) {
        assert(false && "アップロード用バッファー作成_失敗");
        return false;
    }
    //マップ
    BYTE* pMapTex{};
    if (FAILED(upTexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pMapTex)))) {
        assert(false && "マップ_失敗");
        return false;
    }
    const BYTE* pSrcMem = image->pixels;
    for (UINT y = 0; y < numRows; ++y) {
        std::memcpy(pMapTex + (y * layout.Footprint.RowPitch), pSrcMem + (y * image->rowPitch), rowSizeInBytes);
    }
    upTexBuffer->Unmap(0, nullptr);
    //アップロードロケーション
    D3D12_TEXTURE_COPY_LOCATION upLocation{};
    upLocation.pResource = upTexBuffer.Get();
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

    //転送と待機
    comm_fence.Execute();
    comm_fence.WaitFence();
  
    return true;
}

//オブジェクト描画
//DXGIデバイス参照　コマンドリスト参照
void Polygon2D::Draw(const DXGIDevice& dxgiDevice, const CommandList& list) noexcept {


    auto handle = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->GetGPUDescriptorHandleForHeapStart();
    const auto handleSize = dxgiDevice.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    list.Get()->IASetVertexBuffers(0, 1, &vertexBufferView);
    list.Get()->IASetIndexBuffer(&indexBufferView);

    list.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //テクスチャ―との紐づけ
    handle.ptr += heapNum.at(0) * handleSize;
    list.Get()->SetGraphicsRootDescriptorTable(2, handle);

    //list.Get()->DrawInstanced(6, 1, 0, 0);
    list.Get()->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);

}