#include "Model.h"
#include "../Create/Create.h"
#include "../Math/Math.hpp"

Model::Model()
{
}

Model::~Model()
{
	for (auto &mesh : meshList_) {
		delete mesh;
	}
}

void Model::LoadObjFile(const std::string &directoryPath, const std::string &fileName) {

#pragma region 1. ファイルを開く

	std::ifstream file{ directoryPath + "/" + fileName };
	if (!file.is_open()) return;		// 開けなかった場合、処理を終了する

#pragma endregion

	name_ = fileName.substr(0, fileName.size() - 4);

#pragma region 2. 中で必要になる変数の宣言

	meshList_.emplace_back(new Mesh);

	Mesh &modelData = *meshList_.back();				// 構築するModelData
	std::vector<Vector4> positionList;	// 位置
	std::vector<Vector3> normalList;	// 法線
	std::vector<Vector2> texCoordList;	// テクスチャ座標

	std::string line;					// ファイルから一行を格納する

#pragma endregion

#pragma region 3. ファイルを読み, ModelDataを構築

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s{ line };
		s >> identifier;

		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.z *= -1;
			position.w = 1.f;
			positionList.push_back(position);

		}
		else if (identifier == "vt") {
			Vector2 texCoord;
			s >> texCoord.x >> texCoord.y;
			texCoord.y = 1.f - texCoord.y;
			texCoordList.push_back(texCoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.z *= -1;
			normalList.push_back(normal);
		}
		else if (identifier == "f") {
			// 面は三角形限定。その他は未対応
			Mesh::VertexData triangle[3] = {};
			for (uint32_t faceVertex = 0u; faceVertex < 3u; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのIndexは「 位置 / UV / 法線 」 で格納されているので、分解してIndex を取得する
				std::istringstream v{ vertexDefinition };
				uint32_t elementIndices[3];
				for (uint32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/'); // 区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				// 要素へのIndexから、実際の要素の値を取得して、頂点を機築する
				Vector4 position = positionList[elementIndices[0] - 1];
				Vector2 texCoord = texCoordList[elementIndices[1] - 1];
				Vector3 normal = normalList[elementIndices[2] - 1];
				// 末尾から順に(法線の逆転)
				triangle[2u - faceVertex] = { position,texCoord,normal };

			}
			// イテレータを用いた末尾への直接構築
			modelData.vertices_.insert(modelData.vertices_.end(), triangle, triangle + 3);
			/*modelData.indexs.push_back(modelData.vertices.size() - 3);
			modelData.indexs.push_back(modelData.vertices.size() - 2);
			modelData.indexs.push_back(modelData.vertices.size() - 1);*/
		}
		else if (identifier == "mtllib") {

			std::string materialFile;
			s >> materialFile;
			modelData.material_ = Material::LoadFile(directoryPath, materialFile);
		}
	}
#pragma endregion


}

void Model::Draw(ID3D12GraphicsCommandList *const commandList, const Transform &transform, const Matrix4x4 &viewProjection) const
{
	commandList;
	transform;
	viewProjection;
}

void Mesh::CreateBuffer()
{
	//CreateBufferResource(device, sizeof(Render::VertexData) * vertices_.size());

	//// リソースの先頭のアドレスから使う
	//vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//// 使用するリソースの全体のサイズ
	//vbView_.SizeInBytes = static_cast<UINT>(sizeof(Render::VertexData) * vertices_.size());
	//// 1頂点あたりのサイズ
	//vbView_.StrideInBytes = sizeof(Render::VertexData);


	//// 頂点バッファへのデータ転送
	//Render::VertexData *vertMap = nullptr;
	//HRESULT result = vertBuff_->Map(0, nullptr, (void **)&vertMap);
	//if (SUCCEEDED(result)) {
	//	std::copy(vertices_.begin(), vertices_.end(), vertMap);
	//	vertBuff_->Unmap(0, nullptr);
	//}
}

