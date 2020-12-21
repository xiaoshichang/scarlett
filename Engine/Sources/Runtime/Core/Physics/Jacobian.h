#pragma once
#include <vector>
#include "Runtime/Core/Math/ScltMath.h"


namespace scarlett
{
	class ContactManifold;

	enum JacobianType
	{
		Normal,
		Tangent
	}; 

	struct  Jacobian
	{
		Jacobian(JacobianType jt)
		{
			m_jva = Vector3f(0, 0, 0);
			m_jwa = Vector3f(0, 0, 0);
			m_jvb = Vector3f(0, 0, 0);
			m_jwa = Vector3f(0, 0, 0);

		}

		void Init(std::shared_ptr<ContactManifold> manifold, int idx, JacobianType jt, Vector3f dir, float dt);
		void Solve(std::shared_ptr<ContactManifold> manifold, int idx, Vector3f dir, float dt);


		JacobianType jacobinType;
		Vector3f m_jva;
		Vector3f m_jwa;
		Vector3f m_jvb;
		Vector3f m_jwb;
		float m_bias;
		float m_effectiveMass;
		float m_totalLambda;

	};

}