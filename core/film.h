#pragma once

#include "color.h"

//! Represents the film on which light is projected to form an image
class Film {
public:
	Film(unsigned width, unsigned height) : width(width), height(height) {
		pixels = new Color[width*height];
	}
	~Film() {
		delete pixels;
	}

	unsigned	GetWidth() const { return width; }
	unsigned	GetHeight() const { return height; }

	Color		GetPixel(unsigned x, unsigned y) const { return pixels[y * width + x]; }
	void		SetPixel(unsigned x, unsigned y, const Color& color) { pixels[y * width + x] = color; }
	void		Clear() { for (unsigned i = 0; i < width*height; i++) pixels[i] = Color(); }

private:
	unsigned	width, height; // Dimensions of film in pixels
	Color*		pixels;
};