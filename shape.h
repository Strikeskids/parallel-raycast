#ifndef SHAPE_H
#define SHAPE_H

#include "vector.h"

typedef union Shape_union {
	int type;
	Sphere sphere;
	Plane plane;
} Shape;

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

#define PLANE_TYPE 0
#define SPHERE_TYPE 1
#define TRIANGLE_TYPE 2

void norm(vec3 *n, Shape *s, vec3 *pnt);
void normSphere(vec3 *n, Sphere *s, vec3 *pnt);
void normPlane(vec3 *n, Plane *s, vec3 *pnt);
void normTriangle(vec3 *n, Triangle *t, vec3 *pnt);

#endif

