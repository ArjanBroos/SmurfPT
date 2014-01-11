#pragma once

#include "geometry.h"
#include "color.h"
#include "shape.h"

class Sphere : public Shape {
public:
	Sphere() { this->color = Color(); }
	Sphere(Color color) { this->color = color; }
	Point center;
	float radius;
	
	bool Intersect(const Ray& ray, float& t) const;
	Normal GetNormal(const Point& p) const;
};