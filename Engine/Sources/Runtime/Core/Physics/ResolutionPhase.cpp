#include "ResolutionPhase.h"

void scarlett::ResolutionPhaseConstraintBase::resolve(std::vector<ContactManifold*>& manifolds)
{
	for each (ContactManifold* manifold in manifolds)
	{
		for (int i = 0; i < manifold->contactPointCount; i++)
		{
			SolveContactConstranst(manifold->contactPoints[i]);
		}
	}
}

void scarlett::ResolutionPhaseConstraintBase::SolveContactConstranst(ContactPoint & contact)
{

}
