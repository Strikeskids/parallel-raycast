#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "vector.h"

#define TEXTURE_CONSTANT 0x1
#define TEXTURE_CHECKERED 0x2
#define TEXTURE_NON_BASE 0x8000

#define BASE_TEXTURE_STRUCT \
	int type; \
	float fresnel; \
	float roughness; \
	float specularness;

typedef struct BaseTexture_struct {
	BASE_TEXTURE_STRUCT
} BaseTexture;

typedef struct ConstantTexture_struct {
	BASE_TEXTURE_STRUCT
	Color specular;
	Color diffuse;
} ConstantTexture;

typedef struct CheckeredTexture_struct {
	BASE_TEXTURE_STRUCT
	float checkSize;
	Color specular1;
	Color specular2;
	Color diffuse1;
	Color diffuse2;
} CheckeredTexture;

typedef union Texture_union {
	int type;
	BaseTexture base;
	ConstantTexture constant;
	CheckeredTexture checkered;
} Texture;

void textureSpecularAt(Color *color, Texture *tex, vec3 *pos);
void textureDiffuseAt(Color *color, Texture *tex, vec3 *pos);

float textureFresnel(Texture *tex);
float textureRoughness(Texture *tex);
float textureSpecularness(Texture *tex);

#endif

