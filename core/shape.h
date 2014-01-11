#pragma once

#include "geometry.h"
#include "color.h"

enum ShapeType {
	DIFFUSE,
	MIRROR
};

class Shape {
public:
	Color emittance;
	Color color;
	ShapeType type;
	virtual Normal GetNormal(const Point& p) const = 0;
	virtual bool Intersect(const Ray& ray, float& t) const = 0;
};