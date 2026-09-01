#pragma once

#include "DXGIDevice.h"
#include "../Shaders/Shader.h"
#include "../Objects/Layout.h"
#include "RootSignature.h"

//パイプラインステート管理クラス
class PipeLineState {
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipeLineState{};	//パイプラインステート

public:
	PipeLineState() = default;
	~PipeLineState() = default;

	//パイプラインステート作成
	//頂点シェーダー参照　ピクセルシェーダー参照　インプットレイアウト　DXGIデバイス参照　ルートシグネチャー参照
	//作成成功時、true
	[[nodiscard]] bool Create(const Shader& shader, const Element input, const DXGIDevice& dxgiDevice, const RootSignature& rootSig) noexcept;

	//パイプラインステートの取得
	//パイプラインステートのポインター
	[[nodiscard]] ID3D12PipelineState* Get() const noexcept;
};

