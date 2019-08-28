#include "Runtime/Core/Object/World.h"
using namespace scarlett;

scarlett::World::World() : 
	mMeshRenderSystem(nullptr)
{
}

int scarlett::World::Initialize() noexcept
{
	mMeshRenderSystem = new MeshRenderSystem();
	mMeshRenderSystem->Initialize();

	return 0;
}

void scarlett::World::Finalize() noexcept
{
	mEntities.clear();
}

void scarlett::World::Tick() noexcept
{
	mMeshRenderSystem->Tick();
}

void scarlett::World::Render() noexcept
{
	mMeshRenderSystem->Render();
}

std::shared_ptr<Entity> scarlett::World::CreateEntity()
{
	auto entity = std::make_shared<Entity>();
	entity->Initialize(this);
	mEntities[entity->GetGuid()] = entity;
	return entity;
}

std::shared_ptr<Entity> scarlett::World::CreateEntity(const Guid & guid)
{
	if (mEntities[guid]) {
		return nullptr;
	}

	auto entity = std::make_shared<Entity>(guid);
	entity->Initialize(this);
	mEntities[guid] = entity;
	return entity;
}

void scarlett::World::DeleteEntity(const Guid & guid)
{
	auto entity = mEntities[guid];
	if (entity) {
		entity->Finalize();
		mEntities.erase(guid);
	}
}

size_t scarlett::World::GetEntityCount() {
	return mEntities.size();
}

void scarlett::World::LoadScene(std::string scenePath) {
	auto entity = CreateEntity();
	entity->AddComponent<MeshRenderComponent>();

	auto mesh = entity->GetComponent<MeshRenderComponent>();
	mesh->SetVisible(true);
	mesh->AddRenderObject("aili");
}