#include "texture.h"

#define CHECKER_FLOOR(f) ((int) (f < 0 ? f-1 : f))

void textureCheckeredAt(Color *c, CheckeredTexture *tex, vec3 *pos) {
	int type = CHECKER_FLOOR(pos->x / tex->checkSize) ^ CHECKER_FLOOR(pos->y / tex->checkSize) ^ CHECKER_FLOOR(pos->z / tex->checkSize);

	*c = (type & 1 ? tex->color1 : tex->color2);
}

void textureAt(Color *c, Texture *tex, vec3 *pos) {
	switch (tex->type) {
	case TEXTURE_CONSTANT:
		*c = tex->constant.color;
		return;
	case TEXTURE_CHECKERED:
		textureCheckeredAt(c, &tex->checkered, pos);
		return;
	}
}

