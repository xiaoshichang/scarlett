#include "RigidBodyComponent.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Object/Components/TransformComponent.h"

using namespace scarlett;

int scarlett::RigidBodyComponent::Initialize() noexcept
{
	auto master = GetMaster();
	if (master) {
		auto world = master->GetWorld();
		if (world) {
			world->GetPhysicsSystem()->AddComponent(this);
		}
	}
	rigidBody.SetMaster(this);
	return 0;
}

void scarlett::RigidBodyComponent::Finalize() noexcept
{
	auto master = GetMaster();
	if (master) {
		auto world = master->GetWorld();
		if (world) {
			world->GetPhysicsSystem()->DeleteComponent(this);
		}
	}
}

void scarlett::RigidBodyComponent::Tick() noexcept
{
}

RigidBodyComponent::RigidBodyComponent()
{

}

RigidBody * scarlett::RigidBodyComponent::GetRigidBody()
{
	return &rigidBody;
}

AABB * scarlett::RigidBodyComponent::GetAABB()
{
	return &aabb;
}

