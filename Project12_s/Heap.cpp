#include "Heap.h"

#include <cassert>

//ヒープ作成
//デバイスのポインター　ヒープ属性　ヒープ数　シェーダーからの参照を可能にするか
//作成成功時、true
[[nodiscard]] bool HeapManager::Heap::Set(ID3D12Device* device, const D3D12_DESCRIPTOR_HEAP_TYPE type, const UINT num, const bool shaderVisible) noexcept {
    //デバイスの確認
    if (!device) {
        assert(false && "D3D12デバイス取得_失敗");
        return false;
    }
    //ヒープの設定
    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.Type = type;
    desc.NumDescriptors = num;
    desc.NodeMask = 0;
    desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    //ヒープ作成
    if (FAILED(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap)))) {
        assert(false && "ヒープ作成_失敗");
        return false;
    }
    free.resize(num, true);
    return true;
}

//ヒープの取得
//ヒープのポインター
[[nodiscard]] ID3D12DescriptorHeap* HeapManager::Heap::GetHeap() const noexcept {
    if (!heap) {
        assert("ヒープ配布_失敗");
        return nullptr;
    }
    return heap.Get();
}

//空きがあるヒープ番号を取得
//ヒープ番号
[[nodiscard]] std::optional<UINT> HeapManager::Heap::GetNum() noexcept {
    for (int i = 0; i < free.size(); i++) {
        if (free.at(i)) {
            free.at(i) = false;
            return i;
        }
    }
    assert(false && "ヒープ数_超過");
    return std::nullopt;
}

//ヒープを解放
//解放したい番号
void HeapManager::Heap::Release(UINT num) noexcept {
    if (num < 0 || free.size() <= num) {
        assert(false && "解放しようとしたがヒープ範囲外");
        return;
    }
    free.at(num) = true;
}


//ヒープ作成
//デバイスのポインター　ヒープ属性　ヒープ数　シェーダーからの参照を可能にするか
//作成成功時、true
[[nodiscard]] bool HeapManager::Set(ID3D12Device* device, const D3D12_DESCRIPTOR_HEAP_TYPE type, const UINT num, const bool shaderVisible) noexcept {
    //作成済か確認
    if (map.find(type) != map.end()) {
        assert(false && "このディスクリプタヒープは作成済");
        return false;
    }
    //作成
    auto heap = std::make_unique<Heap>();
    if (heap->Set(device, type, num, shaderVisible)) {
        map.emplace(type, std::move(heap));
        return true;
    }
    assert(false && "ディスクリプタヒープ作成_失敗");
    return false;
}

//必要なヒープ作成
//DXGIデバイス参照
//全て作成成功時、true
[[nodiscard]] bool HeapManager::Create(const DXGIDevice& dxgiDevice) noexcept {
    //RTVヒープ作成
    if (!Set(dxgiDevice.GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false)) {
        assert(false && "RTVヒープ作成_失敗");
        return false;
    }
    //CBV_SRV_UAVヒープ作成
    if (!Set(dxgiDevice.GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true)) {
        assert(false && "CBV_SRV_UAVヒープ作成_失敗");
        return false;
    }
    //DSVヒープ作成  デプスバッファーぐらい?
    if (!Set(dxgiDevice.GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false)) {
        assert(false && "DSVヒープ作成_失敗");
        return false;
    }
    return true;
}

//ヒープの取得
//取得したいヒープの属性
//ヒープのポインター
[[nodiscard]] ID3D12DescriptorHeap* HeapManager::GetHeap(const D3D12_DESCRIPTOR_HEAP_TYPE type) const noexcept {
    const auto get = map.find(type);
    //作成してるか確認
    if (get == map.end()) {
        assert(false && "取得しようとしたヒープ_未作成");
        return nullptr;
    }
    return get->second->GetHeap();
}

//ヒープ番号取得
//取得したいヒープの属性
//ヒープの番号
[[nodiscard]] std::optional<UINT> HeapManager::GetNum(const D3D12_DESCRIPTOR_HEAP_TYPE type) const noexcept {
    const auto get = map.find(type);
    //作成してるか確認
    if (get == map.end()) {
        assert(false && "取得しようとしたヒープ_未作成");
        return std::nullopt;
    }
    return get->second->GetNum();
}

//ヒープを解放
//解放したいヒープ属性　解放したい番号
void HeapManager::Release(const D3D12_DESCRIPTOR_HEAP_TYPE type, const UINT num) const noexcept {
    const auto get = map.find(type);
    //作成してるか確認
    if (get == map.end()) {
        assert(false && "解放しようとしたヒープ_未作成");
        return;
    }
    get->second->Release(num);
}