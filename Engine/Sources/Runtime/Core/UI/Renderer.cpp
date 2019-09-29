#include "Renderer.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Application/Application.h"

using namespace scarlett;

Renderer::Renderer(){
	mQuad = GApp->mGraphicsManager->CreateRenderMeshUI();
}

void scarlett::Renderer::RenderSprite(const std::string & texture, const Matrix4f & world, const Matrix4f & transform, uint32_t flags)
{
	auto t = GApp->mGraphicsManager->CreateTexture2D(texture);
	mQuad->mMaterial->SetTexture("ui", t);
	mQuad->mMaterial->SetName("ui");
	mQuad->Render(world.transpose(), Matrix4f::Identity(), transform.transpose());
}

