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
	sphere1.type = DIFFUSE;
	world.AddShape(&sphere1);
	Sphere sphere2(Color(0.f, 0.f, 1.f));
	sphere2.center = Point(1.f, 1.f, 4.f);
	sphere2.radius = 1.f;
	sphere2.type = DIFFUSE;
	world.AddShape(&sphere2);
	Sphere sphere3(Color(0.f, 1.f, 1.f));
	sphere3.center = Point(5.f, 3.f, -5.f);
	sphere3.radius = 1.f;
	sphere3.type = DIFFUSE;
	world.AddShape(&sphere3);
	Sphere sphere4(Color(1.f, 1.f, 0.f));
	sphere4.center = Point(-5.f, 1.f, -5.f);
	sphere4.radius = 2.f;
	sphere4.type = DIFFUSE;
	world.AddShape(&sphere4);
	Sphere sphere5;
	sphere5.center = Point(-2.5f, 3.f, 4.f);
	sphere5.radius = 2.f;
	sphere5.type = MIRROR;
	world.AddShape(&sphere5);
	Sphere sphere6(Color(1.f, 1.f, 1.f));
	sphere6.center = Point(-6.f, 2.5f, 3.5f);
	sphere6.radius = 3.f;
	sphere6.type = DIFFUSE;
	world.AddShape(&sphere6);
	Sphere sphere7;
	sphere7.center = Point(1.f, 2.f, -6.f);
	sphere7.radius = 2.5f;
	sphere7.type = MIRROR;
	world.AddShape(&sphere7);

	Triangle triangle;
	triangle.color = Color(0.f, 1.f, 0.f);
	triangle.p1 = Point(-300.f, 0.f, 300.f);
	triangle.p2 = Point(0.f, 0.f, -100.f);
	triangle.p3 = Point(300.f, 0.f, 300.f);
	triangle.type = DIFFUSE;
	world.AddShape(&triangle);

	Sphere light2(Color(0.f, 0.f, 0.f));
	light2.center = Point(0.f, 0.f, 0.f);
	light2.radius = 1.5f;
	light2.emittance = Color(10.f, 10.f, 10.f);
	light2.type = DIFFUSE;
	//world.AddShape(&light2);
	Sphere light(Color(0.f, 0.f, 0.f));
	light.center = Point(1000.f, 1020.f, 0.f);
	light.radius = 1000.f;
	light.emittance = Color(1.f, 1.f, 1.f);
	light.type = DIFFUSE;
	//world.AddShape(&light);

	camera.position = Point(0.f, 25.f, -25.f);
	camera.direction = Normalize(Vector(0.f, -1.f, 1.f));
	camera.up = Normalize(Vector(0.f, 1.f, 1.f));
	camera.right = Normalize(Vector(1.f, 0.f, 0.f));

	image.create(camera.film.GetWidth(), camera.film.GetHeight());
	texture.create(camera.film.GetWidth(), camera.film.GetHeight());
	sprite.setTexture(texture);

	while(window.isOpen()) {
		HandleEvents(window);

		const unsigned nSamples = 3000;
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

Vector Reflect(const Normal& n, const Vector& dir) {
	return dir - 2.f * Dot(n, dir) * n;
}

Color TraceRay(const Ray& ray, unsigned depth) {
	const unsigned maxDepth = 4;
	if (depth > maxDepth)
		return Color(0.f, 0.f, 0.f);

	float t;
	Shape* shape = NULL;
	if (!world.Intersect(ray, t, &shape))
		return Color(0.6f, 0.6f, 0.9f);

	Point p = ray(t);
	Normal n = shape->GetNormal(p);
	
	Vector newDir;
	if (shape->type == DIFFUSE) {
		newDir = UniformSample(n);
		Ray newRay(p, newDir, 0.001f);
		return TraceRay(newRay, depth+1) * Dot(n, newDir) * shape->color + shape->emittance;
	}
	else if (shape->type == MIRROR) {
		newDir = Reflect(n, ray.d);
		Ray newRay(p, newDir, 0.00001f);
		return TraceRay(newRay, depth+1);
	}
	
	return Color(0.f, 0.f, 0.f);
}

void TraceRays(unsigned maxIterations, sf::Texture& texture) {
	for (unsigned y = 0; y < camera.film.GetHeight(); y++) {
		for (unsigned x = 0; x < camera.film.GetWidth(); x++) {
			Ray ray = camera.GetJitteredRay(x, y);
			Color l = TraceRay(ray, 0);
			Color c = camera.film.GetPixel(x, y);
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