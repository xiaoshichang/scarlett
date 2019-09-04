#include "MeshRenderComponent.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/World.h"

using namespace scarlett;

int scarlett::MeshRenderComponent::Initialize() noexcept
{
	auto master = GetMaster();
	if (master) {
		auto world = master->GetWorld();
		if (world) {
			world->GetMeshRenderSystem()->AddComponent(this);
		}
	}
	return 0;
}

void scarlett::MeshRenderComponent::Finalize() noexcept
{
	auto master = GetMaster();
	if (master) {
		auto world = master->GetWorld();
		if (world) {
			world->GetMeshRenderSystem()->DeleteComponent(this);
		}
	}
}

void scarlett::MeshRenderComponent::Render()
{
}