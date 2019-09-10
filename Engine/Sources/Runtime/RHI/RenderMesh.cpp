#include "Runtime/RHI/RenderMesh.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/RHI/GraphicsMgr.h"


scarlett::RenderMesh::RenderMesh():
	mType(PrimitiveType::PT_POINT),
	mPositions(nullptr),
	mNormals(nullptr),
	mTexCoords(nullptr),
	mIndexes(nullptr)
{
}

scarlett::RenderMesh::~RenderMesh()
{
}

void scarlett::RenderMesh::Initialize(GraphicsManager * mgr, std::shared_ptr<VertexBuffer> vb) noexcept
{
}

void scarlett::RenderMesh::Render(GraphicsManager* mgr, World* world, const Matrix4f& worldMatrix) noexcept {
}

int scarlett::RenderMesh::GetVaildVertexBufferCount() noexcept {
	int result = 0;
	if (mPositions) {
		result += 1;
	}
	if (mNormals) {
		result += 1;
	}
	if (mTexCoords) {
		result += 1;
	}
	return result;
}