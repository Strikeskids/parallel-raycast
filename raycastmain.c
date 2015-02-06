#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

ImageData *allocImage(int width, int height) {
	ImageData *img;
	img = malloc(sizeof(ImageData));

	img->width = width;
	img->height = height;

	img->pixels = malloc(width*height*sizeof(Pixel));

	return img;
}

void freeImage(ImageData *img) {
	free(img->pixels);
	free(img);
}

void saveImage(FILE *out, ImageData *img) {
	fprintf(out, "P6 %d %d 255\n", img->width, img->height);

	fwrite(img->pixels, sizeof(Pixel), img->width*img->height, out);
}

#define WIDTH 800
#define HEIGHT 600

float cb(double e, double c, double r) {
	return 2*r*(e-c);
}
float cc(double e, double c, double r) {
	return e*e+c*c-2*e*c;
}

const int cs = 3;
const float cd[][4] = {{0.5,0.5,0.166667,0.166667},{0.8333333,0.5,0.5,0.166667},{0.333333,0.666667,0.666667,0.333333}};
const char ccolors[][3] = {{0,0,255},{0,255,0},{255,0,0}};
const float e[3] = {0.5,0.5,-1};
const float pz=0;

Pixel raycast(float px, float py) {
	int i;
	float rx, ry, rz, rmag;

	rx = px-e[0];
	ry = py-e[1];
	rz = pz-e[2];

	rmag = sqrt(rx*rx + ry*ry + rz*rz);
	rx /= rmag;
	ry /= rmag;
	rz /= rmag;

	float best = INFINITY;
	int bestCoord = -1;

	for (i=0;i<cs;++i) {
		float b = cb(e[0], cd[i][0], rx) + cb(e[1], cd[i][1], ry) + cb(e[2], cd[i][2], rz);
		float c = cc(e[0], cd[i][0], rx) + cc(e[1], cd[i][1], ry) + cc(e[2], cd[i][2], rz) - cd[i][3]*cd[i][3];
		float d = b*b - 4*c;
		if (d < 0) {
			// Outside camera
			continue;
		}
		float distance = (b+sqrt(d))/-2.0;
		if (distance < best) {
			best = distance;
			bestCoord = i;
		}
	}

	Pixel out;

	if (bestCoord >= 0) {
		out = *(Pixel *)(ccolors[bestCoord]);
	} else {
		out.r = 0;
		out.g = 0;
		out.b = 0;
	}
	return out;
}

int main(int argc, char **argv) {
	ImageData *img;

	img = allocImage(WIDTH, HEIGHT);
	
	int r, c;
	for (r=0;r<HEIGHT;++r) {
		for (c=0;c<WIDTH;++c) {
			img->pixels[r*WIDTH+c] = raycast(2.0*(c+0.5)/WIDTH-1, 2.0*(r+0.5)/HEIGHT-1);
		}
	}

	FILE *out;
	out = fopen("raycast.ppm", "w");
	saveImage(out, img);
	fclose(out);

	freeImage(img);
}

