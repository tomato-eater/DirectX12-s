#include "Shader.h"

#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")

//シェーダーオブジェクト作成
//シェーダーパス　関数名　シェーダーモデル
//作成成功時、true
[[nodiscard]] bool Shader::Create(const wchar_t* path) noexcept {
    Microsoft::WRL::ComPtr<ID3DBlob> error{};
    //vertex
    if (FAILED(D3DCompileFromFile(path, nullptr, nullptr, "vs", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vs, &error))) {
        char* p = static_cast<char*>(error->GetBufferPointer());
        assert(false && "シェーダーコンパイル_失敗");
        return false;
    }
    //pixel
    if (FAILED(D3DCompileFromFile(path, nullptr, nullptr, "ps", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &ps, &error))) {
        char* p = static_cast<char*>(error->GetBufferPointer());
        assert(false && "シェーダーコンパイル_失敗");
        return false;
    }


    return true;
}

//シェーダーオブジェクトの取得
//シェーダーオブジェクトのポインター
[[nodiscard]] ID3DBlob* Shader::GetVS() const noexcept {
    return vs.Get();
}

//シェーダーオブジェクトの取得
//シェーダーオブジェクトのポインター
[[nodiscard]] ID3DBlob* Shader::GetPS() const noexcept {
    return ps.Get();
}