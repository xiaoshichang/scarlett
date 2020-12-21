#include "ResolutionPhase.h"


void scarlett::ResolutionPhaseConstraintBase::Resolve(std::vector<std::shared_ptr<ContactManifold>>& manifolds, float deltaTime)
{
	// 初始化一些相关计算量
	for each (std::shared_ptr<ContactManifold> manifold in manifolds)
	{
		for (int i = 0; i < manifold->contactPointCount; i++)
		{
			InitContactConstranst(manifold, i, deltaTime);
		}
	}

	// 实际解决约束
	for each (std::shared_ptr<ContactManifold> manifold in manifolds)
	{
		for (int i = 0; i < manifold->contactPointCount; i++)
		{
			SolveContactConstranst(manifold, i, deltaTime);
		}
	}
}

void scarlett::ResolutionPhaseConstraintBase::SolveContactConstranst(std::shared_ptr<ContactManifold> manifold, int idx, float deltaTime)
{
	manifold->contactPoints[idx].m_jN.Solve(manifold, idx, manifold->contactPoints[idx].normal, deltaTime);
	manifold->contactPoints[idx].m_jT.Solve(manifold, idx, manifold->contactPoints[idx].tangent1, deltaTime);
	manifold->contactPoints[idx].m_jB.Solve(manifold, idx, manifold->contactPoints[idx].tangent2, deltaTime);
}

void scarlett::ResolutionPhaseConstraintBase::InitContactConstranst(std::shared_ptr<ContactManifold> manifold, int idx, float deltaTime)
{
	manifold->contactPoints[idx].m_jN.Init(manifold, idx, JacobianType::Normal, manifold->contactPoints[idx].normal, deltaTime);
	manifold->contactPoints[idx].m_jT.Init(manifold, idx, JacobianType::Tangent, manifold->contactPoints[idx].tangent1, deltaTime);
	manifold->contactPoints[idx].m_jB.Init(manifold, idx, JacobianType::Tangent, manifold->contactPoints[idx].tangent2, deltaTime);
}
