#include "PhysicsSystem.h"

#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Math/ScltMath.h"
#include "Runtime/Core/Object/Components/RigidBodyComponent.h"
#include "Runtime/Core/Physics/BoardPhase.h"
#include "Runtime/Core/Physics/NarrowPhase.h"
#include "Runtime/Core/Physics/ResolutionPhase.h"
#include "Runtime/Core/Physics/IntegratePhase.h"

using namespace scarlett;

scarlett::PhysicsSystem::PhysicsSystem(World* world)
{
	this->mWorld = world;
	this->boardPhase = new BoardPhaseNquared();
	this->narrowPhase = new NarrowPhaseGJKEPA();
	this->resolutionPhase = new ResolutionPhaseConstraintBase();
	this->integratePhase = new IntegratePhase();
}

int scarlett::PhysicsSystem::Initialize() noexcept
{
	gravity = Vector3f(0.0f, -9.80665f, 0.0f);
	return 0;
}

void scarlett::PhysicsSystem::Finalize() noexcept
{
}

void scarlett::PhysicsSystem::Tick() noexcept
{
	float deltaTime = GApp->mTimeMgr->GetDeltaMsTime() * 0.001;

	std::vector<RigidBody*> activedRigidBodies;
	CollectRigidBodies(activedRigidBodies);

	// udpate Inertia tensor in global 坐标
	for each (RigidBody* rigidBody in activedRigidBodies)
	{
		rigidBody->UpdateInverseInertiaWs();
	}

	// gravity
	Vector3f gravityImpulse = VectorScale(gravity, deltaTime);
	for each (RigidBody* rigidBody in activedRigidBodies)
	{
		if (rigidBody->IsStatic())
		{
			continue;
		}
		rigidBody->SetVelocity(rigidBody->GetVelocity() + gravityImpulse);
	}


	// boardPhase
	boardPhase->Update(deltaTime);

	vector<RigidBodyPair> candicate;
	boardPhase->GeneratePossiblePair(activedRigidBodies, candicate);

	// narrowPhase
	narrowPhase->CollideDetection(candicate, manifolds);

	// resolutionPhase
	resolutionPhase->Resolve(manifolds, deltaTime);

	// integratePhase
	integratePhase->integrate(activedRigidBodies, deltaTime);

	// clearPhase
	manifolds.clear();
	
}

void scarlett::PhysicsSystem::AddComponent(RigidBodyComponent * comp)
{
	if (nullptr == comp)
	{
		return;
	}

	components.insert(comp);
}

void scarlett::PhysicsSystem::DeleteComponent(RigidBodyComponent * comp)
{
	if (nullptr == comp)
	{
		return;
	}

	components.erase(comp);
}

/*
暂时先每帧都收集一遍。后续可以加脏标记或者维护一个有效集合。
*/
void scarlett::PhysicsSystem::CollectRigidBodies(std::vector<RigidBody*>& activedRigidBodies)
{
	activedRigidBodies.clear();
	for each (RigidBodyComponent* comp in components)
	{
		activedRigidBodies.push_back((comp->GetRigidBody()));
	}

}



