#ifndef COLOR_H
#define COLOR_H

#include "image.h"

typedef struct Color_struct {
	float r;
	float g;
	float b;
} Color;

void colorMultiply(Color *c, Color *by);
void colorScale(Color *c, float factor);
void colorAdd(Color *dest, Color *a, Color *b);
Pixel colorPack(Color *c);

#endif

