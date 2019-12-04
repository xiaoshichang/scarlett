#include "TerrainSystem.h"
#include "TerrainSystem.h"

scarlett::TerrainSystem::TerrainSystem(World * world)
{
	mWorld = world;
}

int scarlett::TerrainSystem::Initialize() noexcept
{
	return 0;
}

void scarlett::TerrainSystem::Finalize() noexcept
{
}

void scarlett::TerrainSystem::AddComponent(TerrainComponent * comp)
{
	mComponents.insert(comp);
}

void scarlett::TerrainSystem::DeleteComponent(TerrainComponent * comp)
{
	mComponents.erase(comp);
}



void scarlett::TerrainSystem::Render()
{
	
	for (auto tc : mComponents) {
		tc->Render();
	}
}

void scarlett::TerrainSystem::Tick() noexcept
{
	// todo: visibility Culling
	// todo: lod update
	for (auto tc : mComponents) {
		tc->Tick();
	}
}
