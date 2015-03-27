#include "sceneloader.h"

int loadFloat(float *f, yaml_document_t *doc, yaml_node_item_t item);
int loadVec3(vec3 *v, yaml_document_t *doc, yaml_node_item_t vec3Item);
int loadColor(Color *c, yaml_document_t *doc, yaml_node_item_t item);
int loadPrintError(int returnCode, yaml_document_t *doc, yaml_node_item_t nodeItem, char *message);
int loadMaterialBase(BaseMaterial *material, yaml_document_t *doc, yaml_node_item_t item);
int loadMaterialConstant(ConstantMaterial *material, yaml_document_t *doc, yaml_node_item_t item);
int loadMaterialCheckered(CheckeredMaterial *material, yaml_document_t *doc, yaml_node_item_t item);
int loadMaterial(Material *material, yaml_document_t *doc, yaml_node_item_t item);
int loadShape(Shape *shape, yaml_document_t *doc, yaml_node_item_t item);
int loadShapeFacedObject(FacedObject *object, yaml_document_t *doc, yaml_node_item_t item);
int loadObjectFile(FacedObject *object, char *filename);
int loadShapeTriangle(Triangle *triangle, yaml_document_t *doc, yaml_node_item_t item);
int loadShapePlane(Plane *plane, yaml_document_t *doc, yaml_node_item_t item);
int loadShapeSphere(Sphere *sphere, yaml_document_t *doc, yaml_node_item_t item);
int loadSceneObject(SceneObject *object, yaml_document_t *doc, yaml_node_item_t item);
int loadPointLight(PointLight *light, yaml_document_t *doc, yaml_node_item_t item);
int loadLight(Light *light, yaml_document_t *doc, yaml_node_item_t nodeIndex);
int loadCamera(Camera *camera, yaml_document_t *doc, yaml_node_item_t item);
int loadLights(Scene *scene, yaml_document_t *doc, yaml_node_item_t lightsItem);
int loadObjects(Scene *scene, yaml_document_t *doc, yaml_node_item_t item);
int findNode(yaml_document_t *doc, yaml_node_t *mapping, char *key);

int sceneLoad(Scene *scene, char *fname) {
	FILE *f;

	f = fopen(fname, "r");
	if (!f) return 1;

	yaml_parser_t parser;
	yaml_document_t document;
	int failure;

	yaml_parser_initialize(&parser);
	yaml_parser_set_input_file(&parser, f);

	yaml_parser_load(&parser, &document);

	yaml_node_t *main;

	main = yaml_document_get_root_node(&document);

	if (!main) return 2;
	if (main->type != YAML_MAPPING_NODE) return 2;

	fprintf(stderr, "Start loading...\n");

	int lightsNode;
	int objectsNode;
	int cameraNode;

	lightsNode = findNode(&document, main, "lights");
	objectsNode = findNode(&document, main, "objects");
	cameraNode = findNode(&document, main, "camera");

	failure = loadLights(scene, &document, lightsNode);
	if (failure) return failure;
	failure = loadCamera(&scene->camera, &document, cameraNode);
	if (failure) return failure;
	failure = loadObjects(scene, &document, objectsNode);
	if (failure) return failure;

	return 0;
}

int findNode(yaml_document_t *doc, yaml_node_t *mapping, char *key) {
	if (mapping->type != YAML_MAPPING_NODE) 
		return loadPrintError(-1, doc, mapping - doc->nodes.start, "Expected mapping");
	yaml_node_pair_t *pnt;
	yaml_node_t *keyNode;
	for (pnt=mapping->data.mapping.pairs.start;pnt!=mapping->data.mapping.pairs.top;++pnt) {
		keyNode = yaml_document_get_node(doc, pnt->key);
		if (keyNode->type == YAML_SCALAR_NODE) {
			if (!strcmp(keyNode->data.scalar.value, key)) {
				return pnt->value;
			}
		}
	}
	return -1;
}

