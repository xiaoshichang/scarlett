#include "Runtime/Core/Physics/BoardPhase.h"
#include "BoardPhase.h"

using namespace scarlett;

void scarlett::BoardPhaseNquared::Update(float dt)
{
}

std::vector<RigidBodyPair>& scarlett::BoardPhaseNquared::GeneratePossiblePair(std::vector<RigidBody*>& rigidbodies, std::vector<RigidBodyPair>& candicate)
{
	candicate.clear();

	for (int i = 0; i < rigidbodies.size(); i++)
	{
		for (int j = i; j < rigidbodies.size(); j++)
		{
			if (i == j)
			{
				continue;
			}
			candicate.push_back(std::make_pair(rigidbodies[i], rigidbodies[j]));
		}
	}
	return candicate;
}
