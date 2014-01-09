#pragma once

#include "geometry.h"
#include "color.h"

class Shape {
public:
	Color emittance;
	Color color;
	virtual Normal GetNormal(const Point& p) const = 0;
	virtual bool Intersect(const Ray& ray, float& t) const = 0;
};