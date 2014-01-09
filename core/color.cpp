#include "color.h"
#include "tracer.h"

sf::Color Color::ToSFMLColor() const {
	unsigned char cr = (unsigned char)(r * 255);
	unsigned char cg = (unsigned char)(g * 255);
	unsigned char cb = (unsigned char)(b * 255);
	if (r > 1.f) cr = 255;
	if (g > 1.f) cg = 255;
	if (b > 1.f) cb = 255;
	return sf::Color(cr, cg, cb);
}

Color Color::operator+(const Color& c) const {
	return Color(r + c.r, g + c.g, b + c.b);
}

Color& Color::operator+=(const Color& c) {
	r += c.r;
	g += c.g;
	b += c.b;
	return *this;
}

Color Color::operator-(const Color& c) const {
	return Color(r - c.r, g - c.g, b - c.b);
}

Color& Color::operator-=(const Color& c) {
	r -= c.r;
	g -= c.g;
	b -= c.b;
	return *this;
}

Color Color::operator*(const Color& c) const {
	return Color(r * c.r, g * c.g, b * c.b);
}

Color& Color::operator*=(const Color& c) {
	r *= c.r;
	g *= c.g;
	b *= c.b;
	return *this;
}

Color Color::operator*(float f) const {
	return Color(r * f, g * f, b * f);
}

Color& Color::operator*=(float f) {
	r *= f;
	g *= f;
	b *= f;
	return *this;
}

Color Color::operator/(float f) const {
	assert(f != 0.f);
	return Color(r / f, g / f, b / f);
}

Color& Color::operator/=(float f) {
	assert(f != 0.f);
	r /= f;
	g /= f;
	b /= f;
	return *this;
}

bool Color::operator==(const Color& c) const {
	if (r != c.r) return false;
	if (g != c.g) return false;
	if (b != c.b) return false;
	return true;
}