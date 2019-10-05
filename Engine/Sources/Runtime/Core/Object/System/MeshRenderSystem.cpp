#include "MeshRenderSystem.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Object/System/MeshRenderSystem.h"


scarlett::MeshRenderSystem::MeshRenderSystem(World * world) :
	mWorld(world)
{
}

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

			auto transform = comp->GetMaster()->GetComponent<TransformComponent>();
			for (auto mesh : comp->mMeshes) {
				auto entity = comp->GetMaster();
				mesh->Render(entity);
			}
		}
	}
}
