#include "Polygon2D.h"

#include "Layout.h"

#include <cassert>


[[nodiscard]] bool Polygon2D::CreateVertex(const DXGIDevice& dxgiDevice) noexcept {

    Poly::Vertex vertices[]{
        {{-0.4f, -0.7f,  0.0}},
        {{-0.4f,  0.7f,  0.0}},
        {{ 0.4f, -0.7f,  0.0}},
        {{ 0.4f,  0.7f,  0.0}},
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
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&vertexBuff)))) {
        assert(false && "頂点バッファー作成_失敗");
        return false;
    }
    //マップ
    void* pMapData{};
    if (FAILED(vertexBuff->Map(0, nullptr, reinterpret_cast<void**>(&pMapData)))) {
        assert(false && "マップ_失敗");
        return false;
    }
    std::memcpy(pMapData, &vertices, vertexSize);
    vertexBuff->Unmap(0, nullptr);

    vertexBufferView.BufferLocation = vertexBuff->GetGPUVirtualAddress();
    vertexBufferView.SizeInBytes = static_cast<UINT>(vertexSize);
    vertexBufferView.StrideInBytes = sizeof(vertices[0]);

    return true;
}

[[nodiscard]] bool Polygon2D::CreateIndex(const DXGIDevice& dxgiDevice) noexcept {
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
    if (FAILED(dxgiDevice.GetDevice()->CreateCommittedResource(&hProp, D3D12_HEAP_FLAG_NONE, &rDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&indexBuff)))) {
        assert(false && "頂点バッファー作成_失敗");
        return false;
    }
    //マップ
    void* pMapData{};
    if (FAILED(indexBuff->Map(0, nullptr, reinterpret_cast<void**>(&pMapData)))) {
        assert(false && "マップ_失敗");
        return false;
    }
    std::memcpy(pMapData, &indices, indexSize);
    vertexBuff->Unmap(0, nullptr);

    indexBufferView.BufferLocation = indexBuff->GetGPUVirtualAddress();
    indexBufferView.Format = DXGI_FORMAT_R16_UINT;
    indexBufferView.SizeInBytes = static_cast<UINT>(indexSize);

    return true;
}

[[nodiscard]] bool Polygon2D::Create(const DXGIDevice& dxgiDevice) noexcept {

    if (!CreateVertex(dxgiDevice)) {
        assert(false && "頂点バッファー作成_失敗");
        return false;
    }

    if (!CreateIndex(dxgiDevice)) {
        assert(false && "指数バッファー作成_失敗");
        return false;
    }
  
    return true;
}

void Polygon2D::Draw(const CommandList& list) noexcept {
    list.Get()->IASetVertexBuffers(0, 1, &vertexBufferView);
    list.Get()->IASetIndexBuffer(&indexBufferView);

    list.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //list.Get()->DrawInstanced(6, 1, 0, 0);
    list.Get()->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
}