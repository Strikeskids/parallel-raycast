#ifndef RAYCAST_H
#define RAYCAST_H

#include "raytrace.h"
#include "shape.h"
#include "vector.h"

float castShape(int *index, Shape *s, vec3 *pos, vec3 *dir);
float castSphere(Sphere *s, vec3 *pos, vec3 *dir);
float castPlane(Plane *s, vec3 *pos, vec3 *dir);
float castTriangle(Triangle *s, vec3 *pos, vec3 *dir);
float castFacedObject(int *index, FacedObject *fo, vec3 *pos, vec3 *dir);

#endif