void Mesh::CreateSphere(VertexData *const vertex, ID3D12Resource *const indexResource, const uint32_t &subdivision)
{

	const float kLonEvery = 360u * Angle::Dig2Rad / subdivision; // 経度
	const float kLatEvery = 180u * Angle::Dig2Rad / subdivision; // 緯度

	// 緯度の方向に分解 -90 ~ 90
	for (uint32_t latIndex = 0u; latIndex < subdivision; ++latIndex) {
		const float lat = static_cast<float>(-Angle::PI) / 2.f + kLatEvery * latIndex;
		// 経度の方向に分解 0 ~ 360
		for (uint32_t lonIndex = 0u; lonIndex < subdivision; ++lonIndex) {
			uint32_t indexStart = (latIndex * subdivision + lonIndex) * 4;
			const float lon = lonIndex * kLonEvery;

#pragma region Vertex[0] 左下

			VertexData *vertexData = &vertex[indexStart + 0u];
			vertexData->position = { //左下
				std::cos(lat) * std::cos(lon),std::sin(lat),
				std::cos(lat) * std::sin(lon), 1
			};
			vertexData->texCoord = {
				static_cast<float>(lonIndex) / subdivision,
				1.f - static_cast<float>(latIndex) / subdivision
			};
			vertexData->normal.x = vertexData->position.x;
			vertexData->normal.y = vertexData->position.y;
			vertexData->normal.z = vertexData->position.z;
			vertexData = nullptr;

#pragma endregion

#pragma region Vertex[1] 左上

			vertexData = &vertex[indexStart + 1u];
			vertexData->position = { //左上
				std::cos(lat + kLatEvery) * std::cos(lon),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon), 1
			};
			vertexData->texCoord = {
				static_cast<float>(lonIndex) / subdivision,
				1.f - static_cast<float>(latIndex + 1u) / subdivision
			};
			vertexData->normal.x = vertexData->position.x;
			vertexData->normal.y = vertexData->position.y;
			vertexData->normal.z = vertexData->position.z;
			vertexData = nullptr;

#pragma endregion

#pragma region Vertex[2] 右下

			vertexData = &vertex[indexStart + 2u];
			vertexData->position = { //右下
				std::cos(lat) * std::cos(lon + kLonEvery),
				std::sin(lat),
				std::cos(lat) * std::sin(lon + kLonEvery), 1
			};
			vertexData->texCoord = {
				static_cast<float>(lonIndex + 1u) / subdivision,
				1.f - static_cast<float>(latIndex) / subdivision
			};
			vertexData->normal.x = vertexData->position.x;
			vertexData->normal.y = vertexData->position.y;
			vertexData->normal.z = vertexData->position.z;
			vertexData = nullptr;

#pragma endregion
			//
			//#pragma region Vertex[3] 左上
			//
			//			vertexData = &vertex[indexStart + 3u];
			//			vertexData->position = { //左上
			//				std::cos(lat + kLatEvery) * std::cos(lon),
			//				std::sin(lat + kLatEvery),
			//				std::cos(lat + kLatEvery) * std::sin(lon), 1
			//			};
			//			vertexData->texCoord = {
			//				static_cast<float>(lonIndex) / subdivision,
			//				1.f - static_cast<float>(latIndex + 1u) / subdivision
			//			};
			//			vertexData->normal.x = vertexData->position.x;
			//			vertexData->normal.y = vertexData->position.y;
			//			vertexData->normal.z = vertexData->position.z;
			//			vertexData = nullptr;
			//
			//#pragma endregion

#pragma region Vertex[3] 右上

			vertexData = &vertex[indexStart + 3u];
			vertexData->position = { //右上
				std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery), 1
			};
			vertexData->texCoord = {
				static_cast<float>(lonIndex + 1u) / subdivision,
				1.f - static_cast<float>(latIndex + 1u) / subdivision
			};
			vertexData->normal.x = vertexData->position.x;
			vertexData->normal.y = vertexData->position.y;
			vertexData->normal.z = vertexData->position.z;
			vertexData = nullptr;

#pragma endregion
			//
			//#pragma region Vertex[5] 右下
			//
			//			vertexData = &vertex[indexStart + 5u];
			//			vertexData->position = { //右下
			//				std::cos(lat) * std::cos(lon + kLonEvery),
			//				std::sin(lat),
			//				std::cos(lat) * std::sin(lon + kLonEvery), 1
			//			};
			//			vertexData->texCoord = {
			//				static_cast<float>(lonIndex + 1u) / subdivision,
			//				1.f - static_cast<float>(latIndex) / subdivision
			//			};
			//			vertexData->normal.x = vertexData->position.x;
			//			vertexData->normal.y = vertexData->position.y;
			//			vertexData->normal.z = vertexData->position.z;
			//			vertexData = nullptr;
			//
			//#pragma endregion

		}
	}

	uint32_t *indexData =
		nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void **>(&indexData));
	for (uint32_t i = 0; i < subdivision * subdivision; i++) {

		indexData[i * 6u + 0u] = i * 4u + 0u; indexData[i * 6u + 1u] = i * 4u + 1u; indexData[i * 6u + 2u] = i * 4u + 2u;
		indexData[i * 6u + 3u] = i * 4u + 1u; indexData[i * 6u + 4u] = i * 4u + 3u; indexData[i * 6u + 5u] = i * 4u + 2u;

	}

}
