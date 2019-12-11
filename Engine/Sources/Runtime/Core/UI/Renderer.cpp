#include "Renderer.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Application/Application.h"

using namespace scarlett;

Renderer::Renderer(){
	mQuad = GApp->mGraphicsManager->CreateRenderMeshUI();
	mQuad->GetMaterial()->GetBlendState()->SetEnable(true);

	mQuadFont = GApp->mGraphicsManager->CreateRenderMeshUI();
	mQuadFont->GetMaterial()->GetBlendState()->SetEnable(true);

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

void scarlett::Renderer::RenderFont(std::shared_ptr<ITexture> fontTexture, const Matrix4x4f & world, const Matrix4x4f & transform, uint32_t flags)
{

	mQuadFont->mMaterial->SetTexture("ui", fontTexture);
	mQuadFont->mMaterial->SetName("ui");
	Matrix4x4f w, v, p;
	BuildMatrixIdentity(v);
	mQuadFont->Render(world, v, transform);
}

