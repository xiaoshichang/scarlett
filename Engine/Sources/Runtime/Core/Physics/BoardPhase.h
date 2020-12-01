#pragma once
#include <vector>

#include "Runtime/Core/Physics/Collider.h"
#include "Runtime/Core/Physics/RigidBody.h"


namespace scarlett
{
	typedef std::pair<RigidBody *, RigidBody *> RigidBodyPair;

	class IBoardPhase
	{
	public:
		virtual void Update(float dt) = 0;
		virtual std::vector<RigidBodyPair>& GeneratePossiblePair(std::vector<RigidBody*>& rigidbodies, std::vector<RigidBodyPair>& candicate) = 0;
		
	};
	

	class BoardPhaseNquared : public IBoardPhase
	{
	public:
		virtual void Update(float dt);
		virtual std::vector<RigidBodyPair>& GeneratePossiblePair(std::vector<RigidBody*>& rigidbodies, std::vector<RigidBodyPair>& candicate);


	};

}