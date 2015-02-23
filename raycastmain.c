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

#define WHITE { 1, 1, 1 }

Scene scene = {
	.objectCount = 4,
	.objects = (SceneObject []) {
		{
			.shape = { .sphere = {
				.type = SHAPE_SPHERE,
				.pos = {0.5,0.5,0.166667},
				.radius = 0.166667,
			} },
			.material = { .constant = {
				.type = MATERIAL_CONSTANT,
				.specularness = 0.5,
				.fresnel = 0.2,
				.roughness = 0.2,
				.diffuse = { 0, 0, 1 },
				.specular = WHITE,
			} },
		},
		{
			.shape = { .sphere = {
				.type = SHAPE_SPHERE,
				.pos = {0.8333333,0.5,0.5},
				.radius = 0.166667,
			} },
			.material = { .constant = {
				.type = MATERIAL_CONSTANT,
				.specularness = 0.5,
				.fresnel = 0.2,
				.roughness = 0.2,
				.diffuse = { 0, 1, 0 },
				.specular = WHITE,
			} },
		},
		{
			.shape = { .sphere = {
				.type = SHAPE_SPHERE,
				.pos = {0.333333,0.666667,0.666667},
				.radius = 0.333333,
			} },
			.material = { .constant = {
				.type = MATERIAL_CONSTANT,
				.specularness = 0.5,
				.fresnel = 0.2,
				.roughness = 0.2,
				.diffuse = { 1, 0, 0 },
				.specular = WHITE,
			} },
		},
		{
			.shape = { .plane = {
				.type = SHAPE_PLANE,
				.pos = {0, 0.333333, 0},
				.norm = {0, 1, 0},
			} },
			.material = { .checkered = {
				.type = MATERIAL_CHECKERED,
				.specularness = 0.5,
				.fresnel = 0.2,
				.roughness = 0.2,
				.checkSize = 0.1,
				.diffuse1 = { 1, 1, 0 },
				.diffuse2 = { 0, 1, 1 },
				.specular1 = WHITE,
				.specular2 = WHITE,
			} },
		},
	},

	.lightCount = 2,
	.lights = (Light []) {
		{
			.pointLight = {
				.type = LIGHT_POINT_SOURCE,
				.pos = { 0.0, 1.0, -0.5 },
				.power = 4,
				.lightColor = { 1, 1, 1 },
			}
		},
		{
			.pointLight = {
				.type = LIGHT_POINT_SOURCE,
				.pos = { 1.0, 1.0, -0.5 },
				.power = 2,
				.lightColor = { 0.8, 0, 1 },
			}
		},
	},
};

int main(int argc, char **argv) {
	ImageData *img;

	img = imageAlloc(WIDTH, HEIGHT);

	cameraInit(&scene.camera, &((vec3) {0.5, 0.5, -1}), &((vec3) {0.5, 0.5, 0}), &((vec3) {0, 1, 0}), 1.0*WIDTH/HEIGHT, 1);
	sceneRender(img, &scene);

	FILE *out;
	out = fopen("raycast.ppm", "w");
	imageSave(out, img);
	fclose(out);

	imageFree(img);
}

