#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "scene.h"
#include "image.h"

void imageSave(FILE *out, ImageData *img) {
	fprintf(out, "P6 %d %d 255\n", img->width, img->height);

	fwrite(img->pixels, sizeof(Color), img->width*img->height, out);
}

#define WIDTH 800
#define HEIGHT 600

Scene scene = (Scene) {
	.shapeCount = 3,
	.shapes = (Shape []) {
		{
			.sphere = {
				.type = SHAPE_SPHERE,
				.pos = {0.5,0.5,0.166667},
				.radius = 0.166667,
			}
		},
		{
			.sphere = {
				.type = SHAPE_SPHERE,
				.pos = {0.8333333,0.5,0.5},
				.radius = 0.166667,
			}
		},
		{
			.sphere = {
				.type = SHAPE_SPHERE,
				.pos = {0.333333,0.666667,0.666667},
				.radius = 0.333333,
			}
		},
	},

	.lightCount = 1,
	.lights = (Light []) {
		{
			.pointLight = {
				.type = LIGHT_POINT_SOURCE,
				.pos = { 0.0, 1.0, -0.5 },
				.lightColor = { 255, 255, 255 },
			}
		},
	},
};

int main(int argc, char **argv) {
	ImageData *img;

	img = imageAlloc(WIDTH, HEIGHT);

	cameraInit(&scene.camera, &((vec3) {0.5, 0.5, -1}), &((vec3) {0.5, 0.5, 0}), &((vec3) {0, 1, 0}), 1, 1);
	sceneRender(img, &scene);

	FILE *out;
	out = fopen("raycast.ppm", "w");
	imageSave(out, img);
	fclose(out);

	imageFree(img);
}

