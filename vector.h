#ifndef VECTOR_H
#define VECTOR_H

typedef struct vec3_struct {
	float x;
	float y;
	float z;
} vec3;

typedef struct vec2_struct {
	float x;
	float y;
} vec2;

float mag(vec3 *v);
float magsq(vec3 *v);

float mag2(vec2 *v);
float magsq2(vec2 *v);

void normalize(vec3 *v);
void normalize2(vec2 *v);

float dot(vec3 *a, vec3 *b);
float dot2(vec2 *a, vec2 *b);

void add(vec3 *dest, vec3 *a, vec3 *b);
void sub(vec3 *dest, vec3 *a, vec3 *b);

void add2(vec2 *dest, vec2 *a, vec2 *b);
void sub2(vec2 *dest, vec2 *a, vec2 *b);

void cross(vec3 *dest, vec3 *a, vec3 *b);
void vecCast(vec3 *dest, vec2 *src);

void scale(vec3 *v, float scale);
void scale2(vec2 *v, float scale);

#endif
