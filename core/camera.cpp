#include "camera.h"

Ray Camera::GetRay(unsigned x, unsigned y) {
	assert(x <= film.GetWidth() && y <= film.GetHeight());
	filmCenter = position + dfilm * direction;
	float dx = (x - midx) * 0.01f;
	float dy = (y - midy) * 0.01f;
	Point p(filmCenter + dx * right + dy * -up);
	return Ray(position, p - position, 0.001f);
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