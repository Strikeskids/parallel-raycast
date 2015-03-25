#include "shape.h"

void normSphere(vec3 *n, Sphere *s, vec3 *pnt);
void normPlane(vec3 *n, Plane *s, vec3 *pnt);
void normTriangle(vec3 *n, Triangle *t, vec3 *pnt);
void normFacedObject(vec3 *n, FacedObject *fo, vec3 *pnt, int index);

void shapeNorm(vec3 *n, Shape *s, vec3 *pnt, int index) {
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
	case SHAPE_FACED_OBJECT:
		normFacedObject(n, &s->facedObject, pnt, index);
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
	normThreeVec(n, &t->a, &t->b, &t->c);
}

void normFacedObject(vec3 *n, FacedObject *fo, vec3 *pnt, int index) {
	Face *face;
	if (index < 0 || index >= fo->faceCount)
		return;
	face = &fo->faces[index];
	normThreeVec(n, &fo->verts[face->a], &fo->verts[face->b], &fo->verts[face->c]);
}

void shapeFacedObjectFree(FacedObject *faced) {
	free(faced->verts);
	free(faced->faces);
}

