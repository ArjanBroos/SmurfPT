#pragma once

#include <cassert>
#include <cfloat>
#include <cmath>
#include <algorithm>

#define isnan _isnan
#define isinf(f) (!_finite((f)))
#define INFINITY FLT_MAX
#define PI 3.141592654f

// Global inline definitions
inline float Lerp(float t, float v1, float v2) {
	return v1 + t * (v2 - v1);
}