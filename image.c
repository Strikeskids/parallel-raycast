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

#define REPACK_COLOR(c, r, g, b) *c = (Color) {CLIP_COLOR_COMPONENT(r), CLIP_COLOR_COMPONENT(g), CLIP_COLOR_COMPONENT(b)}
#define CLIP_COLOR_COMPONENT(c) c < 0 ? 0 : c > 255 ? 255 : (unsigned char) c

void colorMultiply(Color *c, Color *bg) {
	int r, g, b;
	r = (int) c->r * bg->r / 255;
	g = (int) c->g * bg->g / 255;
	b = (int) c->b * bg->b / 255;
	REPACK_COLOR(c,r,g,b);
}

void colorScale(Color *c, float factor) {
	int r, g, b;
	r = c->r * factor;
	g = c->g * factor;
	b = c->b * factor;
	REPACK_COLOR(c,r,g,b);
}

void colorAdd(Color *dest, Color *c1, Color *c2) {
	int r, g, b;
	r = c1->r + c2->r;
	g = c1->g + c2->g;
	b = c1->b + c2->b;
	REPACK_COLOR(dest,r,g,b);
}

