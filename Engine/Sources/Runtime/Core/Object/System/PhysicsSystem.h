#pragma once
#include <set>
#include <vector>
#include "Runtime/Interface/ISystem.h"
#include "Runtime/Core/Object/Components/RigidBodyComponent.h"

#include "Runtime/Core/Physics/BoardPhase.h"
#include "Runtime/Core/Physics/NarrowPhase.h"
#include "Runtime/Core/Physics/ResolutionPhase.h"
#include "Runtime/Core/Physics/IntegratePhase.h"


namespace scarlett
{
	class World;
	class RigidBodyComponent;

	class PhysicsSystem : public ITickableSystem
	{


	public:
		PhysicsSystem(World* world);
		virtual int		Initialize() noexcept;
		virtual void	Finalize() noexcept;
		virtual void	Tick() noexcept;

		virtual void AddComponent(RigidBodyComponent* comp);
		virtual void DeleteComponent(RigidBodyComponent* comp);

		void CollectRigidBodies(std::vector<RigidBody*>& activedRigidBodies);

	private:
		std::set<RigidBodyComponent*> components;

	private:
		Vector3f gravity;
		World* mWorld;

		IBoardPhase* boardPhase;
		INarrowPhase* narrowPhase;
		IResolutionPhase* resolutionPhase;
		IIntegratePhase* integratePhase;


	};
}
