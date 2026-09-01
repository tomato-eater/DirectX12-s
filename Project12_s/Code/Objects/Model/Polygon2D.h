#pragma once

#include "Temp/Model.h"
#include "../../DirectX/Comm_Fence.h"
#include "../Const/ConstantBuffer.h"

//二次元オブジェクト管理クラス
class Polygon2D final : public Model{
public:
	Polygon2D() = default;
	~Polygon2D() = default;

	//頂点_指数_バッファー作成
	//DXGIデバイス参照　コマンドセット_フェンス参照　テクスチャ―パス
	//作成成功時、true
	[[nodiscard]] bool Create(const DXGIDevice& dxgiDevice, Comm_Fence& comm_fence, const wchar_t* path) noexcept;

	//オブジェクト描画
	//DXGIデバイス参照　コマンドリスト参照
	void Draw(const DXGIDevice& dxgiDevice, const CommandList& list) noexcept;
};