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
		return std::max(0.0f, std::min(1.0f, a));
	}

}