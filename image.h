#ifndef IMAGE_H
#define IMAGE_H

typedef struct Color_struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} Color;

typedef struct ImageData_struct {
	int width;
	int height;
	Color *pixels;
} ImageData;

ImageData *imageAlloc(int width, int height);
void imageFree(ImageData *img);

void colorMultiply(Color *c, Color *by);
void colorScale(Color *c, float factor);
void colorAdd(Color *dest, Color *a, Color *b);

#endif

