#ifndef SCENE_H
#define SCENE_H

#include "vector.h"
#include "shape.h"

typedef struct Scene_struct {
	int lightCount;
	Light *lights;

	int shapeCount;
	Shape *shapes;

	Camera camera;
} Scene;

#define LIGHT_POINT_SOURCE 0

typedef union Light_union {
	int type;
	PointLight pointLight;
} Camera;

typedef struct PointLight_struct {
	int type;
	vec3 pos;
	Color lightcolor;
} PointLight;

typedef struct Camera_struct {
	vec3 eye;
	vec3 view;
	vec3 up;
	vec3 right;
} Camera;

void *sceneRender(ImageData *img, Scene *scene);
Camera *cameraAlloc(vec3 *eye, vec3 *screen, float rotate, float width, float height);
void cameraPoint(vec3 *screen, Camera *c, float sx, float sy);
void lightReaching(Color *color, Light *light, vec3 *dest);
void lightCenter(vec3 *center, Light *light);

#endif

