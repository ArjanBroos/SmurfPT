#include "sphere.h"

bool Sphere::Intersect(const Ray& ray, float& t) const {
	Vector v = ray.o - center;
	float a = Dot(ray.d, ray.d);
	float b = Dot(ray.d * 2.f, v);
	float c = Dot(v, v) - radius*radius;
	float d = b*b - 4*a*c;

	if (d < 0.f) // No intersection
		return false;

	float D = sqrtf(d);
	float s1 = (-b + D) / (2 * a);
	float s2 = (-b - D) / (2 * a);

	// Ignore intersections too close to the origin of the ray
	// This is to prevent intersections by rounding errors
	if (s1 < ray.mint && s2 < ray.mint) return false;
	if (s1 < ray.mint) s1 = INFINITY;
	if (s2 < ray.mint) s2 = INFINITY;

	t = std::min(s1, s2);
	return true;
}

Normal Sphere::GetNormal(const Point& p) const {
	return Normal(Normalize(p - center));
}