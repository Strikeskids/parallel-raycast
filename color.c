#include "color.h"

void colorMultiply(Color *c, Color *bg) {
	c->r *= bg->r;
	c->g *= bg->g;
	c->b *= bg->b;
}

void colorScale(Color *c, float factor) {
	c->r *= factor;
	c->g *= factor;
	c->b *= factor;
}

void colorAdd(Color *dest, Color *c1, Color *c2) {
	dest->r = c1->r + c2->r;
	dest->g = c1->g + c2->g;
	dest->b = c1->b + c2->b;
}

void colorSub(Color *dest, Color *c1, Color *c2) {
	dest->r = c1->r - c2->r;
	dest->g = c1->g - c2->g;
	dest->b = c1->b - c2->b;
}

#define COLOR_COMPONENT(f) ((unsigned char) (f<0 ? 0 : (f>1 ? 255 : f*255)))

Pixel colorPack(Color *c) {
	return (Pixel) {COLOR_COMPONENT(c->r), COLOR_COMPONENT(c->g), COLOR_COMPONENT(c->b)};
}

