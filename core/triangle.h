#pragma once

#include "geometry.h"
#include "color.h"
#include "shape.h"

class Triangle : public Shape {
public:
	Point p1, p2, p3;

	bool Intersect(const Ray& ray, float& t) const;
	Normal GetNormal(const Point& p) const;
};