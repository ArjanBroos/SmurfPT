#pragma once

#include "geometry.h"
#include "film.h"
#include <ctime>
#include <random>

//! A camera from which we can view the world
class Camera {
public:
	Camera() : dfilm(5.f), film(400, 400),
		up(Vector(0.f, 1.f, 0.f)), right(Vector(1.f, 0.f, 0.f)),
		midx(200.f), midy(200.f), mt((unsigned)time(0)) {}
	Camera(unsigned filmWidth, unsigned filmHeight) : dfilm((float)(filmWidth)/80.f), film(filmWidth, filmHeight),
		up(Vector(0.f, 1.f, 0.f)), right(Vector(1.f, 0.f, 0.f)),
		midx((float)(filmWidth)/2.f), midy((float)(filmHeight)/2.f), mt((unsigned)time(0)) {}
	Point position;
	Vector direction;
	Film film;
	Vector up, right;

	Ray GetRay(unsigned x, unsigned y);
	Ray GetJitteredRay(unsigned x, unsigned y);
	Ray GetJitteredSubRay(unsigned x, unsigned y, int subx, int suby);

	void MoveLeft(float d);
	void MoveRight(float d);
	void MoveForward(float d);
	void MoveBackward(float d);

private:
	std::uniform_real_distribution<> urd;
	std::mt19937 mt;
	float dfilm; // Distance pinhole and film
	float midx, midy;
	Point filmCenter;
};