int loadObjects(Scene *scene, yaml_document_t *doc, yaml_node_item_t item) {
	if (item < 0) {
		scene->objectCount = 0;
		return 0;
	}
	yaml_node_item_t *cur, *start, *end;
	yaml_node_t *node = yaml_document_get_node(doc, item);

	int count, failure;
	start = node->data.sequence.items.start;
	end = node->data.sequence.items.top;
	count = end - start;

	if (node->type != YAML_SEQUENCE_NODE || count <= 0) {
		scene->objectCount = 0;
		return 0;
	}
	
	scene->objects = malloc(sizeof(SceneObject) * count);
	scene->objectCount = count;


	for (cur=start;cur!=end;++cur) {
		failure = loadSceneObject(&scene->objects[cur-start], doc, *cur);
		if (failure) return failure;
	}
	return 0;
}

int loadLights(Scene *scene, yaml_document_t *doc, yaml_node_item_t lightsItem) {
	if (lightsItem < 0) {
		scene->lightCount = 0;
		return 0;
	}
	yaml_node_item_t *item, *start, *end;
	yaml_node_t *lights = yaml_document_get_node(doc, lightsItem);

	int lightCount, failure;
	start = lights->data.sequence.items.start;
	end = lights->data.sequence.items.top;
	lightCount = end - start;

	if (lights->type != YAML_SEQUENCE_NODE || lightCount <= 0) {
		scene->lightCount = 0;
		return 0;
	}

	scene->lights = malloc(sizeof(Light) * lightCount);
	scene->lightCount = lightCount;

	for (item=start;item!=end;++item) {
		failure = loadLight(&scene->lights[item-start], doc, *item);
		if (failure) return failure;
	}

	return 0;
}

int loadCamera(Camera *camera, yaml_document_t *doc, yaml_node_item_t item) {
	if (item < 0) {
		return 0;
	}
	yaml_node_t *node = yaml_document_get_node(doc, item);
	yaml_node_item_t eyeIt, screenIt, upIt, widthIt, heightIt;
	int failure;

	eyeIt = findNode(doc, node, "eye");
	screenIt = findNode(doc, node, "screen");
	upIt = findNode(doc, node, "up");
	widthIt = findNode(doc, node, "width");
	heightIt = findNode(doc, node, "height");

	if ((eyeIt | screenIt | upIt | widthIt | heightIt) < 0)
		return loadPrintError(5, doc, item, "Missing keys in camera (eye*, screen*, up*, width*, height*)");

	vec3 eye, screen, up;
	float width, height;

	failure = loadVec3(&eye, doc, eyeIt);
	if (failure) return failure;
	failure = loadVec3(&screen, doc, screenIt);
	if (failure) return failure;
	failure = loadVec3(&up, doc, upIt);
	if (failure) return failure;

	failure = loadFloat(&width, doc, widthIt);
	if (failure) return failure;
	failure = loadFloat(&height, doc, heightIt);
	if (failure) return failure;

	cameraInit(camera, &eye, &screen, &up, width, height);

	return 0;
}

int loadLight(Light *light, yaml_document_t *doc, yaml_node_item_t nodeIndex) {
	switch (5) {
		default:
			loadPointLight(&light->pointLight, doc, nodeIndex);
			break;
	}
}

int loadPointLight(PointLight *light, yaml_document_t *doc, yaml_node_item_t item) {
	yaml_node_item_t posit, powerit, colorit;
	yaml_node_t *node = yaml_document_get_node(doc, item);
	int failure;

	if (node->type != YAML_MAPPING_NODE)
		return loadPrintError(4, doc, item, "Expected point light mapping");
	
	posit = findNode(doc, node, "pos");
	powerit = findNode(doc, node, "power");
	colorit = findNode(doc, node, "color");

	if ((posit | powerit | colorit) < 0)
		return loadPrintError(5, doc, item, "Missing keys in point light (pos*, power*, color*)");
	
	light->type = LIGHT_POINT_SOURCE;

	failure = loadVec3(&light->pos, doc, posit);
	if (failure) return failure;
	failure = loadFloat(&light->power, doc, powerit);
	if (failure) return failure;
	failure = loadColor(&light->lightColor, doc, colorit);
	if (failure) return failure;
	return 0;
}

