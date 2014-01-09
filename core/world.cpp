#include "world.h"

bool World::Intersect(const Ray& ray, float& t, Shape** shape) {
	bool hitOne = false;
	float mint = INFINITY;
	Shape* closest = NULL;
	for (auto i = shapes.begin(); i != shapes.end(); i++) {
		if ((*i)->Intersect(ray, t) && t < mint && t > 0.f) {
			closest = *i;
			mint = t;
			hitOne = true;
		}
	}
	*shape = closest;
	t = mint;
	return hitOne;
}