#include "CameraSystem.h"
#include "Foundation/Assert.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Object/System/MeshRenderSystem.h"

using namespace scarlett;

scarlett::CameraSystem::CameraSystem(World * world):
	mWorld(world)
{
}

int scarlett::CameraSystem::Initialize() noexcept
{
	return 0;
}

void scarlett::CameraSystem::Finalize() noexcept
{
	mMainCamera = nullptr;
}

void scarlett::CameraSystem::RenderBackground()
{
	SCARLETT_ASSERT(mMainCamera);
	auto camera = mMainCamera->GetComponent<CameraComponent>();

	auto skybox = camera->GetSkybox();
	if (skybox) {
		skybox->Render();
	}
	else {
		GApp->mGraphicsManager->ClearRenderTarget(0.2f, 0.4f, 0.6f, 1.0f);
	}
}

void scarlett::CameraSystem::RenderShadowMap()
{
	SCARLETT_ASSERT(mMainCamera);
	auto camera = mMainCamera->GetComponent<CameraComponent>();

}

std::shared_ptr<Entity> scarlett::CameraSystem::GetMainCamera()
{
	return mMainCamera;
}

void scarlett::CameraSystem::SetMainCamera(std::shared_ptr<Entity> camera)
{
	mMainCamera = camera;
}
