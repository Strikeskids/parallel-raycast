#ifndef SHAPE_H
#define SHAPE_H

#include <stdlib.h>
#include "vector.h"

#define SHAPE_PLANE 1
#define SHAPE_SPHERE 2
#define SHAPE_TRIANGLE 3
#define SHAPE_OBJECT 4

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

typedef struct Face_struct {
	int a;
	int b;
	int c;
} Face;

typedef struct FacedObject_struct {
	int type;
	vec3 low;
	vec3 high;
	int vertCount;
	vec3 *verts;
	int faceCount;
	Face *faces;
} FacedObject;

typedef union Shape_union {
	int type;
	Sphere sphere;
	Plane plane;
	Triangle triangle;
	FacedObject facedObject;
} Shape;

void shapeNorm(vec3 *n, Shape *s, vec3 *pnt, int index);

void shapeFacedObjectFree(FacedObject *faced);

#endif

