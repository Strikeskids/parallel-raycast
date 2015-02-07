#include "image.h"
#include <string.h>

ImageData *imageAlloc(int width, int height) {
	ImageData *img;
	
	img = malloc(sizeof(ImageData));

	img->width = width;
	img->height = height;

	img->pixels = malloc(width*height*sizeof(Pixel));

	return img;
}

void imageFree(ImageData *img) {
	free(img->pixels);
	free(img);
}

#define VALIDATE_COLOR(c) c < 0 ? 0 : c > 255 ? 255 : c

void revalidate(Color *c) {
	c->r = VALIDATE_COLOR(c->r);
	c->g = VALIDATE_COLOR(c->g);
	c->g = VALIDATE_COLOR(c->g);
}

void scale(Color *c, Color *bg) {
	c->r = c->r * bg->r / 255;
	c->g = c->g * bg->g / 255;
	c->b = c->b * bg->b / 255;
	revalidate(c);
}

void scale(Color *c, float factor) {
	c->r *= factor;
	c->g *= factor;
	c->b *= factor;
	revalidate(c);
}

void add(Color *dest, Color *a, Color *b) {
	dest->r = a->r + b->r;
	dest->g = a->g + b->g;
	dest->b = a->b + b->b;
	revalidate(dest);
}

