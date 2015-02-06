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