int loadSceneObject(SceneObject *object, yaml_document_t *doc, yaml_node_item_t item) {

	yaml_node_t *node = yaml_document_get_node(doc, item);
	yaml_node_item_t shapeit, materialit;
	int failure;

	if (node->type != YAML_MAPPING_NODE)
		return loadPrintError(4, doc, item, "Expected scene object mapping");

	shapeit = findNode(doc, node, "shape");
	materialit = findNode(doc, node, "material");

	if ((shapeit | materialit) < 0)
		return loadPrintError(5, doc, item, "Missing keys in scene object (shape*, material*)");
	
	failure = loadShape(&object->shape, doc, shapeit);
	if (failure) return failure;
	failure = loadMaterial(&object->material, doc, materialit);
	if (failure) return failure;

	return 0;
}

int loadShapeSphere(Sphere *sphere, yaml_document_t *doc, yaml_node_item_t item) {
	yaml_node_t *node = yaml_document_get_node(doc, item);
	yaml_node_item_t posit, radiusit;
	int failure;

	sphere->type = SHAPE_SPHERE;

	posit = findNode(doc, node, "pos");
	radiusit = findNode(doc, node, "radius");
	
	if ((posit | radiusit) < 0)
		return loadPrintError(5, doc, item, "Missing keys in sphere (pos*, radius*)");

	failure = loadVec3(&sphere->pos, doc, posit);
	if (failure) return failure;
	failure = loadFloat(&sphere->radius, doc, radiusit);
	if (failure) return failure;

	return 0;
}
int loadShapePlane(Plane *plane, yaml_document_t *doc, yaml_node_item_t item) {
	yaml_node_t *node = yaml_document_get_node(doc, item);
	yaml_node_item_t posit, normit;
	int failure;

	plane->type = SHAPE_PLANE;

	posit = findNode(doc, node, "pos");
	normit = findNode(doc, node, "norm");
	
	if ((posit | normit) < 0)
		return loadPrintError(5, doc, item, "Missing keys in plane (pos*, norm*)");

	failure = loadVec3(&plane->pos, doc, posit);
	if (failure) return failure;
	failure = loadVec3(&plane->norm, doc, normit);
	if (failure) return failure;

	return 0;
}
int loadShapeTriangle(Triangle *triangle, yaml_document_t *doc, yaml_node_item_t item) {
	yaml_node_t *node = yaml_document_get_node(doc, item);
	yaml_node_item_t vertit;
	int failure;

	triangle->type = SHAPE_TRIANGLE;

	vertit = findNode(doc, node, "v");
	
	if (vertit < 0)
		return loadPrintError(5, doc, item, "Missing keys in triangle (v*)");

	yaml_node_t *vs = yaml_document_get_node(doc, vertit);

	yaml_node_item_t *start, *end;
	int count, i;

	start = vs->data.sequence.items.start;
	end = vs->data.sequence.items.top;

	count = end-start;

	if (vs->type != YAML_SEQUENCE_NODE || count != 3)
		return loadPrintError(4, doc, vertit, "Need 3 vertices for a triangle");

	vec3 vecs[3];
	for (i=0;i<3;++i) {
		failure = loadVec3(vecs+i, doc, *(start+i));
		if (failure) return failure;
	}

	triangle->a = vecs[0];
	triangle->b = vecs[1];
	triangle->c = vecs[2];

	return 0;
}
#define MIN(a, b) a < b ? a : b
#define MAX(a, b) a < b ? b : a
#define FVEC(c, a, b) (vec3) {c(a.x, b.x), c(a.y, b.y), c(a.z, b.z)}

