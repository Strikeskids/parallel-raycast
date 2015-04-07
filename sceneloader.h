#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <yaml.h>

#include "scene.h"
#include "vector.h"
#include "color.h"
#include "material.h"

int sceneLoad(Scene *scene, char *fname);

#endif
