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
	friction = 1.0f;
	contactBeta = 0.4f;
	restitution = 0.0f;
}

RigidBody * scarlett::RigidBodyComponent::GetRigidBody()
{
	return &rigidBody;
}

AABB * scarlett::RigidBodyComponent::GetAABB()
{
	return &aabb;
}

void scarlett::RigidBodyComponent::SetFriction(float f)
{
	if (f <= 0.0f)
	{
		friction = 0.0f;
		return;
	}
	if (f >= 1.0f)
	{
		friction = 1.0f;
		return;
	}
	friction = f;
}

void scarlett::RigidBodyComponent::SetContactBeta(float b)
{
	if (b <= 0.0f)
	{
		contactBeta = 0.0f;
		return;
	}
	if (b >= 1.0f)
	{
		contactBeta = 1.0f;
		return;
	}
	contactBeta = b;
}

void scarlett::RigidBodyComponent::SetRestitution(float r)
{
	if (r <= 0.0f)
	{
		restitution = 0.0f;
		return;
	}

	if (r >= 1.0f)
	{
		restitution = 1.0f;
		return;
	}

	restitution = r;
}

float scarlett::RigidBodyComponent::GetFriction()
{
	return friction;
}

float scarlett::RigidBodyComponent::GetContactBeta()
{
	return contactBeta;
}

float scarlett::RigidBodyComponent::GetRestitution()
{
	return restitution;
}

