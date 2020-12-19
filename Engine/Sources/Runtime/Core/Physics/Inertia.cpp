#include "Inertia.h"
using namespace scarlett;


// https://en.wikipedia.org/wiki/List_of_moments_of_inertia


Matrix3x3f scarlett::Inertia::CalculateInertiaSolidCuboid(float mass, float w, float h, float d)
{
	float v1 = 1.0 / 12 * mass * (h * h + d * d);
	float v2 = 1.0 / 12 * mass * (w * w + d * d);
	float v3 = 1.0 / 12 * mass * (h * h + w * w);
	Matrix3x3f ret = {{{
		{v1, 0, 0},
		{0, v2, 0},
		{0, 0, v3},
		}}};
	return ret;
}

Matrix3x3f scarlett::Inertia::CalculateInertiaSolidSphere(float mass, float r)
{
	float v1 = 2 / 5 * mass * r * r;
	Matrix3x3f ret = { {{
		{v1, 0, 0},
		{0, v1, 0},
		(0, 0, v1),
		}} };
	return ret;
}
