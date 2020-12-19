#include "Jacobian.h"
#include "Runtime/Core/Physics/Contact.h"
#include "Runtime/Core/Object/Components/RigidBodyComponent.h"
#include "Runtime/Core/Object/Components/TransformComponent.h"


/*
这里的dir是在某个方向上解决约束。1个法线方向和2个切线方向。
法线方向前面定义为A指向B
*/
void scarlett::Jacobian::Init(std::shared_ptr<ContactManifold> manifold, int idx, Vector3f dir, float dt)
{
	m_va = dir;
	m_wa = CrossProduct(manifold->contactPoints[idx].rA, dir * -1) * -1;
	m_vb = dir * -1;
	m_wb = CrossProduct(manifold->contactPoints[idx].rB, dir * -1);

	m_bias = 0.0f;

	if (jacobinType == JacobianType::Normal)
	{
		float betaA = manifold->colliderA->GetContactBeta();
		float betaB = manifold->colliderB->GetContactBeta();
		float beta = betaA * betaB;

		float restitutionA = manifold->colliderA->GetRestitution();
		float restitutionB = manifold->colliderB->GetRestitution();
		float restitution = restitutionA * restitutionB;


		Vector3f va = manifold->colliderA->GetRigidBody()->GetVelocity();
		Vector3f wa = manifold->colliderA->GetRigidBody()->GetAngularVelocity();
		Vector3f ra = manifold->contactPoints[idx].rA;

		Vector3f vb = manifold->colliderB->GetRigidBody()->GetVelocity();
		Vector3f wb = manifold->colliderB->GetRigidBody()->GetAngularVelocity();
		Vector3f rb = manifold->contactPoints[idx].rB;

		m_bias = 0;
		if (jacobinType == JacobianType::Normal)
		{
			// 发现方向的约束解决的物理含义为
			// 碰撞双方的相对速度在发现方向上的投影速度为 >= 0，也就是两者不会继续靠近了。
			Vector3f relativeVelocity = va - vb + CrossProduct(wa, ra) - CrossProduct(wb, rb);
			float closingVelocity = DotProduct(relativeVelocity, dir);
			m_bias = -(beta / dt) * manifold->contactPoints[idx].penetrationDistance + restitution * closingVelocity;
		}
		
		// http://allenchou.net/2013/12/game-physics-constraints-sequential-impulse/
		// https://www.youtube.com/watch?v=pmdYzNF9x34
		// 视频里面有 J 得算法。
		// 不过这里的计算依然没理解得太透彻
		auto rigidA = manifold->colliderA->GetRigidBody();
		auto rigidB = manifold->colliderB->GetRigidBody();

		Vector3f rva = TransformVector(rigidA->GetInverseInertia(), wa);
		Vector3f rvb = TransformVector(rigidB->GetInverseInertia(), wb);

		float k =
			rigidA->GetInverseMass() + rigidB->GetInverseMass()
			+ DotProduct(wa, rva)
			+ DotProduct(wb, rvb);

		m_effectiveMass = 1 / k;
		m_totalLambda = 0;

	}

}
