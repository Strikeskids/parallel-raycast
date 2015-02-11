#include "texture.h"

void textureAt(Color *c, Texture *tex, vec3 *pos) {
	switch (tex->type) {
	case TEXTURE_CONSTANT:
		*c = tex->constant.color;
		return;
	}
}

