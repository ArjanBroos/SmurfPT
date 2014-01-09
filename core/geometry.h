#pragma once

#include "tracer.h"

class Point;
class Normal;
class Ray;

//! A 3D vector
class Vector {
public:
	// Direction
	float x, y, z;

	// Constructors
	Vector() : x(0.f), y(0.f), z(0.f) {}
	Vector(float x, float y, float z) : x(x), y(y), z(z) {
		assert(!HasNaNs());
	}
	explicit Vector(const Normal& n);

	bool HasNaNs();
	float operator[](unsigned i) const;
	float& operator[](unsigned i);
	float LengthSquared() const;
	float Length() const;

	// Arithmetic
	Vector operator+(const Vector& v) const;
	Vector& operator+=(const Vector& v);
	Vector operator-(const Vector& v) const;
	Vector& operator-=(const Vector& v);
	Vector operator*(float f) const;
	Vector& operator*=(float f);
	Vector operator/(float f) const;
	Vector& operator/=(float f);
	Vector operator-() const;
};

//! Represents a point in 3D space
class Point {
public:
	// Coordinates
	float x, y, z;

	// Constructors
	Point() : x(0.f), y(0.f), z(0.f) {}
	Point(float x, float y, float z) : x(x), y(y), z(z) {}

	float operator[](unsigned i) const;
	float& operator[](unsigned i);

	// Arithmetic
	Point operator+(const Vector& v) const;
	Point& operator+=(const Vector& v);
	Point operator-(const Vector& v) const;
	Vector operator-(const Point& p) const;
	Point& operator-=(const Vector& v);
	Point operator+(const Point& p) const;
	Point& operator+=(const Point& p);
	Point operator/(float f) const;
	Point& operator/=(float f);
	Point operator*(float f) const;
	Point& operator*=(float f);
};

//! Represents the normal of a surface
class Normal {
public:
	// Direction
	float x, y, z;

	// Constructors
	Normal() : x(0.f), y(0.f), z(0.f) {}
	Normal(float x, float y, float z) : x(x), y(y), z(z) {}
	explicit Normal(const Vector& v) : x(v.x), y(v.y), z(v.z) {}

	float operator[](unsigned i) const;
	float& operator[](unsigned i);
	float LengthSquared() const;
	float Length() const;

	// Arithmetic
	Normal operator+(const Normal& v) const;
	Normal& operator+=(const Normal& v);
	Normal operator-(const Normal& v) const;
	Normal& operator-=(const Normal& v);
	Normal operator*(float f) const;
	Normal& operator*=(float f);
	Normal operator/(float f) const;
	Normal& operator/=(float f);
	Normal operator-() const;
};

//! Represents a 3D ray
class Ray {
public:
	Point o; // Origin
	Vector d; // Direction
	mutable float mint, maxt; // Ray(t) with t in [mint, maxt]. Ray segment instead of infinite ray
	float time; // For motion blur effects, the time at which a ray was cast
	unsigned depth; // For terminating light-paths after number of bounces

	Ray() : mint(0.f), maxt(INFINITY), time(0.f), depth(0) {}
	Ray(const Point& origin, const Vector& direction, float start,
		float end = INFINITY, float time = 0.f, unsigned depth = 0)
		: o(origin), d(direction), mint(start), maxt(end), time(time), depth(depth) {}
	Ray(const Point& origin, const Vector& direction, const Ray& parent,
		float start, float end = INFINITY)
		: o(origin), d(direction), mint(start), maxt(end), time(parent.time), depth(parent.depth+1) {}

	Point operator()(float t) const;
};

//! A 3D ray with extra information for use with texturing and antialiasing
class RayDifferential : public Ray {
public:
	bool hasDifferentials;
	Point rxOrigin, ryOrigin;
	Vector rxDirection, ryDirection;

	RayDifferential() : hasDifferentials(false) {}
	RayDifferential(const Point& origin, const Vector& direction, float start,
		float end = INFINITY, float time = 0.f, unsigned depth = 0)
		: Ray(origin, direction, start, end, time, depth) {
			hasDifferentials = false;
	}
	RayDifferential(const Point& origin, const Vector& direction, const Ray& parent,
		float start, float end = INFINITY)
		: Ray(origin, direction, parent, start, end) {
			hasDifferentials = false;
	}
	explicit RayDifferential(const Ray& ray) : Ray(ray) {
		hasDifferentials = false;
	}

