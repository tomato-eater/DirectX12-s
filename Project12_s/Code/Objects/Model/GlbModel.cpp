#include "GlbModel.h"

#include <fastgltf/core.hpp>
#include <fastgltf/types.hpp>
#include <filesystem>

#include "../Layout.h"
#include "../../DirectX/Heap.h"

#include <DirectXTex.h>

#include <cassert>

#pragma comment(lib, "DirectXTex.lib")

//モデルバッファー作成
//ファイルパス　DXGIデバイス参照　コマンドセット＿フェンス参照
//作成成功時、true
[[nodiscard]] bool GlbModel::Create(const char* filePath, const DXGIDevice& dxgiDevice, Comm_Fence& comm_fence) noexcept {

	Microsoft::WRL::ComPtr<ID3D12Resource> upVertexBuffer{};
	Microsoft::WRL::ComPtr<ID3D12Resource> upIndexBuffer{};


	const std::filesystem::path path = filePath;
	//ファイル読み込み
	auto data = fastgltf::GltfDataBuffer::FromPath(path);
	if (data.error() != fastgltf::Error::None) {
		assert(false && "ファイル読み込み_失敗");
		return false;
	}
	//バースオプション
	const auto options = fastgltf::Options::LoadExternalBuffers;
	//パースの実行
	fastgltf::Parser parser(fastgltf::Extensions::None);
	auto asset = parser.loadGltfBinary(data.get(), path.parent_path(), options);
	if (asset.error() != fastgltf::Error::None) {
		const int e = static_cast<const int>(asset.error());
		assert(false && "glTFのパース_失敗");
		return false;
	}
	//バッファーの先頭を取得
	if(asset->buffers.empty()) {
		assert(false && "glTFのバッファーが空");
		return false;
	}
	const auto& buffer = asset->buffers[0];

	const uint8_t* bufferData = nullptr;
	size_t bufferSize = 0;

	std::visit(fastgltf::visitor{ [&](const std::vector<std::byte>& bytes) {
		bufferData = reinterpret_cast<const uint8_t*>(bytes.data());
		bufferSize = bytes.size(); },
		[](const auto&) {}
		}, buffer.data);

	return true;
}

//オブジェクト描画
//DXGIデバイス参照　コマンドリスト参照
void GlbModel::Draw(const DXGIDevice& dxgiDevice, const CommandList& list) noexcept {

}