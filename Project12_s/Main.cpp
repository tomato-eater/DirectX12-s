#include "Main.h"

#include "Heap.h"

#include <cassert>


//初期化
[[nodiscard]] bool Main::Initialize(const HINSTANCE hInstance) noexcept {
	//ウィンドウ生成
	if (!window.Create(hInstance, L"DirectX-S", { 1200, 900 })) {
		assert(false && "ウィンドウ生成_失敗");
		return false;
	}
	//デバイス生成
	if (!dxgiDevice.Create()) {
		assert(false && "デバイス生成_失敗");
		return false;
	}
	//コマンド_キュー_アロケーター リスト_イベント生成
	if (!comm_fence.Create(dxgiDevice, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
		assert(false && "コマンド_キュー_アロケーター_リスト生成_失敗");
		return false;
	}
	//ヒープ作成
	if (!HeapManager::Ins().Create(dxgiDevice)) {
		assert(false && "ヒープ作成_失敗");
		return false;
	}
	//スワップチェイン_レンダ―ターゲット(バックバッファ)作成
	if (!swap_target.Create(window, dxgiDevice, comm_fence)) {
		assert(false && "スワップチェイン_レンダ―ターゲット(バックバッファ)作成_失敗");
		return false;
	}

	if (!shader.Create(L"PolyShader.hlsl")) {
		assert(false && "シェーダー取得_失敗");
		return false;
	}
	if (!rootSig.Create(Poly2D(), dxgiDevice)) {
		assert(false && "ルートシグネチャー作成_失敗");
		return false;
	}
	if (!pipeLine.Create(shader, Poly::Layout(), dxgiDevice, rootSig)) {
		assert(false && "パイプラインステート作成_失敗");
		return false;
	}

	if (!poly.Create(dxgiDevice)) {
		assert(false && "ポリゴン作成_失敗");
		return false;
	}

	return true;
}


//ループ処理
void Main::Loop() noexcept {
	MSG msg{};
	while (window.messageLoop()) {
		//座標等変更

		//バックバッファインデックス取得
		const auto backBuffIdx = swap_target.GetSwap().Get()->GetCurrentBackBufferIndex();
		//アロケーター_リストをリセット
		comm_fence.Reset(backBuffIdx);

		//レンダ―ターゲットの変更　表示用から描画用
		auto reBa = swap_target.ReBarrier(backBuffIdx, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		comm_fence.List().Get()->ResourceBarrier(1, &reBa);

		//ハンドルの設定
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandles[] = { swap_target.GetHandleCPU(backBuffIdx, dxgiDevice) };
		auto depthHandle = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV)->GetCPUDescriptorHandleForHeapStart();
		//depthHandle.ptr += depthBuffer.HeapNum() * dxgiDevice.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		comm_fence.List().Get()->OMSetRenderTargets(1, cpuHandles, true, nullptr);
		//バックバッファクリア
		float clearColor[] = { 0.0f, 0.0f, 0.5f, 1.0f };
		comm_fence.List().Get()->ClearRenderTargetView(cpuHandles[0], clearColor, 0, nullptr);

		//ルートシグネチャー設定
		comm_fence.List().Get()->SetGraphicsRootSignature(rootSig.Get());
		//パイプライン設定
		comm_fence.List().Get()->SetPipelineState(pipeLine.Get());

		// ビューポートとシザー矩形の設定
		comm_fence.ViewportRect(window.GetSize());

		//ヒープの設定
		const auto pHeap = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		comm_fence.List().Get()->SetDescriptorHeaps(1, &pHeap);

		//Map -> setGra -> draw


		//ルートシグネチャーとヒープの紐づけ　主にルートパラメータに着目
		poly.Draw(comm_fence.List());



		//テクスチャー描画



		//レンダ―ターゲットの変更	描画用から表示用
		reBa = swap_target.ReBarrier(backBuffIdx, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		comm_fence.List().Get()->ResourceBarrier(1, &reBa);
		//バッファー更新
		comm_fence.Execute();
		comm_fence.Present(swap_target.GetSwap(), backBuffIdx);

	}
}


//エントリー
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	Main op{};

	if (op.Initialize(hInstance)) {
		op.Loop();
	}
	else {
		assert(false && "初期化_失敗");
	}

	return 0;
}