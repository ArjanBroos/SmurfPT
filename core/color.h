#pragma once

#include <SFML/Graphics.hpp>

//! A color with red, green and blue components
class Color {
public:
	float r, g, b; // Red, Green and Blue components. Between 0 and 1.

	Color() : r(0.f), g(0.f), b(0.f) {}
	Color(float r, float g, float b) : r(r), g(g), b(b) {}

	sf::Color ToSFMLColor() const;

	Color operator+(const Color& c) const;
	Color& operator+=(const Color& c);
	Color operator-(const Color& c) const;
	Color& operator-=(const Color& c);
	Color operator*(const Color& c) const;
	Color& operator*=(const Color& c);
	Color operator*(float f) const;
	Color& operator*=(float f);
	Color operator/(float f) const;
	Color& operator/=(float f);
	bool operator==(const Color& c) const;
};

inline Color operator*(float f, const Color& c) {
	return Color(f * c.r, f * c.g, f * c.b);
}