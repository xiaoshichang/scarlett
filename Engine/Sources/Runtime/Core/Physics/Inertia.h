#pragma once
#include "runtime/Core/Math/ScltMath.h"

namespace scarlett
{

	class Inertia
	{
	public:
		static Matrix3x3f CalculateInertiaSolidCuboid(float mass, float w, float h, float d);
		static Matrix3x3f CalculateInertiaSolidSphere(float mass, float r);
	};
}