int loadObjectFile(FacedObject *object, char *filename) {
	FILE *objfile = fopen(filename, "r");

	char *lineptr;
	int linelen;
	size_t ptrsize;

	int faceCount, faceSpace, vertCount, vertSpace;
	int i;
	vec3 *verts;
	Face *faces;

	vec3 low, high;

	faceCount = 0;
	faceSpace = 10;
	vertCount = 0;
	vertSpace = 10;

	verts = malloc(vertSpace * sizeof(vec3));
	faces = malloc(faceSpace * sizeof(vec3));

	while ((linelen = getline(&lineptr, &ptrsize, objfile)) != -1) {
		if (linelen < 4) continue;
		if (lineptr[0] == 'v') {
			vec3 cur;
			sscanf(lineptr, "v %f %f %f\n", &cur.x, &cur.y, &cur.z);
			if (vertCount >= vertSpace) {
				vertSpace = vertSpace*2 + 1;
				verts = realloc(verts, vertSpace * sizeof(vec3));
			}
			verts[vertCount++] = cur;
		} else if (lineptr[0] == 'f') {
			Face cur;
			sscanf(lineptr, "f %d %d %d\n", &cur.a, &cur.b, &cur.c);
			cur.a--;
			cur.b--;
			cur.c--;
			if (faceCount >= faceSpace) {
				faceSpace = faceSpace*2 + 1;
				faces = realloc(faces, faceSpace * sizeof(Face));
			}
			faces[faceCount++] = cur;
		}
	}

	low = verts[0];
	high = verts[0];

	for (i=1;i<vertCount;++i) {
		low = FVEC(MIN, low, verts[i]);
		high = FVEC(MAX, high, verts[i]);
	}

	object->low = low;
	object->high = high;
	object->vertCount = vertCount;
	object->verts = verts;
	object->faceCount = faceCount;
	object->faces = faces;

	fclose(objfile);

	return 0;
}

int loadShapeFacedObject(FacedObject *object, yaml_document_t *doc, yaml_node_item_t item) {
	yaml_node_t *node = yaml_document_get_node(doc, item);
	yaml_node_item_t fit, radiusit;
	int failure;

	object->type = SHAPE_OBJECT;

	fit = findNode(doc, node, "file");
	radiusit = findNode(doc, node, "radius");
	
	if (fit < 0)
		return loadPrintError(5, doc, item, "Missing keys in faced object (file*)");
	
	yaml_node_t *filenode = yaml_document_get_node(doc, fit);
	char filename[100];
	if (filenode->type != YAML_SCALAR_NODE || filenode->data.scalar.length >= 100)
		return loadPrintError(4, doc, fit, "String length < 100 expected for filename");

	strncpy(filename, filenode->data.scalar.value, filenode->data.scalar.length);
	filename[filenode->data.scalar.length] = '\0';

	object->type = SHAPE_OBJECT;

	return loadObjectFile(object, filename);
}
int loadShape(Shape *shape, yaml_document_t *doc, yaml_node_item_t item) {
	yaml_node_t *node = yaml_document_get_node(doc, item);
	yaml_node_item_t typeit;
	int failure;

	if (node->type != YAML_MAPPING_NODE)
		return loadPrintError(4, doc, item, "Expected shape mapping");

	typeit = findNode(doc, node, "type");
	
	if (typeit < 0)
		return loadPrintError(5, doc, item, "Missing keys in shape (type*)");

	int length;
	char *value;

	yaml_node_t *type = yaml_document_get_node(doc, typeit);

	value = type->data.scalar.value;
	length = type->data.scalar.length;

	if (!strncmp("sphere", value, length)) {
		return loadShapeSphere(&shape->sphere, doc, item);
	} else if (!strncmp("plane", value, length)) {
		return loadShapePlane(&shape->plane, doc, item);
	} else if (!strncmp("tri", value, length)) {
		return loadShapeTriangle(&shape->triangle, doc, item);
	} else if (!strncmp("object", value, length)) {
		return loadShapeFacedObject(&shape->facedObject, doc, item);
	} else {
		return loadPrintError(6, doc, typeit, "Invalid shape type");
	}
}

