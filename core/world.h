#pragma once

#include "geometry.h"
#include <vector>
#include "shape.h"

class World {
public:
	bool Intersect(const Ray& ray, float& t, Shape** shape);

	void AddShape(Shape* shape) { shapes.push_back(shape); }
	
private:
	std::vector<Shape*> shapes;
};