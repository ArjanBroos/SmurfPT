#include <SFML/Graphics.hpp>
#include "../core/geometry.h"
#include "../core/camera.h"
#include "../core/sphere.h"
#include "../core/color.h"
#include "../core/world.h"
#include "../core/triangle.h"
#include "../core/tracer.h"
#include <random>
#include <ctime>
#include <sstream>
#include <fstream>

unsigned w = 1280;
unsigned h = 720;

World world;
Camera camera(w, h);
sf::Image image;
sf::Sprite sprite;
unsigned iteration = 1;

std::uniform_real_distribution<> urd;
std::mt19937 mt((unsigned)time(0));

void HandleEvents(sf::RenderWindow& window);
void Render(sf::RenderWindow& window);
void ClearImage();
void TraceRays(unsigned maxIterations, sf::Texture& texture);
Color TraceRay(const Ray& ray, unsigned depth);
Vector RandomDirection(const Normal& n);

int main() {
	sf::RenderWindow window(sf::VideoMode(w, h), "SmurfPT");
	sf::Texture texture;

	Sphere sphere1(Color(1.f, 0.f, 0.f));
	sphere1.center = Point(5.f, 1.f, 5.f);
	sphere1.radius = 3.f;
	world.AddShape(&sphere1);
	Sphere sphere2(Color(0.f, 0.f, 1.f));
	sphere2.center = Point(1.f, 1.f, 4.f);
	sphere2.radius = 1.f;
	world.AddShape(&sphere2);
	Sphere sphere3(Color(0.f, 1.f, 1.f));
	sphere3.center = Point(5.f, 3.f, -5.f);
	sphere3.radius = 1.f;
	world.AddShape(&sphere3);
	Sphere sphere4(Color(1.f, 1.f, 0.f));
	sphere4.center = Point(-5.f, 1.f, -5.f);
	sphere4.radius = 2.f;
	world.AddShape(&sphere4);
	Triangle triangle;
	triangle.color = Color(0.f, 1.f, 0.f);
	triangle.p1 = Point(-300.f, 0.f, 300.f);
	triangle.p2 = Point(0.f, 0.f, -100.f);
	triangle.p3 = Point(300.f, 0.f, 300.f);
	world.AddShape(&triangle);

	Sphere light(Color(0.f, 0.f, 0.f));
	light.center = Point(1000.f, 1020.f, 0.f);
	light.radius = 1000.f;
	light.emittance = Color(1.f, 1.f, 1.f);
	world.AddShape(&light);

	camera.position = Point(0.f, 20, -20.f);
	camera.direction = Normalize(Vector(0.f, -1.f, 1.f));
	camera.up = Normalize(Vector(0.f, 1.f, 1.f));
	camera.right = Normalize(Vector(1.f, 0.f, 0.f));

	image.create(camera.film.GetWidth(), camera.film.GetHeight());
	texture.create(camera.film.GetWidth(), camera.film.GetHeight());
	sprite.setTexture(texture);

	while(window.isOpen()) {
		HandleEvents(window);

		const unsigned nSamples = 500;
		if (iteration <= nSamples) {

			std::stringstream ss;
			ss << iteration;
			std::string caption = "Tracer - Iteration: " + ss.str();

			window.setTitle(caption);
			TraceRays(nSamples, texture);
			iteration++;
			Render(window);
		}
	}

	return 0;
}

void HandleEvents(sf::RenderWindow& window) {
	float cameraStep = 0.2f;
	sf::Event e;
	while (window.pollEvent(e)) {
		if (e.type == sf::Event::Closed)
			window.close();
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
			window.close();
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::A) {
			camera.MoveLeft(cameraStep);
			camera.film.Clear();
			iteration = 1;
		}
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::D) {
			camera.MoveRight(cameraStep);
			camera.film.Clear();
			iteration = 1;
		}
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::W) {
			camera.MoveForward(cameraStep);
			camera.film.Clear();
			iteration = 1;
		}
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::S) {
			camera.MoveBackward(cameraStep);
			camera.film.Clear();
			iteration = 1;
		}
		if (e.type == sf::Event::Resized) {
			Render(window);
		}
	}
}

void Render(sf::RenderWindow& window) {
	window.clear();
	window.draw(sprite);
	window.display();
}

void ClearImage() {
	for (unsigned y = 0; y < camera.film.GetHeight(); y++) {
		for (unsigned x = 0; x < camera.film.GetWidth(); x++) {
			image.setPixel(x, y, sf::Color(0, 0, 0));
		}
	}
}

Vector UniformSample(const Normal& n) {
	Vector vn(n.x, n.y, n.z);
	Vector t, b;
	CoordinateSystem(vn, &t, &b);
	float u1 = (float)urd(mt);
	float u2 = (float)urd(mt);

	float r = sqrtf(1.0f - u1*u1);
	float phi = u2 * 2.f * PI;

	Vector v(cosf(phi) * r, u1, sinf(phi) * r);
	return t * v.x + vn * v.y + b * v.z;
}

Vector CosineSample(const Normal& n) {
	Vector vn(n.x, n.y, n.z);
	Vector t, b;
	CoordinateSystem(vn, &t, &b);
	float u1 = (float)urd(mt);
	float u2 = (float)urd(mt);

	float r = sqrtf(u1);
	float theta = 2.f * PI * u2;

	Vector v(cosf(theta) * r, sinf(theta) * r, sqrtf(std::max(0.f, 1.f - u1)));
	return t * v.x + vn * v.y + b * v.z;
}

Color TraceRay(const Ray& ray, unsigned depth) {
	const unsigned maxDepth = 1;
	if (depth > maxDepth)
		return Color(0.f, 0.f, 0.f);

	float t;
	Shape* shape = NULL;
	if (!world.Intersect(ray, t, &shape))
		return Color(0.f, 0.f, 0.f);

	Point p = ray(t);
	Normal n = shape->GetNormal(p);

	Vector newDir = UniformSample(n);
	Ray newRay(p, newDir, 0.001f);
	return TraceRay(newRay, depth+1) * Dot(n, newDir) * shape->color + shape->emittance;
}

void TraceRays(unsigned maxIterations, sf::Texture& texture) {
	for (unsigned y = 0; y < camera.film.GetHeight(); y++) {
		for (unsigned x = 0; x < camera.film.GetWidth(); x++) {
			Ray ray = camera.GetRay(x, y);
			Color c = camera.film.GetPixel(x, y);
			Color l = TraceRay(ray, 0);
			camera.film.SetPixel(x, y, l + c);
		}
	}

	for (unsigned y = 0; y < camera.film.GetHeight(); y++) {
		for (unsigned x = 0; x < camera.film.GetWidth(); x++) {
			Color c = camera.film.GetPixel(x,y);
			c /= (float)iteration;
			image.setPixel(x, y, c.ToSFMLColor());
		}
	}
	texture.update(image);
}