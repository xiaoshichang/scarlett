#include "Jacobian.h"
#include "Runtime/Core/Physics/Contact.h"
#include "Runtime/Core/Object/Components/RigidBodyComponent.h"
#include "Runtime/Core/Object/Components/TransformComponent.h"


/*
这里的dir是在某个方向上解决约束。1个法线方向和2个切线方向。
法线方向前面定义为A指向B
*/
void scarlett::Jacobian::Init(std::shared_ptr<ContactManifold> manifold, int idx, scarlett::JacobianType jt, Vector3f dir, float dt)
{
	jacobinType = jt;

	m_jva = dir * -1;
	m_jwa = CrossProduct(manifold->contactPoints[idx].rA, dir) * -1;
	m_jvb = dir;
	m_jwb = CrossProduct(manifold->contactPoints[idx].rB, dir);

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
			// 推导方式具体可以看
			// http://allenchou.net/2013/12/game-physics-resolution-contact-constraints/
			Vector3f relativeVelocity = vb + CrossProduct(wb, rb) - va - CrossProduct(wa, ra);
			float closingVelocity = DotProduct(relativeVelocity, dir);
			m_bias = -(beta / dt) * manifold->contactPoints[idx].penetrationDistance + restitution * closingVelocity;
		}
	}
		
	// http://allenchou.net/2013/12/game-physics-constraints-sequential-impulse/
	// https://www.youtube.com/watch?v=pmdYzNF9x34
	// 视频里面有 effectiveMass 的算法。
	// 不过这里的计算依然没理解得太透彻
	auto rigidA = manifold->colliderA->GetRigidBody();
	auto rigidB = manifold->colliderB->GetRigidBody();

	Vector3f rva = TransformVector(rigidA->GetInverseInertia(), m_jwa);
	Vector3f rvb = TransformVector(rigidB->GetInverseInertia(), m_jwb);

	float k =
		rigidA->GetInverseMass() + rigidB->GetInverseMass()
		+ DotProduct(m_jwa, rva)
		+ DotProduct(m_jwb, rvb);

	m_effectiveMass = 1 / k;
	m_totalLambda = 0;


}

void scarlett::Jacobian::Solve(std::shared_ptr<ContactManifold> manifold, int idx, Vector3f dir, float dt)
{
	ContactPoint& point = manifold->contactPoints[idx];

	float jv = DotProduct(m_jva, manifold->colliderA->GetRigidBody()->GetVelocity())
		+ DotProduct(m_jwa, manifold->colliderA->GetRigidBody()->GetAngularVelocity())
		+ DotProduct(m_jvb, manifold->colliderB->GetRigidBody()->GetVelocity())
		+ DotProduct(m_jwb, manifold->colliderB->GetRigidBody()->GetAngularVelocity());

	float lambda = m_effectiveMass * (-(jv + m_bias));
	float oldTotalLambda = m_totalLambda;
	switch (jacobinType)
	{
	case JacobianType::Normal:
		m_totalLambda = std::max(m_totalLambda + lambda, 0.0f);
		break;

	case JacobianType::Tangent:
		float friction = manifold->colliderA->GetFriction() * manifold->colliderB->GetFriction();
		float maxFriction = friction * point.m_jN.m_totalLambda;
		m_totalLambda = Clamp(m_totalLambda + lambda, -maxFriction, maxFriction);
		break;
	}
	lambda = m_totalLambda - oldTotalLambda;
	
	Vector3f va = manifold->colliderA->GetRigidBody()->GetVelocity();
	Vector3f vadelta = m_jva * manifold->colliderA->GetRigidBody()->GetInverseMass() * lambda;
	manifold->colliderA->GetRigidBody()->SetVelocity(va + vadelta);

	Vector3f wa = manifold->colliderA->GetRigidBody()->GetAngularVelocity();
	Vector3f wadelta = TransformVector(manifold->colliderA->GetRigidBody()->GetInverseInertiaWS(), m_jwa) * lambda;
	manifold->colliderA->GetRigidBody()->SetAngularVelocity(wa + wadelta);

	Vector3f vb = manifold->colliderB->GetRigidBody()->GetVelocity();
	Vector3f vbdelta = m_jvb * manifold->colliderB->GetRigidBody()->GetInverseMass() * lambda;
	manifold->colliderB->GetRigidBody()->SetVelocity(vb + vbdelta);

	Vector3f wb = manifold->colliderB->GetRigidBody()->GetAngularVelocity();
	Vector3f wbdelta = TransformVector(manifold->colliderB->GetRigidBody()->GetInverseInertiaWS(), m_jwb) * lambda;
	manifold->colliderB->GetRigidBody()->SetAngularVelocity(wb + wbdelta);

}
