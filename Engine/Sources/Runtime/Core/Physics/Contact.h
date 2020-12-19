#pragma once
#include "Runtime/Core/Math/ScltMath.h"
#include "Runtime/Core/Physics/Collider.h"
#include "Runtime/Core/Physics/Jacobian.h"

#define CONTACT_POINT_COUNT 5
#define CONTACT_DRIFTING_THRESHOLD  0.004f

namespace scarlett
{
	class RigidBodyComponent;

	class ContactPoint
	{
	public:
		ContactPoint(void)
			: normalImpulseSum(0.0f)
			, tangentImpulseSum1(0.0f)
			, tangentImpulseSum2(0.0f)
			, m_jN(JacobianType::Normal)
			, m_jT(JacobianType::Tangent)
			, m_jB(JacobianType::Tangent)
		{ 
		}

	public:
		// contact point data
		Vector3f globalPositionA;	// 全局坐标下，物体A的穿透点的坐标
		Vector3f globalPositionB;	// 全局坐标下，物体B的穿透点的坐标
		Vector3f localPositionA;	// A的局部坐标下，物体A的穿透点的坐标
		Vector3f localPositionB;	// A的局部坐标下，物体B的穿透点的坐标

		// these 3 vectors form an orthonormal basis
		Vector3f normal;	// 穿透法线，最容易分开这次穿透
		Vector3f tangent1, tangent2;	// 两个切线向量

		Vector3f rA;	// A到A穿透点的向量
		Vector3f rB;	// B到B穿透点的向量

		// penetration depth
		float penetrationDistance;	// 穿透深度

		// for clamping (more on this later)
		float normalImpulseSum;
		float tangentImpulseSum1;
		float tangentImpulseSum2;

		Jacobian m_jN;
		Jacobian m_jT;
		Jacobian m_jB;

	};

	struct ContactManifold
	{
		RigidBodyComponent* colliderA;
		RigidBodyComponent* colliderB;

		// AB间的所有碰撞点
		int contactPointCount = 0;
		ContactPoint contactPoints[CONTACT_POINT_COUNT];

		void AddContact(ContactPoint point);
		void UpdateContacts();

	};

}