#include "math.h"
#include "raycast.h"
#include "raytrace.h"
#include "vector.h"

#include <stdlib.h>
#include <stdio.h>

float lineSide(vec3 *n, vec3 *a, vec3 *b, vec3 *p) {
	vec3 line, dir;
	sub(&line, b, a);
	sub(&dir, p, a);
	cross(&line, &line, &dir);
	return dot(n, &line);
}

float castThreeVec(vec3 *n, vec3 *a, vec3 *b, vec3 *c, vec3 *pos, vec3 *dir) {
	vec3 p;

	float t = (dot(n, a) - dot(n, pos)) / (dot(n, dir));

	p = *dir;
	scale(&p, t);
	add(&p, &p, pos);

	float s1, s2, s3;
	s1 = lineSide(n, a, b, &p);
	s2 = lineSide(n, b, c, &p);
	s3 = lineSide(n, c, a, &p);

	return s1*s2 >= 0 && s1*s3 >= 0 ? t : INFINITY;
}

float castShape(int *index, Shape *s, vec3 *pos, vec3 *dir) {
	switch (s->type) {
		case SHAPE_SPHERE:
			return castSphere(&s->sphere, pos, dir);
		case SHAPE_PLANE:
			return castPlane(&s->plane, pos, dir);
		case SHAPE_TRIANGLE:
			return castTriangle(&s->triangle, pos, dir);
		case SHAPE_OBJECT:
			return castFacedObject(index, &s->facedObject, pos, dir);
		default:
			return INFINITY;
	}
}

#define AXIS_CHECK(c, n, l, h, p) (n->c != 0 ? 1 : l->c <= p->c && p->c <= h->c)
#define AXES_CHECK(n, l, h, p) (AXIS_CHECK(x, n, l, h, p) && AXIS_CHECK(y, n, l, h, p) && AXIS_CHECK(z, n, l, h, p))

int castAxisRect(vec3 *norm, vec3 *low, vec3 *high, vec3 *pos, vec3 *dir) {
	vec3 p, *ps;

	float t = (dot(norm, low) - dot(norm, pos)) / (dot(norm, dir));

	p = *dir;
	scale(&p, t);
	add(&p, &p, pos);
	ps = &p;

	return AXES_CHECK(norm, low, high, ps);
}

#define AXIS_SET(a, value, v, orig) v = orig; v.a = value
#define AXES_SET(a, value) AXIS_SET(a, value, low, fo->low); AXIS_SET(a, value, high, fo->high)

float castFacedObject(int *index, FacedObject *fo, vec3 *pos, vec3 *dir) {
	vec3 low, high;
	
	int hit = 0;
	if (!hit) {
	AXES_SET(x, fo->low.x);
	hit = castAxisRect(&((vec3) {1, 0, 0}), &low, &high, pos, dir);
	}
	if (!hit) {
	AXES_SET(x, fo->high.x);
	hit = castAxisRect(&((vec3) {1, 0, 0}), &low, &high, pos, dir);
	}
	if (!hit) {
	AXES_SET(y, fo->low.y);
	hit = castAxisRect(&((vec3) {0, 1, 0}), &low, &high, pos, dir);
	}
	if (!hit) {
	AXES_SET(y, fo->high.y);
	hit = castAxisRect(&((vec3) {0, 1, 0}), &low, &high, pos, dir);
	}
	if (!hit) {
	AXES_SET(z, fo->low.z);
	hit = castAxisRect(&((vec3) {0, 0, 1}), &low, &high, pos, dir);
	}
	if (!hit) {
	AXES_SET(z, fo->high.z);
	hit = castAxisRect(&((vec3) {0, 0, 1}), &low, &high, pos, dir);
	}

	if (!hit) {
		return INFINITY;
	}

	int i, bestI = -1;
	float best = INFINITY, t;
	vec3 n;
	Face *face;
	for (i=0;i<fo->faceCount;++i) {
		face = &fo->faces[i];
		normThreeVec(&n, &fo->verts[face->a], &fo->verts[face->b], &fo->verts[face->c]);
		t = castThreeVec(&n, &fo->verts[face->a], &fo->verts[face->b], &fo->verts[face->c], pos, dir);
		if (t < best) {
			best = t;
			bestI = i;
		}
	}

	*index = bestI;
	return best;
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

float castTriangle(Triangle *s, vec3 *pos, vec3 *dir) {
	vec3 n;
	shapeNorm(&n, (Shape *) s, NULL, -1);
	return castThreeVec(&n, &s->a, &s->b, &s->c, pos, dir);
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
		ray->object = NULL;
		ray->index = -1;
		ray->pnt = (vec3) {0, 0, 0};
		return 0;
	}
}

