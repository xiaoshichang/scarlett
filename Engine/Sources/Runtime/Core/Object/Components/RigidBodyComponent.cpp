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

RigidBodyComponent::RigidBodyComponent(float mass, const Matrix4x4f& inertia, float impulseCoeff, float frictionCoeff):
	rigidBody(mass, inertia, impulseCoeff, frictionCoeff)
{
}

RigidBody * scarlett::RigidBodyComponent::GetRigidBody()
{
	return &rigidBody;
}