int loadMaterial(Material *material, yaml_document_t *doc, yaml_node_item_t item) {
	yaml_node_t *node = yaml_document_get_node(doc, item);
	yaml_node_item_t typeit;
	int failure;

	if (node->type != YAML_MAPPING_NODE)
		return loadPrintError(4, doc, item, "Expected material mapping");

	typeit = findNode(doc, node, "type");
	
	if (typeit < 0)
		return loadPrintError(5, doc, item, "Missing keys in material (type*)");

	int length;
	char *value;

	yaml_node_t *type = yaml_document_get_node(doc, typeit);

	value = type->data.scalar.value;
	length = type->data.scalar.length;

	if (!strncmp("constant", value, length)) {
		return loadMaterialConstant(&material->constant, doc, item);
	} else if (!strncmp("checkered", value, length)) {
		return loadMaterialCheckered(&material->checkered, doc, item);
	} else {
		return loadPrintError(6, doc, item, "Invalid material type");
	}
}

int loadMaterialCheckered(CheckeredMaterial *material, yaml_document_t *doc, yaml_node_item_t item) {
	yaml_node_t *node = yaml_document_get_node(doc, item);
	yaml_node_item_t specular1it, specular2it, specularit, diffuseit, diffuse1it, diffuse2it, checkit;

	int failure;

	failure = loadMaterialBase((BaseMaterial *) material, doc, item);
	if (failure) return failure;

	material->type = MATERIAL_CHECKERED;

	specularit = findNode(doc, node, "specular");
	diffuseit = findNode(doc, node, "diffuse");
	specular1it = findNode(doc, node, "specular1");
	diffuse1it = findNode(doc, node, "diffuse1");
	specular2it = findNode(doc, node, "specular2");
	diffuse2it = findNode(doc, node, "diffuse2");
	checkit = findNode(doc, node, "checksize");

	if ((diffuseit & (diffuse1it | diffuse2it)) < 0)
		return loadPrintError(5, doc, item, "Missing key in material (diffuse*) or (diffuse1*, diffuse2*)");

	if (specular1it >= 0) {
		failure = loadColor(&material->specular1, doc, specular1it);
		if (failure) return failure;
	} else {
		material->specular1 = (Color) {1.0, 1.0, 1.0};
	}
	if (specular2it >= 0) {
		failure = loadColor(&material->specular2, doc, specular2it);
		if (failure) return failure;
	} else {
		material->specular2 = (Color) {1.0, 1.0, 1.0};
	}
	if (specularit >= 0) {
		failure = loadColor(&material->specular1, doc, specularit);
		if (failure) return failure;
		material->specular2 = material->specular1;
	}

	if (diffuseit >= 0) {
		failure = loadColor(&material->diffuse1, doc, diffuseit);
		if (failure) return failure;
		material->diffuse2 = material->diffuse1;
	} else {
		failure = loadColor(&material->diffuse1, doc, diffuse1it);
		if (failure) return failure;
		failure = loadColor(&material->diffuse2, doc, diffuse2it);
		if (failure) return failure;
	}

	if (checkit >= 0) {
		failure = loadFloat(&material->checkSize, doc, checkit);
		if (failure) return failure;
	} else {
		material->checkSize = 0.2;
	}
	
	return 0;
}

