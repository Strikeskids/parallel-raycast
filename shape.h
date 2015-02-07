#ifndef SHAPE_H
#define SHAPE_H

#include "vector.h"

typedef struct Sphere_struct {
	int type;
	vec3 pos;
	float radius;
} Sphere;

typedef struct Plane_struct {
	int type;
	vec3 pos;
	vec3 norm;
} Plane;

typedef struct Triangle_struct {
	int type;
	vec3 a;
	vec3 b;
	vec3 c;
} Triangle;

typedef union Shape_union {
	int type;
	Sphere sphere;
	Plane plane;
	Triangle triangle;
} Shape;

#define SHAPE_PLANE 0
#define SHAPE_SPHERE 1
#define SHAPE_TRIANGLE 2

void shapeNorm(vec3 *n, Shape *s, vec3 *pnt);

#endif

