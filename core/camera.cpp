#include "camera.h"

Ray Camera::GetRay(unsigned x, unsigned y) {
	assert(x <= film.GetWidth() && y <= film.GetHeight());
	filmCenter = position + dfilm * direction;
	float dx = (x - midx) * 0.01f;
	float dy = (y - midy) * 0.01f;
	Point p(filmCenter + dx * right + dy * -up);
	return Ray(position, p - position, 0.000001f);
}

Ray Camera::GetJitteredRay(unsigned x, unsigned y) {
	assert(x <= film.GetWidth() && y <= film.GetHeight());
	filmCenter = position + dfilm * direction;
	float dx = (x - midx) * 0.01f + urd(mt) * 0.01f;
	float dy = (y - midy) * 0.01f + urd(mt) * 0.01f;	
	Point p(filmCenter + dx * right + dy * -up);
	return Ray(position, p - position, 0.000001f);
}

Ray Camera::GetJitteredSubRay(unsigned x, unsigned y, int subx, int suby) {
	assert(x <= film.GetWidth() && y <= film.GetHeight());
	filmCenter = position + dfilm * direction;
	float dx = (x - midx) * 0.01f;
	float dy = (y - midy) * 0.01f;

	float minx = (float)(subx - 1) * 0.005f + dx;
	float maxx = (float)(subx) * 0.005f + dx;
	float miny = (float)(suby - 1) * 0.005f + dy;
	float maxy = (float)(suby) * 0.005f + dy;

	dx = minx + (float)urd(mt) * (maxx - minx);
	dy = miny + (float)urd(mt) * (maxy - miny);

	Point p(filmCenter + dx * right + dy * -up);
	return Ray(position, p - position, 0.000001f);
}

void Camera::MoveLeft(float d) {
	Vector down(0.f, -1.f, 0.f);
	Vector left(Cross(down, direction));
	position += d * left;
}

void Camera::MoveRight(float d) {
	Vector down(0.f, -1.f, 0.f);
	Vector right(Cross(direction, down));
	position += d * right;
}

void Camera::MoveForward(float d) {
	position += d * direction;
}

void Camera::MoveBackward(float d) {
	position -= d * direction;
}