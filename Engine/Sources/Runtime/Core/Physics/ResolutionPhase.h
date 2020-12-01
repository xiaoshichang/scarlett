#pragma once
#include <vector>

#include "Runtime/Core/Physics/Contact.h"



namespace scarlett
{
	class IResolutionPhase
	{
	public:
		virtual void resolve(std::vector<ContactManifold*>& manifolds) = 0;
	};

	class ResolutionPhaseConstraintBase : public IResolutionPhase
	{
	public:
		virtual void resolve(std::vector<ContactManifold*>& manifolds);
		void SolveContactConstranst(ContactPoint& contact);
	};
}