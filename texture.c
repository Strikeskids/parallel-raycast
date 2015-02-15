#include "texture.h"

#define CHECKER_FLOOR(f) ((int) (f < 0 ? f-1 : f))

void textureCheckeredSpecularAt(Color *c, CheckeredTexture *tex, vec3 *pos) {
	int type = CHECKER_FLOOR(pos->x / tex->checkSize) ^ CHECKER_FLOOR(pos->y / tex->checkSize) ^ CHECKER_FLOOR(pos->z / tex->checkSize);

	*c = (type & 1 ? tex->specular1 : tex->specular2);
}

void textureSpecularAt(Color *c, Texture *tex, vec3 *pos) {
	switch (tex->type) {
	case TEXTURE_CONSTANT:
		*c = tex->constant.specular;
		return;
	case TEXTURE_CHECKERED:
		textureCheckeredSpecularAt(c, &tex->checkered, pos);
		return;
	}
}

void textureCheckeredDiffuseAt(Color *c, CheckeredTexture *tex, vec3 *pos) {
	int type = CHECKER_FLOOR(pos->x / tex->checkSize) ^ CHECKER_FLOOR(pos->y / tex->checkSize) ^ CHECKER_FLOOR(pos->z / tex->checkSize);

	*c = (type & 1 ? tex->diffuse1 : tex->diffuse2);
}

void textureDiffuseAt(Color *c, Texture *tex, vec3 *pos) {
	switch (tex->type) {
	case TEXTURE_CONSTANT:
		*c = tex->constant.diffuse;
		return;
	case TEXTURE_CHECKERED:
		textureCheckeredDiffuseAt(c, &tex->checkered, pos);
		return;
	}
}

float textureFresnel(Texture *tex) {
	if (!(tex->type & TEXTURE_NON_BASE)) {
		return tex->base.fresnel;
	}
}

float textureRoughness(Texture *tex) {
	if (!(tex->type & TEXTURE_NON_BASE)) {
		return tex->base.roughness;
	}
}

float textureSpecularness(Texture *tex) {
	if (!(tex->type & TEXTURE_NON_BASE)) {
		return tex->base.specularness;
	}
}

