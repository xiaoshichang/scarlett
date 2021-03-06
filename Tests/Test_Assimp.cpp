#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure
#include "assimp/postprocess.h"     // Post processing flags

#include <iostream>

int main()
{
	const std::string& pFile2 = "Asset/Scenes/aili_cycle.fbx";
	// Create an instance of the Importer class
	Assimp::Importer importer2;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene2 = importer2.ReadFile(pFile2,
		//aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (scene2 == nullptr) {
		std::cout << importer2.GetErrorString() << std::endl;
		return 1;
	}
	auto root = scene2->mRootNode;

	std::cout << "numtexuters: " << scene2->mNumTextures << std::endl;
	
	for (unsigned int i = 0; i < root->mNumChildren; ++i) {
		auto node = root->mChildren[i];
		std::cout << "node name: " << node->mName.C_Str() << std::endl;
		std::cout << "mesh cout: " << node->mNumMeshes << std::endl;
		if (node->mNumMeshes > 0) {
			auto mesh = scene2->mMeshes[node->mMeshes[0]];
			std::cout << "primitives type: " << mesh->mPrimitiveTypes << std::endl;
			std::cout << "has position" << mesh->HasPositions() << std::endl;
			std::cout << "position vertex count: " << mesh->mNumVertices << std::endl;
			std::cout << "has normal" << mesh->HasNormals() << std::endl;
			auto first = mesh->mNormals[0];
			std::cout << "first normal" << first.x << "," << first.y << "," << first.z << std::endl;
			std::cout << "faces: " << mesh->mNumFaces << std::endl;
			std::cout << "uv channels: " << mesh->GetNumUVChannels() << std::endl;
			std::cout << "has uvcoord: " << mesh->HasTextureCoords(0) << std::endl;
			
			std::cout << "uv components: " << mesh->mNumUVComponents[0] << std::endl;
			auto uv = mesh->mTextureCoords[0];
			if (uv) {
				std::cout << "first uv" << uv->x << "," << uv->y << std::endl;
			}

			auto midx = mesh->mMaterialIndex;
			auto material = scene2->mMaterials[midx];

			aiColor4D diffuse;
			aiColor4D displace;
			aiColor4D emb;
			aiString name;

			if (AI_SUCCESS == aiGetMaterialString(material, AI_MATKEY_NAME, &name)) {
				std::cout << "material name" << name.C_Str() <<  std::endl;
			}

			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
				std::cout << "diffuse color" << diffuse.r << diffuse.g << diffuse.b << std::endl;
			}
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &emb)) {
				std::cout << "emb color" << emb.r << emb.g << emb.b << std::endl;
			}
			int model;
			if (AI_SUCCESS == aiGetMaterialInteger(material, AI_MATKEY_SHADING_MODEL, &model)) {
				std::cout << "MODEL" << model << std::endl;
			}

			auto diffuset = aiGetMaterialTextureCount(material, aiTextureType::aiTextureType_DIFFUSE);
			auto displayt = aiGetMaterialTextureCount(material, aiTextureType::aiTextureType_DISPLACEMENT);
			std::cout << "ttt" << diffuset << displayt << std::endl;

			for (int i = 0; i < material->mNumProperties; ++i) {
				std::cout << material->mProperties[i]->mKey.C_Str() << "	ttt" << material->mProperties[i]->mType << "	l" << material->mProperties[i]->mDataLength << std::endl;
			}

		}
		std::cout << std::endl;
	}

	aiCamera* camera = nullptr;
	if (scene2->HasCameras()) {
		camera = scene2->mCameras[0];
	}
	std::cout << "camera:" << std::endl;
	std::cout << "Aspect: " << camera->mAspect << std::endl;
	std::cout << "name: " << camera->mName .C_Str() << std::endl;
	std::cout << "mHorizontalFOV" << camera->mHorizontalFOV << std::endl;
	std::cout << camera->mPosition.x << "," << camera->mPosition.y << "," << camera->mPosition.z << std::endl;
	std::cout << camera->mLookAt.x << "," << camera->mLookAt.y << "," << camera->mLookAt.z << std::endl;
	std::cout << camera->mUp.x << "," << camera->mUp.y << "," << camera->mUp.z << std::endl;

	if ( scene2) {
		return 0;
	}

	return 1;
}