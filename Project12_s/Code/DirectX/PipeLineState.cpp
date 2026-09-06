#include "PipeLineState.h"

#include <cassert>

//ラスタライズステート　表裏面表示など
D3D12_RASTERIZER_DESC Rasterizer()
{
	D3D12_RASTERIZER_DESC rasterDesc{};
	rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;//塗りつぶす
	rasterDesc.CullMode = D3D12_CULL_MODE_NONE;	//表裏面表示判断
	rasterDesc.FrontCounterClockwise = true;	//描画描画順を反時計回りにする
	rasterDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterDesc.DepthClipEnable = true;		//深度方向のクリッピング
	rasterDesc.MultisampleEnable = false;		//アンチエイリアス
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.ForcedSampleCount = 0;
	rasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return rasterDesc;
}

//ブレンド
D3D12_BLEND_DESC Blend(int type) {
	//ブレンドステート
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.BlendEnable = false;
	blendDesc.LogicOpEnable = false;
	blendDesc.SrcBlend = type == 2 ? D3D12_BLEND_SRC_ALPHA : D3D12_BLEND_ONE;
	blendDesc.DestBlend = type == 2 ? D3D12_BLEND_INV_SRC_ALPHA : type == 1 ? D3D12_BLEND_ONE : D3D12_BLEND_ZERO;
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_BLEND_DESC blend{};
	blend.AlphaToCoverageEnable = false;	//アルファ対カバレッジ
	blend.IndependentBlendEnable = false;	//同時レンダーターゲットで独立させるか
	for (auto& target : blend.RenderTarget) {
		target = blendDesc;
	}

	return blend;
}

//デプスステート
D3D12_DEPTH_STENCIL_DESC Depth(bool depth)
{
	D3D12_DEPTH_STENCIL_DESC depthDesc{};
	depthDesc.DepthEnable = depth;
	depthDesc.StencilEnable = false;
	depthDesc.DepthWriteMask = depth ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
	depthDesc.DepthFunc = depth ? D3D12_COMPARISON_FUNC_LESS : D3D12_COMPARISON_FUNC_ALWAYS;

	return depthDesc;
}

//パイプラインステート作成
//頂点シェーダー参照　ピクセルシェーダー参照　インプットレイアウト　DXGIデバイス参照　ルートシグネチャー参照　ブレンドタイプ　深度設定
//作成成功時、true
[[nodiscard]] bool PipeLineState::Create(const Shader& shader, const Element input, const DXGIDevice& dxgiDevice, const RootSignature& rootSig, const int type, const bool depth) noexcept {
	//パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipDesc{};
	pipDesc.pRootSignature = rootSig.Get();
	pipDesc.VS = { shader.GetVS()->GetBufferPointer(), shader.GetVS()->GetBufferSize() };
	pipDesc.PS = { shader.GetPS()->GetBufferPointer(), shader.GetPS()->GetBufferSize() };
	pipDesc.BlendState = Blend(type);
	pipDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	pipDesc.RasterizerState = Rasterizer();
	pipDesc.DepthStencilState = Depth(depth);
	pipDesc.InputLayout = { input.first, static_cast<UINT>(input.second) };
	pipDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	pipDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipDesc.NumRenderTargets = 1;
	pipDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipDesc.SampleDesc = { 1, 0 };
	//パイプラインステート作成
	if (FAILED(dxgiDevice.GetDevice()->CreateGraphicsPipelineState(&pipDesc, IID_PPV_ARGS(&pipeLineState)))) {
		assert(false && "パイプラインステート作成_失敗");
		return false;
	}
	return true;
}

//パイプラインステートの取得
//パイプラインステートのポインター
[[nodiscard]] ID3D12PipelineState* PipeLineState::Get() const noexcept {
	if (!pipeLineState) {
		assert(false && "パイプラインステート配布_失敗");
		return nullptr;
	}
	return pipeLineState.Get();
}