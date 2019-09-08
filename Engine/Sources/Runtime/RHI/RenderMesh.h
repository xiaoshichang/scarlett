#pragma once
#include <memory>

#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure

#include "Runtime/Interface/IResource.h"
#include "Runtime/RHI/VertexBuffer.h"
#include "Runtime/RHI/IndexBuffer.h"

using namespace std;

namespace scarlett {

	class RenderMesh : public IMesh{
	public:
		RenderMesh();
		virtual ~RenderMesh();

		virtual void Render(GraphicsManager* mgr, World* world) noexcept;
		int GetVaildVertexBufferCount() noexcept;

	public:
		shared_ptr<VertexBuffer>	mPositions;
		shared_ptr<VertexBuffer>	mNormals;
		shared_ptr<VertexBuffer>	mTexCoords;
		shared_ptr<IndexBuffer>		mIndexes;
		PrimitiveType				mType;
	};

}