#pragma once
#include "Runtime/Core/Math/ScltMath.h"
#include "Runtime/Core/Physics/Collider.h"


#define CONTACT_POINT_COUNT 5
#define CONTACT_DRIFTING_THRESHOLD  0.004f

namespace scarlett
{

	class ContactPoint
	{
	public:
		ContactPoint(void)
			: normalImpulseSum(0.0f)
			, tangentImpulseSum1(0.0f)
			, tangentImpulseSum2(0.0f)
		{ }

	public:
		// contact point data
		Vector3f globalPositionA;
		Vector3f globalPositionB;
		Vector3f localPositionA;
		Vector3f localPositionB;

		// these 3 vectors form an orthonormal basis
		Vector3f normal; // points from colliderA to colliderB
		Vector3f tangent1, tangent2;

		// penetration depth
		float penetrationDistance;

		// for clamping (more on this later)
		float normalImpulseSum;
		float tangentImpulseSum1;
		float tangentImpulseSum2;

	};

	struct ContactManifold
	{
		Collider* colliderA;
		Collider* colliderB;

		// AB间的所有碰撞点
		int contactPointCount = 0;
		ContactPoint contactPoints[CONTACT_POINT_COUNT];

		void UpdateContacts();
	};

}