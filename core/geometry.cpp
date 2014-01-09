#include "geometry.h"

inline Vector::Vector(const Normal& n) : x(n.x), y(n.y), z(n.z) {
}

//! Returns true if one of the coordinates is NaN (Not a Number)
bool Vector::HasNaNs() {
	return isnan(x) || isnan(y) || isnan(z);
}

float Vector::operator[](unsigned i) const {
	assert(i <= 2);
	return (&x)[i];
}

float& Vector::operator[](unsigned i) {
	assert(i <= 2);
	return (&x)[i];
}

float Vector::LengthSquared() const {
	return x * x + y * y + z * z;
}

float Vector::Length() const {
	return sqrtf(LengthSquared());
}

Vector Vector::operator+(const Vector& v) const {
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector& Vector::operator+=(const Vector& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector Vector::operator-(const Vector& v) const {
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector& Vector::operator-=(const Vector& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector Vector::operator*(float f) const {
	return Vector(x*f, y*f, z*f);
}

Vector& Vector::operator*=(float f) {
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Vector Vector::operator/(float f) const {
	assert(f != 0.f);
	float inv = 1.f / f;
	return Vector(x*inv, y*inv, z*inv);
}

Vector& Vector::operator/=(float f) {
	assert(f != 0.f);
	float inv = 1.f / f;
	x *= inv;
	y *= inv;
	z *= inv;
	return *this;
}

Vector Vector::operator-() const {
	return Vector(-x, -y, -z);
}

Point Point::operator+(const Vector& v) const {
	return Point(x + v.x, y + v.y, z + v.z);
}

Point& Point::operator+=(const Vector& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

float Point::operator[](unsigned i) const {
	assert(i <= 2);
	return (&x)[i];
}

float& Point::operator[](unsigned i) {
	assert(i <= 2);
	return (&x)[i];
}

Point Point::operator-(const Vector& v) const {
	return Point(x - v.x, y - v.y, z - v.z);
}

Vector Point::operator-(const Point& p) const {
	return Vector(x - p.x, y - p.y, z - p.z);
}

Point& Point::operator-=(const Vector& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Point Point::operator+(const Point& p) const {
	return Point(x + p.x, y + p.y, z + p.z);
}

Point& Point::operator+=(const Point& p) {
	x += p.x;
	y += p.y;
	z += p.z;
	return *this;
}

Point Point::operator/(float f) const {
	assert(f != 0.f);
	return Point(x / f, y / f, z / f);
}

Point& Point::operator/=(float f) {
	assert(f != 0.f);
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

Point Point::operator*(float f) const {
	return Point(x * f, y * f, z * f);
}

Point& Point::operator*=(float f) {
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

float Normal::operator[](unsigned i) const {
	assert(i <= 2);
	return (&x)[i];
}

float& Normal::operator[](unsigned i) {
	assert(i <= 2);
	return (&x)[i];
}

float Normal::LengthSquared() const {
	return x * x + y * y + z * z;
}

float Normal::Length() const {
	return sqrtf(LengthSquared());
}

Normal Normal::operator+(const Normal& v) const {
	return Normal(x + v.x, y + v.y, z + v.z);
}

Normal& Normal::operator+=(const Normal& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Normal Normal::operator-(const Normal& v) const {
	return Normal(x - v.x, y - v.y, z - v.z);
}

Normal& Normal::operator-=(const Normal& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Normal Normal::operator*(float f) const {
	return Normal(x*f, y*f, z*f);
}

Normal& Normal::operator*=(float f) {
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Normal Normal::operator/(float f) const {
	assert(f != 0.f);
	float inv = 1.f / f;
	return Normal(x*inv, y*inv, z*inv);
}

Normal& Normal::operator/=(float f) {
	assert(f != 0.f);
	float inv = 1.f / f;
	x *= inv;
	y *= inv;
	z *= inv;
	return *this;
}

Normal Normal::operator-() const {
	return Normal(-x, -y, -z);
}

Point Ray::operator()(float t) const {
	return o + d * t;
}

void RayDifferential::ScaleDifferentials(float s) {
	rxOrigin = o + (rxOrigin - o) * s;
	ryOrigin = o + (ryOrigin - o) * s;
	rxDirection = d + (rxDirection - d) * s;
	ryDirection = d + (ryDirection - d) * s;
}

Point BBox::operator[](unsigned i) const {
	assert(i <= 1);
	return (&pMin)[i];
}

Point& BBox::operator[](unsigned i) {
	assert(i <= 1);
	return (&pMin)[i];
}

//! Returns a bounding box enclosing both b and p
BBox BBox::Union(const BBox& b, const Point& p) const {
	BBox ret;
	ret.pMin.x = std::min(b.pMin.x, p.x);
	ret.pMax.x = std::max(b.pMax.x, p.x);
	ret.pMin.y = std::min(b.pMin.y, p.y);
	ret.pMax.y = std::max(b.pMax.y, p.y);
	ret.pMin.z = std::min(b.pMin.z, p.z);
	ret.pMax.z = std::max(b.pMax.z, p.z);
	return ret;
}

//! Returns a bounding box enclosing both b1 and b2
BBox BBox::Union(const BBox& b1, const BBox& b2) const {
	BBox ret;
	ret.pMin.x = std::min(b1.pMin.x, b2.pMin.x);
	ret.pMax.x = std::max(b1.pMax.x, b2.pMax.x);
	ret.pMin.y = std::min(b1.pMin.y, b2.pMin.y);
	ret.pMax.y = std::max(b1.pMax.y, b2.pMax.y);
	ret.pMin.z = std::min(b1.pMin.z, b2.pMin.z);
	ret.pMax.z = std::max(b1.pMax.z, b2.pMax.z);
	return ret;
}

//! Returns true if this box overlaps with b
bool BBox::Overlaps(const BBox& b) const {
	if (pMin.x > b.pMax.x || pMax.x < b.pMin.x) return false;
	if (pMin.y > b.pMax.y || pMax.y < b.pMax.y) return false;
	if (pMin.z > b.pMax.z || pMax.z < b.pMax.z) return false;
	return true;
}

//! Returns true if p is inside this box
bool BBox::Inside(const Point& p) const {
	return (pMin.x <= p.x && p.x <= pMax.x &&
			pMin.y <= p.y && p.y <= pMax.y &&
			pMin.z <= p.z && p.z <= pMax.z);
}

//! Returns the area of the 6 surfaces of this box
float BBox::SurfaceArea() const {
	Vector d = pMax - pMin;
	return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
}

//! Returns the volume of this box
float BBox::Volume() const {
	Vector d = pMax - pMin;
	return d.x * d.y * d.z;
}

//! Returns the longest side of the box
//! 0 = x, 1 = y, 2 = z
unsigned BBox::MaximumExtent() const {
	Vector d = pMax - pMin;
	if (d.x > d.y && d.x > d.z)
		return 0;
	else if (d.y > d.z)
		return 1;
	else
		return 2;
}

//! Returns a point interpolated along the axes of the bounding box
Point BBox::Lerp(float tx, float ty, float tz) const {
	return Point(::Lerp(tx, pMin.x, pMax.x), ::Lerp(ty, pMin.y, pMax.y),
		::Lerp(tz, pMin.z, pMax.z));
}

//! Returns a vector with the position of p relative to the corners of this box
//! (0, 0, 0) means p = pMin, (1, 1, 1) means p = pMax
Vector BBox::Offset(const Point& p) const {
	return Vector( (p.x - pMin.x) / (pMax.x - pMin.x),
		(p.y - pMin.y) / (pMax.y - pMin.y),
		(p.z - pMin.z) / (pMax.z - pMin.z) );
}

//! Returns the center and radius of a sphere that bounds the bounding box
void BBox::BoundingSphere(Point* center, float* radius) const {
	assert(center && radius);
	*center = pMin * .5f + pMax * .5f;
	*radius = Inside(*center) ? Distance(*center, pMax) : 0.f;
}

//! Expands the box by adding delta padding
void BBox::Expand(float delta) {
	pMin -= Vector(delta, delta, delta);
	pMax += Vector(delta, delta, delta);
}