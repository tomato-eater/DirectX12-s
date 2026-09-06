#include "GlbModel.h"

#include <fastgltf/core.hpp>
#include <fastgltf/types.hpp>
#include <filesystem>

#include "../Layout.h"
#include "../../DirectX/Heap.h"

#include <cassert>

#pragma comment(lib, "DirectXTex.lib")

using Microsoft::WRL::ComPtr;

//モデルバッファー作成
//ファイルパス　DXGIデバイス参照　コマンドセット＿フェンス参照
//作成成功時、true
[[nodiscard]] bool GlbModel::Create(const wchar_t* filePath, const DXGIDevice& dxgiDevice, Comm_Fence& comm_fence) noexcept {
	//ファイル読み込み
	const std::filesystem::path path = filePath;
	auto data = fastgltf::GltfDataBuffer::FromPath(filePath);
	if (data.error() != fastgltf::Error::None) {
		assert(false && "ファイル読み込み_失敗");
		return false;
	}
	//パースの実行
	fastgltf::Parser parser(fastgltf::Extensions::None);
	auto asset = parser.loadGltfBinary(data.get(), path.parent_path(), fastgltf::Options::LoadExternalBuffers | fastgltf::Options::LoadExternalImages);
	if (asset.error() != fastgltf::Error::None) {
		const int e = static_cast<const int>(asset.error());
		assert(false && "glTFのパース_失敗");
		return false;
	}
	//バッファーの先頭を取得
	if (asset->buffers.empty()) {
		assert(false && "glTFのバッファーが空");
		return false;
	}
	const auto& modelBuffer = asset->buffers[0];

	const uint8_t* bufferData = nullptr;
	size_t bufferSize = 0;

	std::visit([&](const auto& arg) {
		using T = std::decay_t<decltype(arg)>;
		// 1. GLBの内部バッファー（ByteView）の場合（最も確率が高い）|| 2. メモリ上に確保された配列（Array）の場合
		if constexpr (std::is_same_v<T, fastgltf::sources::ByteView> || std::is_same_v<T, fastgltf::sources::Array>) {
			bufferData = reinterpret_cast<const uint8_t*>(arg.bytes.data());
			bufferSize = arg.bytes.size();
		}
		// 3. ベクター形式（std::vector<std::byte>）の場合
		else if constexpr (std::is_same_v<T, std::vector<std::byte>>) {
			bufferData = reinterpret_cast<const uint8_t*>(arg.data());
			bufferSize = arg.size();
		}
		}, modelBuffer.data);

	if (!bufferData) {
		assert(false && "バッファーデータの取得に失敗");
		return false;
	}

	std::vector<ComPtr<ID3D12Resource>> upBuffer{};
	std::vector<ComPtr<ID3D12Resource>> upTexBuff{};

	comm_fence.Reset(0);

	for (int i = 0; i < asset->bufferViews.size(); i++) {
		const auto& view = asset->bufferViews[i];
		if (view.byteLength == 0) continue;
		//
		const std::byte* dataPtr = std::visit([](const auto& arg) -> const std::byte* {
			using T = std::decay_t<decltype(arg)>;
			// 1. GLBの内部バッファー（ByteView）の場合（最も確率が高い）|| 2. メモリ上に確保された配列（Array）の場合
			if constexpr (std::is_same_v<T, fastgltf::sources::ByteView> || std::is_same_v<T, fastgltf::sources::Array>) {
				return arg.bytes.data();
			}
			// 3. ベクター形式（std::vector<std::byte>）の場合
			else if constexpr (std::is_same_v<T, std::vector<std::byte>>) {
				return arg.data();
			}
			return nullptr;
			}, asset->buffers[view.bufferIndex].data);
		if (!dataPtr) {
			assert(false && "バッファー内のデータが空");
			return false;
		}
		std::span<const std::byte> sourceData(dataPtr + view.byteOffset, view.byteLength);

		auto it = std::ranges::find_if(asset->accessors, [i](const fastgltf::Accessor& acc) { return acc.bufferViewIndex.has_value() && acc.bufferViewIndex.value() == i; });

		//テクスチャー
		if (it == asset->accessors.end()) {
			for (int tex = 0; tex < asset->images.size(); tex++) {
				const uint8_t* compressedDataPtr = nullptr;
				size_t compressedSize = 0;
				if (const auto* pBufferViewIndex = std::get_if<fastgltf::sources::BufferView>(&asset->images[tex].data)) {
					// 1. 対象の bufferView と、それが所属する buffer を取得
					const auto& view = asset->bufferViews[pBufferViewIndex->bufferViewIndex];
					const auto& gltfBuffer = asset->buffers[view.bufferIndex];
					// 2. 頂点の時と同じ手法で、そのバッファーのメモリ先頭ポインタを安全に取得
					const std::byte* bufferStartPtr = std::visit([](const auto& arg) -> const std::byte* {
						using T = std::decay_t<decltype(arg)>;
						if constexpr (std::is_same_v<T, fastgltf::sources::ByteView> || std::is_same_v<T, fastgltf::sources::Array>) {
							return arg.bytes.data();
						}
						else if constexpr (std::is_same_v<T, std::vector<std::byte>>) {
							return arg.data();
						}
						return nullptr;
						}, gltfBuffer.data);

					if (bufferStartPtr) {
						// 3. バッファーの先頭に、ビューのオフセットを足すことで、正確なJPG/PNGの先頭アドレスになる
						compressedDataPtr = reinterpret_cast<const uint8_t*>(bufferStartPtr + view.byteOffset);
						compressedSize = view.byteLength;
					}
				}

				DirectX::TexMetadata metadata{};		//テクスチャーデータ
				DirectX::ScratchImage scratchImage{};	//画像データ管理
				DirectX::WIC_FLAGS wicFlags = DirectX::WIC_FLAGS_DEFAULT_SRGB;
				if (FAILED(DirectX::LoadFromWICMemory(reinterpret_cast<const uint8_t*>(compressedDataPtr), compressedSize, wicFlags, &metadata, scratchImage))) {
					assert(false && "テクスチャー読み込み_失敗");
					return false;
				}

				//テクスチャ―バッファー
				if (!CreateTexture(dxgiDevice, comm_fence, metadata, *scratchImage.GetImage(0, 0, 0), textureBuffer.emplace_back(), upTexBuff.emplace_back(), renMesh.emplace_back().heapNum)) {
					assert(false && "テクスチャ―バッファー作成_失敗");
					return false;
				}
			}
		}
		else {
			const fastgltf::Accessor& accessor = *it;
			//指数バッファー
			if (view.target == fastgltf::BufferTarget::ElementArrayBuffer) {
				if (!CreateBuffer(dxgiDevice, comm_fence, sourceData.data(), sourceData.size_bytes(), buffer.emplace_back(), upBuffer.emplace_back())) {
					assert(false && "指数バッファー作成_失敗");
					return false;
				}
			}
			//頂点バッファー
			else {
				if (!CreateBuffer(dxgiDevice, comm_fence, sourceData.data(), sourceData.size_bytes(), buffer.emplace_back(), upBuffer.emplace_back())) {
					assert(false && "頂点バッファー作成_失敗");
					return false;
				}
			}
		}
	}

	for (int i = 0; i < asset->meshes.size();i++) {
		const auto& mesh = asset->meshes[i];
		for (int j = 0; j < mesh.primitives.size(); j++) {
			if (renMesh.size() <= i + j) {
				renMesh.emplace_back();
			}
			//指数バッファービュー
			if (mesh.primitives[j].indicesAccessor.has_value()) {
				const auto& accessor = asset->accessors[mesh.primitives[j].indicesAccessor.value()];
				if (accessor.bufferViewIndex.has_value()) {
					size_t viewIdx = accessor.bufferViewIndex.value();
					const auto& view = asset->bufferViews[viewIdx];

					renMesh.at(i + j).indexCount = static_cast<UINT>(accessor.count);
					renMesh.at(i + j).indexBufferView = {
						 .BufferLocation = buffer.at(viewIdx)->GetGPUVirtualAddress() + accessor.byteOffset,
						 .SizeInBytes = static_cast<UINT>(view.byteLength),
						 .Format = accessor.componentType == fastgltf::ComponentType::UnsignedShort ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT
					};					
				}
			}
			//頂点バッファービュー
			auto posIt = mesh.primitives[j].findAttribute("POSITION");
			if (posIt != mesh.primitives[j].attributes.end()) {
				const auto& accessor = asset->accessors[posIt->accessorIndex];
				if (accessor.bufferViewIndex.has_value()) {
					size_t viewIdx = accessor.bufferViewIndex.value();
					const auto& view = asset->bufferViews[viewIdx];

					renMesh.at(i + j).vertexBufferView = {
						.BufferLocation = buffer.at(viewIdx)->GetGPUVirtualAddress() + accessor.byteOffset,
						.SizeInBytes = static_cast<UINT>(view.byteLength),
						.StrideInBytes = static_cast<UINT>(view.byteStride.value_or(fastgltf::getElementByteSize(accessor.type, accessor.componentType)))
					};
				}
			}
			// ③ 頂点バッファービュー (TEXCOORD_0 / UV) のマッピング
			auto uvIt = mesh.primitives[j].findAttribute("TEXCOORD_0");
			if (uvIt != mesh.primitives[j].attributes.end()) {
				const auto& accessor = asset->accessors[uvIt->accessorIndex];
				if (accessor.bufferViewIndex.has_value()) {
					size_t viewIdx = accessor.bufferViewIndex.value();
					const auto& view = asset->bufferViews[viewIdx];

					renMesh.at(i + j).texCoordView = {
						.BufferLocation = buffer.at(viewIdx)->GetGPUVirtualAddress() + accessor.byteOffset,
						.SizeInBytes = static_cast<UINT>(view.byteLength),
						.StrideInBytes = static_cast<UINT>(view.byteStride.value_or(fastgltf::getElementByteSize(accessor.type, accessor.componentType)))
					};
				}
			}

		}
	}

	//転送と待機
	comm_fence.Execute();
	comm_fence.WaitFence();

	return true;
}

//オブジェクト描画
//DXGIデバイス参照　コマンドリスト参照
void GlbModel::Draw(const DXGIDevice& dxgiDevice, const CommandList& list) noexcept {
	auto handle = HeapManager::Ins().GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->GetGPUDescriptorHandleForHeapStart();
	const auto handleSize = dxgiDevice.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (const auto& mesh : renMesh) {
		if (mesh.vertexBufferView.SizeInBytes > 0) {
			list.Get()->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);
		}
		if (mesh.texCoordView.SizeInBytes > 0) {
			list.Get()->IASetVertexBuffers(1, 1, &mesh.texCoordView);
		}
		if (mesh.indexBufferView.SizeInBytes > 0) {
			list.Get()->IASetIndexBuffer(&mesh.indexBufferView);
		}
		list.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		if (mesh.heapNum.has_value()) {
			auto h = handle;
			h.ptr+= static_cast<UINT64>(mesh.heapNum.value()) * handleSize;
			list.Get()->SetGraphicsRootDescriptorTable(2, h);
		}
		list.Get()->DrawIndexedInstanced(mesh.indexCount, 1, 0, 0, 0);
	}
}