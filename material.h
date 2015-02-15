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

typedef struct BaseMaterial_struct {
	BASE_TEXTURE_STRUCT
} BaseMaterial;

typedef struct ConstantMaterial_struct {
	BASE_TEXTURE_STRUCT
	Color specular;
	Color diffuse;
} ConstantMaterial;

typedef struct CheckeredMaterial_struct {
	BASE_TEXTURE_STRUCT
	float checkSize;
	Color specular1;
	Color specular2;
	Color diffuse1;
	Color diffuse2;
} CheckeredMaterial;

typedef union Material_union {
	int type;
	BaseMaterial base;
	ConstantMaterial constant;
	CheckeredMaterial checkered;
} Material;

void materialSpecularAt(Color *color, Material *tex, vec3 *pos);
void materialDiffuseAt(Color *color, Material *tex, vec3 *pos);

float materialFresnel(Material *tex);
float materialRoughness(Material *tex);
float materialSpecularness(Material *tex);

#endif

