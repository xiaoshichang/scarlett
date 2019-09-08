#include "MeshRenderSystem.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Object/System/MeshRenderSystem.h"


scarlett::MeshRenderSystem::MeshRenderSystem(World * world) :
	mWorld(world)
{
}

int scarlett::MeshRenderSystem::Initialize() noexcept
{
	mGraphicsManager = mWorld->mApp->mGraphicsManager;
	return 0;
}

void scarlett::MeshRenderSystem::Finalize() noexcept
{
	for (auto mesh : mMeshes) {
		mGraphicsManager->DeleteRenderMesh(mesh);
	}
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
			for (auto mid : comp->mMeshIdxes) {
				auto mesh = mMeshes[mid];
				if (mesh) {
					mesh->Render(mGraphicsManager, mWorld);
				}
			}
		}
	}
}

void scarlett::MeshRenderSystem::LoadMesh(aiMesh * mesh)
{
	auto _mesh = mGraphicsManager->CreateRenderMesh(mesh);
	mMeshes.push_back(_mesh);
}
