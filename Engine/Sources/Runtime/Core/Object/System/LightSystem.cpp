#include "LightSystem.h"

scarlett::LightSystem::LightSystem(World * world):
	mWorld(world)
{
}

int scarlett::LightSystem::Initialize() noexcept
{
	return 0;
}

void scarlett::LightSystem::Finalize() noexcept
{
}

void scarlett::LightSystem::Tick() noexcept
{
}

void scarlett::LightSystem::AddComponent(scarlett::LightComponent * comp)
{
	mComponents.insert(comp);
}

void scarlett::LightSystem::DeleteComponent(scarlett::LightComponent * comp)
{
	mComponents.erase(comp);
}

scarlett::LightComponent * scarlett::LightSystem::GetMainDirectionalLight()
{
	if (mComponents.empty()) {
		return nullptr;
	}

	return *mComponents.begin();
}
