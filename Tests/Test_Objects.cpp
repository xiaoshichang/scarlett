#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Object/Components/TransformComponent.h"
#include "Runtime/Core/Object/Components/MeshRenderComponent.h"
#include <iostream>

#include "Runtime/Core/Application/Application.h"

namespace scarlett {
	extern Application* GApp = new Application();
}

int main() {

	auto world = new scarlett::World();
	world->Initialize();
	auto entity = world->CreateEntity();
	std::cout << entity->GetGuid() << std::endl;

	auto guid = boost::uuids::random_generator()();
	auto entity2 = world->CreateEntity(guid);
	std::cout << entity2->GetGuid() << std::endl;
	std::cout << guid << std::endl;

	std::cout << "entity count: " << world->GetEntityCount() << std::endl;

	entity2->AddChild(entity);
	std::cout << "children count: " << entity2->GetChildrenCount() << std::endl;
	std::cout << "parent of entity is: " << entity->GetParent()->GetGuid() << std::endl;

	auto comp = entity->GetComponent<scarlett::TransformComponent>();
	comp->SetPosition(Vector3f(1, 1, 1));

	auto comp1 = entity->GetComponent<scarlett::MeshRenderComponent>();

	entity->AddComponent<scarlett::MeshRenderComponent>();
	std::cout << "positoin: " << entity->GetComponent<scarlett::TransformComponent>()->GetPosition() << std::endl;
	entity->RemoveComponent<scarlett::TransformComponent>();

	entity2->RemoveChild(entity);
	std::cout << "RemoveChild " << std::endl;
	std::cout << "children count: " << entity2->GetChildrenCount() << std::endl;

	world->DeleteEntity(guid);
	std::cout << "entity count: " << world->GetEntityCount() << std::endl;

	world->Finalize();
	return 0;
}