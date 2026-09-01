#pragma once

#include "DXGIDevice.h"

//二次元オブジェクト設定
struct Poly2D {
	D3D12_DESCRIPTOR_RANGE r{};		//ディスクリプタテーブル
	D3D12_ROOT_PARAMETER rParameter{};	//ルートパラメーター
	D3D12_STATIC_SAMPLER_DESC sDesc{};	//サンプラーの設定
	D3D12_ROOT_SIGNATURE_DESC rDesc{};	//ルートシグネチャーの設定
	Poly2D() {
		//ディスクリプタテーブル
		r.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		r.NumDescriptors = 1;
		r.BaseShaderRegister = 0;
		r.RegisterSpace = 0;
		r.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//ルートパラメーター
		rParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rParameter.DescriptorTable = { 1, &r};
		rParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		//サンプラーの設定
		sDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sDesc.MipLODBias = 0;
		sDesc.MaxAnisotropy = 0;
		sDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sDesc.MinLOD = 0.0f;
		sDesc.MaxLOD = D3D12_FLOAT32_MAX;
		sDesc.ShaderRegister = 0;
		sDesc.RegisterSpace = 0;
		sDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		//ルートシグネチャーの設定
		rDesc.NumParameters = 1;
		rDesc.pParameters = &rParameter;
		rDesc.NumStaticSamplers = 1;
		rDesc.pStaticSamplers = &sDesc;
		rDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}
};

//三次元オブジェクト設定
struct Model3D {
	D3D12_DESCRIPTOR_RANGE r[2]{};		//ディスクリプタテーブル
	D3D12_ROOT_PARAMETER rParameter[3]{};	//ルートパラメーター
	D3D12_STATIC_SAMPLER_DESC sDesc{};	//サンプラーの設定
	D3D12_ROOT_SIGNATURE_DESC rDesc{};	//ルートシグネチャーの設定
	Model3D() {
		//ディスクリプタテーブル　定数		b1
		r[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		r[0].NumDescriptors = 1;
		r[0].BaseShaderRegister = 1;	//(?1)になる
		r[0].RegisterSpace = 0;
		r[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//ディスクリプタテーブル　テクスチャ―	t0
		r[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		r[1].NumDescriptors = 1;
		r[1].BaseShaderRegister = 0;	//(?0)になる
		r[1].RegisterSpace = 0;
		r[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//ルートパラメーター		b0
		rParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rParameter[0].Descriptor = { 0, 0 };
		rParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		//ルートパラメーター
		rParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rParameter[1].DescriptorTable = { 1, &r[0] };
		rParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		//ルートパラメーター
		rParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rParameter[2].DescriptorTable = {1, &r[1]};
		rParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		//サンプラーの設定		s0
		sDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sDesc.MipLODBias = 0;
		sDesc.MaxAnisotropy = 1;	//斜めからのテクスチャをきれいに描画するか	1, 2, 4, 8, 16
		sDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sDesc.MinLOD = 0.0f;
		sDesc.MaxLOD = D3D12_FLOAT32_MAX;
		sDesc.ShaderRegister = 0;
		sDesc.RegisterSpace = 0;
		sDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		//ルートシグネチャーの設定
		rDesc.NumParameters = _countof(rParameter);
		rDesc.pParameters = rParameter;
		rDesc.NumStaticSamplers = 1;
		rDesc.pStaticSamplers = &sDesc;
		rDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}
};

//ルートシグネチャー管理クラス
class RootSignature final {
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature{};	//ルートシグネチャー

	//ルートシグネチャー作成
	//ルートシグネチャーの設定　DXGIデバイス参照
	//作成成功時、true
	[[nodiscard]] bool Create(const D3D12_ROOT_SIGNATURE_DESC& rDesc, const DXGIDevice& dxgiDevice) noexcept;

public:
	RootSignature() = default;
	~RootSignature() = default;

	//ルートシグネチャー作成
	//ルートシグネチャーの設定　DXGIデバイス参照
	//作成成功時、true
	template <class T>
	[[nodiscard]] bool Create(T temp, const DXGIDevice& dxgiDevice) noexcept {
		return Create(temp.rDesc, dxgiDevice);
	}

	//ルートシグネチャーの取得
	//ルートシグネチャーのポインター
	[[nodiscard]] ID3D12RootSignature* Get() const noexcept;
};

