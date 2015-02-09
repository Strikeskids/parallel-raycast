#include "math.h"
#include "raycast.h"
#include "raytrace.h"
#include "vector.h"

#include <stdlib.h>
#include <stdio.h>

float castShape(Shape *s, vec3 *pos, vec3 *dir) {
	switch (s->type) {
		case SHAPE_SPHERE:
			return castSphere(&s->sphere, pos, dir);
		case SHAPE_PLANE:
			return castPlane(&s->plane, pos, dir);
		case SHAPE_TRIANGLE:
			return castTriangle(&s->triangle, pos, dir);
		default:
			return INFINITY;
	}
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
	vec3 n, p;
	shapeNorm(&n, (Shape *) s, &s->a);

	float t = (dot(&n, &s->a) - dot(&n, pos)) / (dot(&n, dir));

	p = *dir;
	scale(&p, t);
	add(&p, &p, pos);

	float s1, s2, s3;
	s1 = lineSide(&n, &s->a, &s->b, &p);
	s2 = lineSide(&n, &s->b, &s->c, &p);
	s3 = lineSide(&n, &s->c, &s->a, &p);

	return s1*s2 >= 0 && s1*s3 >= 0 ? t : INFINITY;
}

SceneObject *rayTrace(vec3 *hit, Scene *scene, int ignoreCount, SceneObject **ignore, vec3 *src, vec3 *dest) {
	int i, j, skip;

	vec3 r;
	float t;

	float bestT = INFINITY;
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
		
		t = castShape(&scene->objects[i].shape, src, &r);

		if (t >= 0 && t < bestT) {
			bestHit = r;
			scale(&bestHit, t);
			add(&bestHit, &bestHit, src);

			bestShape = &scene->objects[i];
			bestT = t;
		}
	}

	if (bestT < INFINITY) {
		*hit = bestHit;
		return bestShape;
	} else {
		return NULL;
	}
}

