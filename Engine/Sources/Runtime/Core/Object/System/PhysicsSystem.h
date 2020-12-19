#pragma once
#include <set>
#include <vector>
#include <memory>
#include "Runtime//Core/Math/ScltMath.h"
#include "Runtime/Interface/ISystem.h"


namespace scarlett
{
	class World;
	class RigidBodyComponent;
	class IBoardPhase;
	class INarrowPhase;
	class IResolutionPhase;
	class IIntegratePhase;
	class ContactManifold;
	class RigidBody;

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

		std::vector< std::shared_ptr<ContactManifold> > manifolds;
	};
}
