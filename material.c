#include "material.h"

#define CHECKER_FLOOR(f) ((int) (f < 0 ? f-1 : f))

void materialCheckeredSpecularAt(Color *c, CheckeredMaterial *tex, vec3 *pos) {
	int type = CHECKER_FLOOR(pos->x / tex->checkSize) ^ CHECKER_FLOOR(pos->y / tex->checkSize) ^ CHECKER_FLOOR(pos->z / tex->checkSize);

	*c = (type & 1 ? tex->specular1 : tex->specular2);
}

void materialSpecularAt(Color *c, Material *tex, vec3 *pos) {
	switch (tex->type) {
	case MATERIAL_CONSTANT:
		*c = tex->constant.specular;
		return;
	case MATERIAL_CHECKERED:
		materialCheckeredSpecularAt(c, &tex->checkered, pos);
		return;
	case MATERIAL_FUNCTION:
		*c = tex->func.specular;
		return;
	}
}

void materialCheckeredDiffuseAt(Color *c, CheckeredMaterial *tex, vec3 *pos) {
	int type = CHECKER_FLOOR(pos->x / tex->checkSize) ^ CHECKER_FLOOR(pos->y / tex->checkSize) ^ CHECKER_FLOOR(pos->z / tex->checkSize);

	*c = (type & 1 ? tex->diffuse1 : tex->diffuse2);
}

void materialDiffuseAt(Color *c, Material *tex, vec3 *pos) {
	switch (tex->type) {
	case MATERIAL_CONSTANT:
		*c = tex->constant.diffuse;
		return;
	case MATERIAL_CHECKERED:
		materialCheckeredDiffuseAt(c, &tex->checkered, pos);
		return;
	case MATERIAL_FUNCTION:
		tex->func.diffuse(c, &tex->func, pos);
		return;
	}
}

float materialFresnel(Material *tex) {
	if (!(tex->type & MATERIAL_NON_BASE)) {
		return tex->base.fresnel;
	}
	return 0;
}

float materialRoughness(Material *tex) {
	if (!(tex->type & MATERIAL_NON_BASE)) {
		return tex->base.roughness;
	}
	return 0;
}

float materialSpecularness(Material *tex) {
	if (!(tex->type & MATERIAL_NON_BASE)) {
		return tex->base.specularness;
	}
	return 0;
}

