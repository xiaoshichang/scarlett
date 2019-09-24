#pragma once
#include <memory>

#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure

#include "Runtime/Interface/IResource.h"
#include "Runtime/RHI/VertexBuffer.h"
#include "Runtime/RHI/IndexBuffer.h"
#include "Runtime/RHI/Material.h"


namespace scarlett {
	class World;

	enum MeshType {
		MT_Model = 1,
		MT_Skybox = 2
	};

	enum PrimitiveType {
		PT_POINT = 1,
		PT_LINE,
		PT_TRIANGLE,
	};

	class IMesh : public IRenderResource {
	public:
		virtual void Initialize(aiMesh* mesh) noexcept = 0;
		virtual void Initialize(void* data, int count, VertexFormat vf) noexcept = 0;
		virtual void Render(World* world, const Matrix4f& worldMatrix) noexcept = 0;

		int	GetVaildVertexBufferCount() {
			int result = 0;
			if (mPositions) result += 1;
			if (mNormals) result += 1;
			if (mTexCoords) result += 1;
			return result;
		}

		IMesh() : 
			mMeshType(MeshType::MT_Model),
			mPrimitiveType(PrimitiveType::PT_TRIANGLE),
			mPositions(nullptr),
			mNormals(nullptr),
			mTexCoords(nullptr),
			mIndexes(nullptr),
			mMaterial(nullptr){}

		std::shared_ptr<IMaterial>	GetMaterial() { return mMaterial; }

	public:
		MeshType					mMeshType;
		PrimitiveType				mPrimitiveType;

		std::shared_ptr<IVertexBuffer>	mPositions;
		std::shared_ptr<IVertexBuffer>	mNormals;
		std::shared_ptr<IVertexBuffer>	mTexCoords;
		std::shared_ptr<IIndexBuffer>	mIndexes;
		std::shared_ptr<IMaterial>		mMaterial;
	};
}