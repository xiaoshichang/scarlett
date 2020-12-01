#pragma once
#include <vector>

#include "Runtime/Core/Physics/BoardPhase.h"
#include "Runtime/Core/Physics/Contact.h"

namespace scarlett
{
	class INarrowPhase
	{
	public:
		virtual void CollideDetection(std::vector<RigidBodyPair>& rigidBodies, std::vector<ContactManifold*>&) = 0;

	};

	class NarrowPhaseGJKEPA: public INarrowPhase
	{
		virtual void CollideDetection(std::vector<RigidBodyPair>& rigidBodies, std::vector<ContactManifold*>&);
	};

}