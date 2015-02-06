#ifndef IMAGE_H
#define IMAGE_H

typedef struct Pixel_struct {
	char r;
	char g;
	char b;
} Pixel;

typedef struct ImageData_struct {
	int width;
	int height;
	Pixel *pixels;
} ImageData;

ImageData *imageAlloc(int width, int height);
void imageFree(ImageData *img);

#endif
