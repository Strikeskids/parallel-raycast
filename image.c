#include "image.h"
#include <stdlib.h>

ImageData *imageAlloc(int width, int height) {
	ImageData *img;
	
	img = malloc(sizeof(ImageData));

	img->width = width;
	img->height = height;

	img->pixels = malloc(width*height*sizeof(Color));

	return img;
}

void imageFree(ImageData *img) {
	free(img->pixels);
	free(img);
}

#define CLIP_COLOR_COMPONENT(c) c < 0 ? 0 : c > 255 ? 255 : c

void colorClip(Color *c) {
	c->r = CLIP_COLOR_COMPONENT(c->r);
	c->g = CLIP_COLOR_COMPONENT(c->g);
	c->g = CLIP_COLOR_COMPONENT(c->g);
}

void colorMultiply(Color *c, Color *bg) {
	c->r = c->r * bg->r / 255;
	c->g = c->g * bg->g / 255;
	c->b = c->b * bg->b / 255;
	colorClip(c);
}

void colorScale(Color *c, float factor) {
	c->r *= factor;
	c->g *= factor;
	c->b *= factor;
	colorClip(c);
}

void colorAdd(Color *dest, Color *a, Color *b) {
	dest->r = a->r + b->r;
	dest->g = a->g + b->g;
	dest->b = a->b + b->b;
	colorClip(dest);
}

