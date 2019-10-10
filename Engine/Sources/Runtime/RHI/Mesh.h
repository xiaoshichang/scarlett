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
	class Entity;

	enum MeshType {
		MT_Model = 1,
		MT_Skybox = 2,
		MT_UI = 3,
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
		virtual void InitializeUI() noexcept = 0;
		virtual void Render(Entity* self) noexcept = 0;
		virtual void Render(const Matrix4x4f& world, const Matrix4x4f& view, const Matrix4x4f& projection) noexcept = 0;

		IMesh() : 
			mMeshType(MeshType::MT_Model),
			mPrimitiveType(PrimitiveType::PT_TRIANGLE),
			mPositions(nullptr),
			mNormals(nullptr),
			mTexCoords(nullptr),
			mIndexes(nullptr),
			mBoneIdxes(nullptr),
			mBoneWeights(nullptr),
			mMaterial(nullptr){}

		std::shared_ptr<IMaterial>	GetMaterial() { return mMaterial; }

	public:
		MeshType					mMeshType;
		PrimitiveType				mPrimitiveType;

		std::shared_ptr<IVertexBuffer>	mPositions;
		std::shared_ptr<IVertexBuffer>	mNormals;
		std::shared_ptr<IVertexBuffer>	mTexCoords;
		std::shared_ptr<IVertexBuffer>  mBoneIdxes;
		std::shared_ptr<IVertexBuffer>  mBoneWeights;
		std::shared_ptr<IIndexBuffer>	mIndexes;
		std::shared_ptr<IMaterial>		mMaterial;
	};
}