	void ScaleDifferentials(float s);
};

//! Represents a bounding box
class BBox {
public:
	Point pMin, pMax; // Opposite corners

	BBox() {
		pMin = Point(INFINITY, INFINITY, INFINITY);
		pMax = Point(-INFINITY, -INFINITY, -INFINITY);
	}
	BBox(const Point& p) : pMin(p), pMax(p) {}
	BBox(const Point& p1, const Point& p2) {
		pMin = Point(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
		pMax = Point(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
	}

	Point operator[](unsigned i) const;
	Point& operator[](unsigned i);

	BBox Union(const BBox& b, const Point& p) const;
	BBox Union(const BBox& b1, const BBox& b2) const;
	bool Overlaps(const BBox& b) const;
	bool Inside(const Point& p) const;
	float SurfaceArea() const;
	float Volume() const;
	unsigned MaximumExtent() const;
	Point Lerp(float tx, float ty, float tz) const;
	Vector Offset(const Point& p) const;
	void BoundingSphere(Point* center, float* radius) const;

	void Expand(float delta);
};

// Geometry inline functions

inline Normal operator*(float f, const Normal& n) {
	return n*f;
}

inline Vector operator*(float f, const Vector& v) {
	return v*f;
}

inline float Dot(const Vector& v1, const Vector& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float Dot(const Normal& n1, const Normal& n2) {
	return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
}

inline float Dot(const Normal& n, const Vector& v) {
	return n.x * v.x + n.y * v.y + n.z * v.z;
}

inline float Dot(const Vector& v, const Normal& n) {
	return v.x * n.x + v.y * n.y + v.z * n.z;
}

inline float AbsDot(const Vector& v1, const Vector& v2) {
	return fabsf(Dot(v1, v2));
}

inline float AbsDot(const Normal& n1, const Normal& n2) {
	return fabsf(Dot(n1, n2));
}

inline float AbsDot(const Normal& n, const Vector& v) {
	return fabsf(Dot(n, v));
}

inline float AbsDot(const Vector& v, const Normal& n) {
	return fabsf(Dot(v, n));
}

inline Vector Cross(const Vector& v1, const Vector& v2) {
	return Vector(	v1.y * v2.z - v1.z * v2.y,
					v1.z * v2.x - v1.x * v2.z,
					v1.x * v2.y - v1.y * v2.x);
}

inline Vector Normalize(const Vector& v) {
	return v / v.Length();
}

inline Normal Normalize(const Normal& n) {
	return n / n.Length();
}

//! Generate a coordinate system from a single vector
//! v1 should be a unit vector
inline void CoordinateSystem(const Vector& v1, Vector* v2, Vector* v3) {
	assert(fabsf(v1.Length() - 1.f) < 0.001f);
	assert(v2 && v3);
	if (fabsf(v1.x) > fabsf(v1.y)) {
		float invLen = 1.f / sqrtf(v1.x * v1.x + v1.z * v1.z);
		*v2 = Vector(-v1.z * invLen, 0.f, v1.x * invLen);
	}
	else {
		float invLen = 1.f / sqrtf(v1.y * v1.y + v1.z * v1.z);
		*v2 = Vector(0.f, v1.z * invLen, -v1.y * invLen);
	}
	*v3 = Cross(v1, *v2);
}

inline float Distance(const Point& p1, const Point& p2) {
	return (p1 - p2).Length();
}

inline float DistanceSquared(const Point& p1, const Point& p2) {
	return (p1 - p2).LengthSquared();
}

//! Return n facing the same hemisphere as v
inline Normal FaceForward(const Normal& n, const Vector& v) {
	return (Dot(n, v) < 0.f) ? -n : n;
}

//! Return n1 facing the same hemisphere as n2
inline Normal FaceForward(const Normal& n1, const Normal& n2) {
	return (Dot(n1, n2) < 0.f) ? -n1 : n1;
}

//! Return v facing the same hemisphere as n
inline Vector FaceForward(const Vector& v, const Normal& n) {
	return (Dot(v, n) < 0.f) ? -v : v;
}

//! Return v1 facing the same hemisphere as v2
inline Vector FaceForward(const Vector& v1, const Vector& v2) {
	return (Dot(v1, v2) < 0.f) ? -v1 : v1;
}