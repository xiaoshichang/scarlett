#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include <math.h>
#include <algorithm>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef TWO_PI
#define TWO_PI 3.14159265358979323846f * 2.0f
#endif

namespace scarlett {

	inline float sat(float a) {
		if (a < 0.0f)
		{
			return 0.0f;
		}

		if (a > 1.0f)
		{
			return 1.0f;
		}
		return a;

	}

	inline float Clamp(float v, float a, float b)
	{
		if (v < a)
		{
			return a;
		}

		if (v > b)
		{
			return b;
		}

		return v;
	}

}