#include "AnimationSystem.h"
#include "Runtime/Core/Object/World.h"
using namespace scarlett;

scarlett::AnimationSystem::AnimationSystem(World* world):
	mWorld(world)
{
}

scarlett::AnimationSystem::~AnimationSystem()
{
}

int scarlett::AnimationSystem::Initialize() noexcept
{
	return 0;
}

void scarlett::AnimationSystem::Finalize() noexcept
{
}

void scarlett::AnimationSystem::Tick() noexcept
{
	for (auto skeleton : mComponents) {
		skeleton->Tick();
	}

}

void scarlett::AnimationSystem::AddComponent(SkeletonComponent * comp)
{
	mComponents.insert(comp);
}

void scarlett::AnimationSystem::DeleteComponent(SkeletonComponent * comp)
{
	mComponents.erase(comp);

}
