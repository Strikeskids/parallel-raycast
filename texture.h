#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "vector.h"

#define TEXTURE_CONSTANT 1
#define TEXTURE_CHECKERED 2

typedef struct ConstantTexture_struct {
	int type;
	Color color;
} ConstantTexture;

typedef struct CheckeredTexture_struct {
	int type;
	float checkSize;
	Color color1;
	Color color2;
} CheckeredTexture;

typedef union Texture_union {
	int type;
	ConstantTexture constant;
	CheckeredTexture checkered;
} Texture;

void textureAt(Color *color, Texture *tex, vec3 *pos);

#endif

