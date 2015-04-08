#ifndef IMAGE_H
#define IMAGE_H

typedef struct Pixel_struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} Pixel;

typedef struct ImageData_struct {
	int width;
	int height;
	Pixel *pixels;
} ImageData;

ImageData *imageAlloc(int width, int height);
void imageFree(ImageData *img);

#endif

