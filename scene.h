#ifndef SCENE_H
#define SCENE_H

#include "vector.h"
#include "shape.h"
#include "texture.h"
#include "image.h"
#include "color.h"

#define LIGHT_POINT_SOURCE 1

typedef struct PointLight_struct {
	int type;
	vec3 pos;
	Color lightColor;
} PointLight;

typedef union Light_union {
	int type;
	PointLight pointLight;
} Light;

typedef struct Camera_struct {
	vec3 eye;
	vec3 view;
	vec3 up;
	vec3 right;
} Camera;

typedef struct SceneObject_struct {
	Shape shape;
	Texture texture;
} SceneObject;

typedef struct Scene_struct {
	int lightCount;
	Light *lights;

	int objectCount;
	SceneObject *objects;

	Camera camera;
} Scene;

void sceneRender(ImageData *img, Scene *scene);

Camera *cameraAlloc(vec3 *eye, vec3 *screen, vec3 *up, float width, float height);
void cameraInit(Camera *c, vec3 *eye, vec3 *screen, vec3 *up, float width, float height);
void cameraPoint(vec3 *screen, Camera *c, float sx, float sy);

void lightReaching(Color *color, Light *light, vec3 *dest);
void lightCenter(vec3 *center, Light *light);

#endif

