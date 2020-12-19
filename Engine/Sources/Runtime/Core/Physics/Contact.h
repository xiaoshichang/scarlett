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
		Vector3f globalPositionA;	// ȫ�������£�����A�Ĵ�͸�������
		Vector3f globalPositionB;	// ȫ�������£�����B�Ĵ�͸�������
		Vector3f localPositionA;	// A�ľֲ������£�����A�Ĵ�͸�������
		Vector3f localPositionB;	// A�ľֲ������£�����B�Ĵ�͸�������

		// these 3 vectors form an orthonormal basis
		Vector3f normal;	// ��͸���ߣ������׷ֿ���δ�͸
		Vector3f tangent1, tangent2;	// ������������

		Vector3f rA;	// A��A��͸�������
		Vector3f rB;	// B��B��͸�������

		// penetration depth
		float penetrationDistance;	// ��͸���

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

		// AB���������ײ��
		int contactPointCount = 0;
		ContactPoint contactPoints[CONTACT_POINT_COUNT];

		void AddContact(ContactPoint point);
		void UpdateContacts();

	};

}