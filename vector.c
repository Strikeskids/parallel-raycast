#include "vector.h"
#include "math.h"

float distance(vec3 *a, vec3 *b) {
	return sqrt(distancesq(a, b));
}
float distancesq(vec3 *a, vec3 *b) {
	vec3 dif;
	sub(&dif, a, b);
	return dot(&dif, &dif);
}

float mag(vec3 *v) {
	return sqrt(dot(v,v));
}
float magsq(vec3 *v) {
	return dot(v,v);
}

float mag2(vec2 *v) {
	return sqrt(dot2(v,v));
}
float magsq2(vec2 *v) {
	return dot2(v,v);
}

void normalize(vec3 *v) {
	float m = mag(v);
	v->x /= m;
	v->y /= m;
	v->z /= m;
}
void normalize2(vec2 *v) {
	float m = mag2(v);
	v->x /= m;
	v->y /= m;
}

float dot(vec3 *a, vec3 *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}
float dot2(vec2 *a, vec2 *b) {
	return a->x * b->x + a->y * b->y;
}

void add(vec3 *dest, vec3 *a, vec3 *b) {
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
	dest->z = a->z + b->z;
}
void sub(vec3 *dest, vec3 *a, vec3 *b) {
	dest->x = a->x - b->x;
	dest->y = a->y - b->y;
	dest->z = a->z - b->z;
}

void add2(vec2 *dest, vec2 *a, vec2 *b) {
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
}
void sub2(vec2 *dest, vec2 *a, vec2 *b) {
	dest->x = a->x - b->x;
	dest->y = a->y - b->y;
}

void cross(vec3 *dest, vec3 *a, vec3 *b) {
	float x, y, z;
	x = a->y * b->z - a->z * b->y;
	y = a->z * b->x - a->x * b->z;
	z = a->x * b->y - a->y * b->x;
	dest->x = x;
	dest->y = y;
	dest->z = z;
}
void vecCast(vec3 *dest, vec2 *src) {
	dest->x = src->x;
	dest->y = src->y;
	dest->z = 0;
}

void scale(vec3 *v, float scale) {
	v->x *= scale;
	v->y *= scale;
	v->z *= scale;
}
void scale2(vec2 *v, float scale) {
	v->x *= scale;
	v->y *= scale;
}

void normThreeVec(vec3 *n, vec3 *a, vec3 *b, vec3 *c) {
	vec3 x, y;
	sub(&x, b, a);
	sub(&y, c, a);
	cross(n, &x, &y);
	normalize(n);
}

