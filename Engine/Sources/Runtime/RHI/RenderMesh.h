#pragma once
#include <memory>

#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure

#include "Runtime/Interface/IResource.h"
#include "Runtime/RHI/VertexBuffer.h"
#include "Runtime/RHI/IndexBuffer.h"
#include "Runtime/RHI/Material.h"

using namespace std;

namespace scarlett {

	enum MeshType {
		MT_Model = 1,
		MT_Skybox = 2
	};

	class RenderMesh : public IMesh{
	public:
		RenderMesh();
		virtual ~RenderMesh();
		virtual void		Initialize(void* data, int count, VertexFormat vf) noexcept;
		virtual void		Render(World* world, const Matrix4f& worldMatrix) noexcept;
		virtual int			GetVaildVertexBufferCount() noexcept;
		virtual void		Finialize() noexcept;
		virtual shared_ptr<Material>		GetMaterial() noexcept;

	public:
		MeshType					mMeshType;
		shared_ptr<VertexBuffer>	mPositions;
		shared_ptr<VertexBuffer>	mNormals;
		shared_ptr<VertexBuffer>	mTexCoords;
		shared_ptr<IndexBuffer>		mIndexes;
		PrimitiveType				mType;
		shared_ptr<Material>		mMaterial;
	};

}