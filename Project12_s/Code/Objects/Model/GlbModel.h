#pragma once

#include "Temp/Model.h"
#include "../../DirectX/Comm_Fence.h"
#include "../Const/ConstantBuffer.h"

//glbモデル管理クラス
class GlbModel final : public Model{
public:
	GlbModel() = default;
	~GlbModel() = default;

	//モデルバッファー作成
	//ファイルパス　DXGIデバイス参照　コマンドセット＿フェンス参照
	//作成成功時、true
	[[nodiscard]] bool Create(const char* filePath, const DXGIDevice& dxgiDevice, Comm_Fence& comm_fence) noexcept;

	//オブジェクト描画
	//DXGIデバイス参照　コマンドリスト参照
	void Draw(const DXGIDevice& dxgiDevice, const CommandList& list) noexcept;
};

