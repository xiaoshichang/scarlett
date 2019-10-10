#include "Renderer.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Application/Application.h"

using namespace scarlett;

Renderer::Renderer(){
	mQuad = GApp->mGraphicsManager->CreateRenderMeshUI();
}

void scarlett::Renderer::RenderSprite(const std::string & texture, const Matrix4x4f & world, const Matrix4x4f & transform, uint32_t flags)
{
	auto t = GApp->mGraphicsManager->CreateTexture2D(texture);
	mQuad->mMaterial->SetTexture("ui", t);
	mQuad->mMaterial->SetName("ui");
	Matrix4x4f w, v, p;
	BuildMatrixIdentity(v);
	mQuad->Render(world, v, transform);
}

