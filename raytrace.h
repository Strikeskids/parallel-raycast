#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "shape.h"
#include "vector.h"
#include "scene.h"

SceneObject *rayTrace(vec3 *hit, Scene *scene, int ignoreCount, SceneObject **ignore, vec3 *src, vec3 *dest);

#endif

