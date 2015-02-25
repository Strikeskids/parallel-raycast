#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "vector.h"

#define MATERIAL_CONSTANT 0x1
#define MATERIAL_CHECKERED 0x2
#define MATERIAL_FUNCTION 0x3
#define MATERIAL_NON_BASE 0x8000

#define BASE_MATERIAL_STRUCT \
	int type; \
	float fresnel; \
	float roughness; \
	float specularness;

typedef struct BaseMaterial_struct {
	BASE_MATERIAL_STRUCT
} BaseMaterial;

typedef struct ConstantMaterial_struct {
	BASE_MATERIAL_STRUCT
	Color specular;
	Color diffuse;
} ConstantMaterial;

typedef struct FunctionMaterial_struct {
	BASE_MATERIAL_STRUCT
	Color specular;
	void (*diffuse)(Color *, struct FunctionMaterial_struct *, vec3 *);
} FunctionMaterial;

typedef struct CheckeredMaterial_struct {
	BASE_MATERIAL_STRUCT
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
	FunctionMaterial func;
} Material;

void materialSpecularAt(Color *color, Material *tex, vec3 *pos);
void materialDiffuseAt(Color *color, Material *tex, vec3 *pos);

float materialFresnel(Material *tex);
float materialRoughness(Material *tex);
float materialSpecularness(Material *tex);

#endif