int loadMaterialConstant(ConstantMaterial *material, yaml_document_t *doc, yaml_node_item_t item) {
	yaml_node_t *node = yaml_document_get_node(doc, item);
	yaml_node_item_t specularit, diffuseit;
	int failure;

	failure = loadMaterialBase((BaseMaterial *) material, doc, item);
	if (failure) return failure;

	material->type = MATERIAL_CONSTANT;

	specularit = findNode(doc, node, "specular");
	diffuseit = findNode(doc, node, "diffuse");

	if (diffuseit < 0)
		return loadPrintError(5, doc, item, "Missing key in material (specular*)");
	
	if (specularit >= 0) {
		failure = loadColor(&material->specular, doc, specularit);
		if (failure) return failure;
	} else {
		material->specular = (Color) {1.0, 1.0, 1.0};
	}
	failure = loadColor(&material->diffuse, doc, diffuseit);
	if (failure) return failure;
	
	return 0;
}

int loadMaterialBase(BaseMaterial *material, yaml_document_t *doc, yaml_node_item_t item) {
	yaml_node_t *node = yaml_document_get_node(doc, item);
	yaml_node_item_t fit, rit, specit;
	int failure;

	fit = findNode(doc, node, "fresnel");
	rit = findNode(doc, node, "roughness");
	specit = findNode(doc, node, "specularness");

	if (fit >= 0) {
		failure = loadFloat(&material->fresnel, doc, fit);
		if (failure) return failure;
	} else {
		material->fresnel = 0.2;
	}
	if (fit >= 0) {
		failure = loadFloat(&material->roughness, doc, fit);
		if (failure) return failure;
	} else {
		material->roughness = 0.2;
	}
	if (specit >= 0) {
		failure = loadFloat(&material->specularness, doc, specit);
		if (failure) return failure;
	} else {
		material->specularness = 0.5;
	}
	return 0;
}

int loadPrintError(int returnCode, yaml_document_t *doc, yaml_node_item_t nodeItem, char *message) {
	yaml_node_t *node = yaml_document_get_node(doc, nodeItem);

	fprintf(stderr, "%s\n\tat %lu:%lu\n", message, node->start_mark.line, node->start_mark.column);
	return returnCode;
}

int loadColor(Color *c, yaml_document_t *doc, yaml_node_item_t item) {
	yaml_node_t *node = yaml_document_get_node(doc, item);
	if (node->type != YAML_SEQUENCE_NODE)
		return loadPrintError(4, doc, item, "Expected 3 element color");

	yaml_node_item_t *start, *end;
	start = node->data.sequence.items.start;
	end = node->data.sequence.items.top;
	int i, failure;

	char scal[100];
	float vals[3] = {0,0,0};

	for (i=0;start+i<end&&i<3;++i) {
		failure = loadFloat(vals+i, doc, *(start+i));
		if (failure) return failure;
	}

	c->r = vals[0];
	c->g = vals[1];
	c->b = vals[2];
	return 0;
}

int loadVec3(vec3 *v, yaml_document_t *doc, yaml_node_item_t vec3Item) {
	yaml_node_t *node = yaml_document_get_node(doc, vec3Item);
	if (node->type != YAML_SEQUENCE_NODE)
		return loadPrintError(4, doc, vec3Item, "Expected 3 vector");
	
	yaml_node_item_t *start, *end;
	start = node->data.sequence.items.start;
	end = node->data.sequence.items.top;
	int i, failure;

	float vals[3] = {0,0,0};

	for (i=0;start+i<end&&i<3;++i) {
		failure = loadFloat(vals+i, doc, *(start+i));
		if (failure) return failure;
	}

	v->x = vals[0];
	v->y = vals[1];
	v->z = vals[2];

	return 0;
}

int loadFloat(float *f, yaml_document_t *doc, yaml_node_item_t item) {
	char scal[100];
	yaml_node_t *node = yaml_document_get_node(doc, item);
	if (node->type != YAML_SCALAR_NODE || node->data.scalar.length >= 100)
		return loadPrintError(4, doc, item, "Expected float value");

	memcpy(scal, node->data.scalar.value, node->data.scalar.length);
	scal[node->data.scalar.length] = '\0';

	*f = strtof(scal, NULL);

	return 0;
}

