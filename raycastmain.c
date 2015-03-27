#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "material.h"
#include "scene.h"
#include "image.h"

void imageSave(FILE *out, ImageData *img) {
	fprintf(out, "P6 %d %d 255\n", img->width, img->height);

	fwrite(img->pixels, sizeof(Pixel), img->width*img->height, out);
}

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char **argv) {
	ImageData *img;

	img = imageAlloc(WIDTH, HEIGHT);

	Scene loaded;
	if (sceneLoad(&loaded, "scene2.yml")) {
		fprintf(stderr, "Failed to load\n");
		return 1;
	}

	sceneRender(img, &loaded);

	FILE *out;
	out = fopen("raycast.ppm", "w");
	imageSave(out, img);
	fclose(out);

	imageFree(img);

	return 0;
}

