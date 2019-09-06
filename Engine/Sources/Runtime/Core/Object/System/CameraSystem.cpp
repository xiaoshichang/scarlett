#include "CameraSystem.h"

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

std::shared_ptr<Entity> scarlett::CameraSystem::GetMainCamera()
{
	return mMainCamera;
}

void scarlett::CameraSystem::SetMainCamera(std::shared_ptr<Entity> camera)
{
	mMainCamera = camera;
}
