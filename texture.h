#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"

#define TEXTURE_CONSTANT 1

typedef struct ConstantTexture_struct {
	int type;
	Color color;
} ConstantTexture;

typedef union Texture_union {
	int type;
	ConstantTexture constant;
} Texture;

void textureAt(Color *color, Texture *tex);

#endif

