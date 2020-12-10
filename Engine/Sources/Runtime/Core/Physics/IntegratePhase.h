#pragma once
#include <vector>


namespace scarlett
{
	class RigidBody;
	class IIntegratePhase
	{
	public:
		virtual void integrate(std::vector<RigidBody*> activedRigidBodies, float dt) = 0;
	};

	class IntegratePhase : public IIntegratePhase
	{
	public:
		virtual void integrate(std::vector<RigidBody*> activedRigidBodies, float dt);
	};
}