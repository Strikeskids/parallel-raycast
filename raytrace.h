#ifndef RAYTRACE_H
#define RAYTRACE_H

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

Shape *rayTrace(vec3 *hit, Scene *scene, int ignoreCount, Shape *ignore, vec3 *src, vec3 *dest);

#endif

