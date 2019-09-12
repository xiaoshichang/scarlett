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

	class RenderMesh : public IMesh{
	public:
		RenderMesh();
		virtual ~RenderMesh();
		virtual void		Initialize(std::shared_ptr<VertexBuffer> vb) noexcept;
		virtual void		Render(World* world, const Matrix4f& worldMatrix) noexcept;
		virtual int			GetVaildVertexBufferCount() noexcept;
		virtual void		Finialize() noexcept;
		virtual shared_ptr<Material>		GetMaterial() noexcept;

	public:
		shared_ptr<VertexBuffer>	mPositions;
		shared_ptr<VertexBuffer>	mNormals;
		shared_ptr<VertexBuffer>	mTexCoords;
		shared_ptr<IndexBuffer>		mIndexes;
		PrimitiveType				mType;
		shared_ptr<Material>		mMaterial;
	};

}