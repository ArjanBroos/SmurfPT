#include "triangle.h"

bool Triangle::Intersect(const Ray& ray, float& t) const {
	Vector p1p2 = p2 - p1;
	Vector p1p3 = p3 - p1;
	Normal n = Normalize(Normal(Cross(p1p3, p1p2)));
	float nDotRay = Dot(n, ray.d);
	if (Dot(n, ray.d) == 0.f)
		return false; // Ray and triangle parallel

	// Calculate hit point
	float d = Dot(n, Vector(p1.x, p1.y, p1.z));
	float tt = -(Dot(n, Vector(ray.o.x, ray.o.y, ray.o.z)) + d) / nDotRay;
	Point p = ray(t);

	// Test edge 1
	Vector p1p = p - p1;
	float v = Dot(n, Cross(p1p, p1p2));
	if (v < 0.f) return false;

	// Test edge 2
	Vector p2p = p - p2;
	Vector p2p3 = p3 - p2;
	float w = Dot(n, Cross(p2p, p2p3));
	if (w < 0.f) return false;

	// Test edge 3
	Vector p3p = p - p3;
	Vector p3p1 = p1 - p3;
	float u = Dot(n, Cross(p3p, p3p1));
	if (u < 0.f) return false;

	// Ignore intersections too close to the origin of the ray
	// This is to prevent intersections by rounding errors
	if (tt < ray.mint) return false;
	
	t = tt;
	return true;
}

Normal Triangle::GetNormal(const Point& p) const {
	return Normalize(Normal(Cross(p3 - p1, p2 - p1)));
}