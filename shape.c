#include "shape.h"

void normSphere(vec3 *n, Sphere *s, vec3 *pnt);
void normPlane(vec3 *n, Plane *s, vec3 *pnt);
void normTriangle(vec3 *n, Triangle *t, vec3 *pnt);

void shapeNorm(vec3 *n, Shape *s, vec3 *pnt) {
	switch (s->type) {
	case SHAPE_PLANE:
		normPlane(n, &s->plane, pnt);
		return;
	case SHAPE_SPHERE:
		normSphere(n, &s->sphere, pnt);
		return;
	case SHAPE_TRIANGLE:
		normTriangle(n, &s->triangle, pnt);
		return;
	}
}

void normSphere(vec3 *n, Sphere *s, vec3 *pnt) {
	sub(n, pnt, &s->pos);
	normalize(n);
}

void normPlane(vec3 *n, Plane *s, vec3 *pnt) {
	*n = s->norm;
}

void normTriangle(vec3 *n, Triangle *t, vec3 *pnt) {
	vec3 x, y;
	sub(&x, &t->b, &t->a);
	sub(&y, &t->c, &t->a);
	cross(n, &x, &y);
	normalize(n);
}

