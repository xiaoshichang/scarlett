#include "MeshRenderComponent.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/World.h"

using namespace scarlett;

int scarlett::MeshRenderComponent::Initialize() noexcept
{
	GetMaster()->GetWorld()->GetMeshRenderSystem()->AddComponent(this);
	return 0;
}

void scarlett::MeshRenderComponent::Finalize() noexcept
{
	GetMaster()->GetWorld()->GetMeshRenderSystem()->DeleteComponent(this);
}

void scarlett::MeshRenderComponent::Render()
{
	for (auto mesh : mRenderObjects) {
		if (mesh->IsVisible()) {
			mesh->Render();
		}
	}
}


void scarlett::MeshRenderComponent::AddRenderObject(std::string resourcePath)
{
	// todo: create a RenderObject by resource path.
	auto renderObject = std::make_shared<RenderObject>();
	renderObject->SetVisible(true);
	mRenderObjects.push_back(renderObject);
}
