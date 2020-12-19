#pragma once
#include <vector>
#include "Runtime/Core/Physics/Contact.h"


namespace scarlett
{
	class IResolutionPhase
	{
	public:
		virtual void Resolve(std::vector<std::shared_ptr<ContactManifold>>& manifolds, float deltaTime) = 0;
	};

	class ResolutionPhaseConstraintBase : public IResolutionPhase
	{
	public:
		virtual void Resolve(std::vector<std::shared_ptr<ContactManifold>>& manifolds, float deltaTime);
		void SolveContactConstranst(std::shared_ptr<ContactManifold> manifold, int idx, float deltaTime);
		void InitContactConstranst(std::shared_ptr<ContactManifold> manifold, int idx);
	};
}