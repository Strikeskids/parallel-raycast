#ifndef IMAGE_H
#define IMAGE_H

typedef struct Color_struct {
	char r;
	char g;
	char b;
} Color;

typedef struct ImageData_struct {
	int width;
	int height;
	Color *pixels;
} ImageData;

ImageData *imageAlloc(int width, int height);
void imageFree(ImageData *img);

void scale(Color *c, Color *by);
void scale(Color *c, float factor);
void add(Color *dest, Color *aColor *b);

#endif

