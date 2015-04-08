#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "shape.h"
#include "vector.h"
#include "scene.h"

typedef struct Ray_struct {
	vec3 pnt;
	SceneObject *object;
	int index;
} Ray;

int rayTrace(Ray *ray, Scene *scene, vec3 *src, vec3 *dest);

#endif

