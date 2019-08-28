#include "MeshRenderSystem.h"
#include "Runtime/Core/Object/System/MeshRenderSystem.h"

int scarlett::MeshRenderSystem::Initialize() noexcept
{
	return 0;
}

void scarlett::MeshRenderSystem::Finalize() noexcept
{
}

void scarlett::MeshRenderSystem::Tick() noexcept
{
}

void scarlett::MeshRenderSystem::AddComponent(MeshRenderComponent * comp)
{
	mComponents.insert(comp);
}

void scarlett::MeshRenderSystem::DeleteComponent(MeshRenderComponent * comp)
{
	mComponents.erase(comp);
}


void scarlett::MeshRenderSystem::Render()
{
	if (!IsActive())
		return;

	for (auto comp : mComponents) {
		if (comp->IsVisible()) {
			comp->Render();
		}
	}
}
