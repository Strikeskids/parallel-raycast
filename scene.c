#include "scene.h"
#include "raytrace.h"
#include <stdlib.h>
#include <stdio.h>

void gatherLight(Color *color, Scene *scene, vec3 *pnt, SceneObject *hit) {
	int i;
	Color result, cur, source;

	result = (Color) {0, 0, 0};
	if (!hit) {
		*color = result;
		return;
	}

	vec3 norm;
	shapeNorm(&norm, &hit->shape, pnt);

	for (i=scene->lightCount;i-->0;) {
		int shadowed = 0;
		lightReaching(&source, &scene->lights[i], pnt);

		vec3 toSource, sourcePnt, shadower;
		lightCenter(&sourcePnt, &scene->lights[i]);
		if (rayTrace(&shadower, scene, 1, &hit, pnt, &sourcePnt)) {
			float toLight = distance(&sourcePnt, pnt), toObj = distance(&shadower, pnt);
			if (toLight > toObj) {
				shadowed = 1;
			}
		}

		textureAt(&cur, &hit->texture);
		colorMultiply(&cur, &source);
		
		sub(&toSource, &sourcePnt, pnt);
		normalize(&toSource);

		float decrease = dot(&norm, &toSource);
		if (decrease < 0) {
			shadowed = 1;
		}
		float intensity = shadowed ? 0.25 : decrease*(1-0.25) + 0.25;
		colorScale(&cur, intensity);

		colorAdd(&result, &result, &cur);
	}
	
	*color = result;
}

void sceneRender(ImageData *img, Scene *scene) {
	int x, y;

	vec3 screen;

	for (x=0;x<img->width;++x) {
		for (y=0;y<img->height;++y) {
			cameraPoint(&screen, &scene->camera, (x+0.5)/img->width - 0.5, (y+0.5)/img->height - 0.5);

			vec3 pnt;
			SceneObject *hit = rayTrace(&pnt, scene, 0, NULL, &scene->camera.eye, &screen);
	
			gatherLight(&img->pixels[y*img->width+x], scene, &pnt, hit);
		}
	}
}

void cameraInit(Camera *cam, vec3 *eye, vec3 *screen, vec3 *up, float width, float height) {
	cam->eye = *eye;
	sub(&cam->view, screen, eye);

	cross(&cam->right, up, &cam->view);
	normalize(&cam->right);

	cross(&cam->up, &cam->right, &cam->view);
	normalize(&cam->up);

	scale(&cam->right, width);
	scale(&cam->up, height);
}

Camera *cameraAlloc(vec3 *eye, vec3 *screen, vec3 *up, float width, float height) {
	Camera *cam;
	cam = malloc(sizeof(Camera));

	cameraInit(cam, eye, screen, up, width, height);
	
	return cam;
}

void cameraPoint(vec3 *screen, Camera *c, float sx, float sy) {
	vec3 offx, offy, center;

	offx = c->right;
	offy = c->up;

	scale(&offx, sx);
	scale(&offy, sy);

	add(screen, &offx, &offy);
	add(&center, &c->eye, &c->view);

	add(screen, screen, &center);
}

void lightReaching(Color *color, Light *light, vec3 *dest) {
	switch (light->type) {
		case LIGHT_POINT_SOURCE:
			*color = light->pointLight.lightColor;
			return;
	}
}

void lightCenter(vec3 *center, Light *light) {
	switch (light->type) {
		case LIGHT_POINT_SOURCE:
			*center = light->pointLight.pos;
			return;
	}
}

