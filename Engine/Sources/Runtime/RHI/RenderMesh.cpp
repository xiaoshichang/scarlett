#include "Runtime/RHI/RenderMesh.h"
#include "Runtime/RHI/GraphicsMgr.h"


scarlett::RenderMesh::RenderMesh():
	mPositions(nullptr),
	mNormals(nullptr),
	mTexCoords(nullptr)
{
}

scarlett::RenderMesh::~RenderMesh()
{
}
