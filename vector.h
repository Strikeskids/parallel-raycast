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
} vec2:

float mag(vec3 *v);
float magsq(vec3 *v);

float mag(vec2 *v);
float magsq(vec2 *v);

void normalize(vec3 *v);
void normalize(vec2 *v);

float dot(vec3 *a, vec3 *b);
float dot(vec2 *a, vec2 *b);

void add(vec3 *dest, vec3 *a, vec3 *b);
void sub(vec3 *dest, vec3 *a, vec3 *b);

void add(vec2 *dest, vec2 *a, vec2 *b);
void sub(vec2 *dest, vec2 *a, vec2 *b);

void cross(vec3 *dest, vec3 *a, vec3 *b);
void vecCast(vec3 *dest, vec2 *src);

void scale(vec3 *v, float scale);
void scale(vec2 *v, float scale);

#endif
