#include "vector.h"
#include "math.h"

float mag(vec3 *v) {
	return sqrt(dot(v,v));
}
float magsq(vec3 *v) {
	return dot(v,v);
}

float mag(vec2 *v) {
	return sqrt(dot(v,v));
}
float magsq(vec2 *v) {
	return dot(v,v);
}

void normalize(vec3 *v) {
	float m = mag(v);
	v->x /= m;
	v->y /= m;
	v->z /= m;
}
void normalize(vec2 *v) {
	float m = mag(v);
	v->x /= m;
	v->y /= m;
}

float dot(vec3 *a, vec3 *b) {
	return a->x*b->x + a->y*b->y + a->z*b->z;
}
float dot(vec2 *a, vec2 *b) {
	return a->x*b->x + a->y*b->y;
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

void add(vec2 *dest, vec2 *a, vec2 *b) {
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
}
void sub(vec2 *dest, vec2 *a, vec2 *b) {
	dest->x = a->x - b->x;
	dest->y = a->y - b->y;
}

void cross(vec3 *dest, vec3 *a, vec3 *b) {
	float x, y, z;
	x = a->y*b->z - a->z*b->y;
	y = a->z*b->x - a->z*b->z;
	z = a->x*b->y - a->y*b->x
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
void scale(vec2 *v, float scale) {
	v->x *= scale;
	v->y *= scale;
}
