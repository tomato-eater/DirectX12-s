#pragma once

#include "../Window/Window.h"

#include "../DirectX/DXGIDevice.h"
#include "../DirectX/Swap_Target.h"

#include "../DirectX/RootSignature.h"
#include "../DirectX/PipeLineState.h"

#include "../Shaders/Shader.h"

#include "../Objects/Camera/Camera.h"
#include "../Objects/Model/Polygon2D.h"
#include "../Objects/Model/GameObject.h"

#include "../Objects/Model/GlbModel.h"

//メイン管理クラス
class Main final {
	Window window{};				//ウィンドウ管理インスタンス
	DXGIDevice dxgiDevice{};		//DXGI_ファクトリー_アダプターとD3D12デバイス管理インスタンス
	Comm_Fence comm_fence{};		//コマンド_キュー_アロケーター リスト_イベント管理インスタンス
	Swap_Target swap_target{};		////スワップチェイン_レンダ―ターゲット(バックバッファ)管理インスタンス

	Shader shader{};				//VS_PS用シェーダー管理インスタンス
	RootSignature rootSig{};		//ルートシグネチャー管理インスタンス
	PipeLineState pipeLine{};		//パイプラインステート管理インスタンス

	Camera camera{};
	GameObject object{};

	Polygon2D poly{};				//2Dポリゴン管理インスタンス

	GlbModel glbModel{};

public:
	Main() = default;
	~Main() = default;

	//初期化
	[[nodiscard]] bool Initialize(const HINSTANCE hInstance) noexcept;

	//ループ	
	void Loop() noexcept;
};

