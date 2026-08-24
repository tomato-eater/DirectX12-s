#pragma once

#include <d3dcompiler.h>
#include <wrl/client.h>

//シェーダー管理クラス
class Shader {
	Microsoft::WRL::ComPtr<ID3DBlob> vs{};	//シェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> ps{};	//シェーダーオブジェクト

public:
	Shader() = default;
	~Shader() = default;

	//シェーダーオブジェクト作成
	//シェーダーパス　関数名　シェーダーモデル
	//作成成功時、true
	[[nodiscard]] bool Create(const wchar_t* path) noexcept;

	//シェーダーオブジェクトの取得
	//シェーダーオブジェクトのポインター
	[[nodiscard]] ID3DBlob* GetVS() const noexcept;

	//シェーダーオブジェクトの取得
	//シェーダーオブジェクトのポインター
	[[nodiscard]] ID3DBlob* GetPS() const noexcept;
};

