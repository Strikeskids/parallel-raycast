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

#endif
