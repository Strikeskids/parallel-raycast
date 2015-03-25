#include "math.h"
#include "raycast.h"
#include "raytrace.h"
#include "vector.h"

#include <stdlib.h>
#include <stdio.h>

float castShape(int *index, Shape *s, vec3 *pos, vec3 *dir) {
	switch (s->type) {
		case SHAPE_SPHERE:
			return castSphere(&s->sphere, pos, dir);
		case SHAPE_PLANE:
			return castPlane(&s->plane, pos, dir);
		case SHAPE_TRIANGLE:
			return castTriangle(&s->triangle, pos, dir);
		case SHAPE_FACED_OBJECT:
			return castFacedObject(index, &s->facedObject, pos, dir);
		default:
			return INFINITY;
	}
}

float castFacedObject(int *index, FacedObject *fo, vec3 *pos, vec3 *dir) {
	
}

float castSphere(Sphere *s, vec3 *pos, vec3 *dir) {
	float b, c, d;
	
	vec3 dif;

	sub(&dif, pos, &s->pos);
	b = 2*dot(&dif, dir);
	c = dot(pos, pos) + dot(&s->pos, &s->pos) - 2*dot(pos, &s->pos) - s->radius * s->radius;

	d = b*b - 4*c;

	if (d < 0) return INFINITY;

	return -(b+sqrt(d))/2;
}

float castPlane(Plane *s, vec3 *pos, vec3 *dir) {
	vec3 *n = &s->norm;
	return (dot(n, &s->pos) - dot(n, pos)) / (dot(n, dir));
}

float lineSide(vec3 *n, vec3 *a, vec3 *b, vec3 *p) {
	vec3 line, dir;
	sub(&line, b, a);
	sub(&dir, p, a);
	cross(&line, &line, &dir);
	return dot(n, &line);
}

float castTriangle(Triangle *s, vec3 *pos, vec3 *dir) {
	vec3 n;
	shapeNorm(&n, (Shape *) s, NULL);
	return castThreeVec(&n, &s->a, &s->b, &s->c, pos, dir);
}

float castThreeVec(vec3 *n, vec3 *a, vec3 *b, vec3 *c, vec3 *pos, vec3 *dir) {
	vec3 p;

	float t = (dot(&n, a) - dot(&n, pos)) / (dot(&n, dir));

	p = *dir;
	scale(&p, t);
	add(&p, &p, pos);

	float s1, s2, s3;
	s1 = lineSide(&n, a, b, &p);
	s2 = lineSide(&n, b, c, &p);
	s3 = lineSide(&n, c, a, &p);

	return s1*s2 >= 0 && s1*s3 >= 0 ? t : INFINITY;
}

int rayTrace(Ray *ray, Scene *scene, int ignoreCount, SceneObject **ignore, vec3 *src, vec3 *dest) {
	int i, j, skip;

	vec3 r;
	float t;
	int castIndex;

	float bestT = INFINITY;
	int bestIndex = -1;
	SceneObject *bestShape;
	vec3 bestHit;

	sub(&r, dest, src);
	normalize(&r);

	for (i=scene->objectCount;i-->0;) {
		skip = 0;
		for (j=ignoreCount;j-->0;) {
			if (ignore[j] == &scene->objects[i]) {
				skip = 1;
				break;
			}
		}
		if (skip) continue;
		
		t = castShape(&castIndex, &scene->objects[i].shape, src, &r);

		if (t >= 0 && t < bestT) {
			bestHit = r;
			scale(&bestHit, t);
			add(&bestHit, &bestHit, src);

			bestShape = &scene->objects[i];
			bestT = t;
			bestIndex = castIndex;
		}
	}

	if (bestT < INFINITY) {
		ray->pnt = bestHit;
		ray->object = bestShape;
		ray->index = bestIndex;
		return 1;
	} else {
		return 0;
	}
}

