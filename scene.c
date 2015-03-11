#include "scene.h"
#include "raytrace.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926592653589
#endif

void lightModel(Color *out, SceneObject *obj, vec3 *norm, vec3 *pnt, Color *in, vec3 *from, vec3 *to) {
	vec3 vout, vin, vbis;
	float vinNorm, voutNorm, vbisNorm, vinVbis, vbisNorm2;
	float fresnel, rough, rough2, krough, roughIn, roughOut, slopeDistribution;

	float specularness, diffuseFactor, specularFactor;
	Color diffuseColor, fresnelColor, fresnelOppColor, specularColor;

	sub(&vin, from, pnt);
	normalize(&vin);

	sub(&vout, to, pnt);
	normalize(&vout);

	add(&vbis, &vout, &vin);
	normalize(&vbis);

	vinNorm = dot(norm, &vin);
	voutNorm = dot(norm, &vout);
	vbisNorm = dot(norm, &vbis);
	vbisNorm2 = vbisNorm * vbisNorm;
	vinVbis = dot(&vin, &vbis);

	specularness = materialSpecularness(&obj->material);

	diffuseFactor = (1 - specularness) / M_PI;

	fresnel = materialFresnel(&obj->material);
	materialSpecularAt(&fresnelColor, &obj->material, pnt);

	colorSub(&fresnelOppColor, &((Color) WHITE), &fresnelColor);
	colorScale(&fresnelOppColor, pow(1-vinVbis, 5));
	colorAdd(&specularColor, &fresnelColor, &fresnelOppColor);

	rough = materialRoughness(&obj->material);
	rough2 = rough * rough;
	krough = sqrt(2/M_PI) * rough;

	roughIn = vinNorm / (vinNorm * (1 - krough) + krough);
	roughOut = voutNorm / (voutNorm * (1 - krough) + krough);

	slopeDistribution = exp((vbisNorm2 - 1) / (rough2 * vbisNorm2)) / (rough2 * vbisNorm2 * vbisNorm2);

	specularFactor = specularness * slopeDistribution * roughIn * roughOut * fresnel;
	specularFactor = specularness * specularFactor / (4 * M_PI * vinNorm * voutNorm);

	materialDiffuseAt(&diffuseColor, &obj->material, pnt);

	colorScale(&specularColor, specularFactor);
	colorScale(&diffuseColor, diffuseFactor);

	colorAdd(out, &specularColor, &diffuseColor);
	colorScale(out, vinNorm < 0 ? 0 : vinNorm);
	colorMultiply(out, in);
}

void sceneTangent1(vec3 *tan, vec3 *norm) {
	vec3 opp;
	if (fabs(1-fabs(norm->x)) < 1e-2) {
		opp = (vec3) {0, 1.0, 0};
	} else {
		opp = (vec3) {1.0, 0, 0};
	}
	
	cross(tan, norm, &opp);
	normalize(tan);
}

void sceneTangent2(vec3 *tan2, vec3 *norm, vec3 *tan1) {
	cross(tan2, tan1, norm);
	normalize(tan2);
}

float scenePhimax(SceneObject *obj) {
	float rough;

	rough = materialRoughness(&obj->material);
	
	return (rough-ROUGH_MIN) * M_PI / 2 / (ROUGH_MAX-ROUGH_MIN);
}

float uniform(pcg32_random_t *p) {
	return (float) pcg32_random_r(p) / 0xFFFFFFFF;
}

void hemisphereSample(vec3 *samp, pcg32_random_t *p, vec3 *norm, vec3 *tan1, vec3 *tan2, float phimax) {
	float cosphi, sinphi;
	float theta, costheta, sintheta;

	cosphi = 1 - (1-cos(phimax))*uniform(p);
	sinphi = sqrt(1 - cosphi*cosphi);

	theta = M_PI * 2 * uniform(p);
	costheta = cos(theta);
	sintheta = sin(theta);

	vec3 sc1, sc2, sc3;
	sc1 = *norm;
	sc2 = *tan1;
	sc3 = *tan2;

	scale(&sc1, cosphi);
	scale(&sc2, costheta*sinphi);
	scale(&sc3, sintheta*sinphi);

	add(samp, &sc1, &sc2);
	add(samp, samp, &sc3);
}

void gatherLight(Color *color, pcg32_random_t *p, int level, Scene *scene, vec3 *to, vec3 *pnt, SceneObject *hit) {
	int i;
	Color result, cur, source;

	result = (Color) {0, 0, 0};
	if (!hit) {
		*color = result;
		return;
	}

	vec3 norm;
	shapeNorm(&norm, &hit->shape, pnt);

	for (i=scene->lightCount;i-->0;) {
		int shadowed = 0;
		lightReaching(&source, &scene->lights[i], pnt);

		vec3 sourcePnt, shadower;
		lightCenter(&sourcePnt, &scene->lights[i]);
		if (rayTrace(&shadower, scene, 1, &hit, pnt, &sourcePnt)) {
			float toLight = distance(&sourcePnt, pnt), toObj = distance(&shadower, pnt);
			if (toLight > toObj) {
				shadowed = 1;
			}
		}

		if (!shadowed) {
			lightModel(&cur, hit, &norm, pnt, &source, &sourcePnt, to);
			colorAdd(&result, &result, &cur);
		}
	}

	if (level < RENDER_REFLECTIONS) {
		Color inp;
		vec3 tan1, tan2, dir, towards, bounced;
		vec3 para, out, reflect;
		int i;

		sub(&out, to, pnt);
		normalize(&out);
		para = norm;
		scale(&para, 2*dot(&norm, &out));
		sub(&reflect, &para, &out);

		sceneTangent1(&tan1, &reflect);
		sceneTangent2(&tan2, &reflect, &tan1);
		float maxphi = scenePhimax(hit);

		for (i=0;i<SAMPLE_COUNT;++i) {
			hemisphereSample(&dir, p, &reflect, &tan1, &tan2, maxphi);
			if (dot(&norm, &dir) < 0)
				continue;
			add(&towards, pnt, &dir);

			SceneObject *next = rayTrace(&bounced, scene, 1, &hit, pnt, &towards);
			if (next) {
				gatherLight(&inp, p, level+1, scene, pnt, &bounced, next);
				lightModel(&cur, next, &norm, pnt, &inp, &bounced, to);
				colorScale(&cur, 1.0/SAMPLE_COUNT);

				colorAdd(&result, &result, &cur);
			}
		}
	}

	*color = result;
}

void sceneRender(ImageData *img, Scene *scene) {
	int x, y;

	#pragma omp parallel for collapse(2) private(x, y) shared(img, scene)
	for (x=0;x<img->width;++x) {
		for (y=0;y<img->height;++y) {
			vec3 screen, pnt;

			pcg32_random_t p;
			p.state = (uint64_t) rand()<<32 | rand();
			p.inc = (uint64_t) rand()<<32 | rand();

			SceneObject *hit;
			Color color;

			cameraPoint(&screen, &scene->camera, (x+0.5)/img->width - 0.5, (y+0.5)/img->height - 0.5);

			hit = rayTrace(&pnt, scene, 0, NULL, &scene->camera.eye, &screen);

			gatherLight(&color, &p, 0, scene, &scene->camera.eye, &pnt, hit);
			img->pixels[y*img->width+x] = colorPack(&color);
		}
	}
}

void cameraInit(Camera *cam, vec3 *eye, vec3 *screen, vec3 *up, float width, float height) {
	cam->eye = *eye;
	sub(&cam->view, screen, eye);

	cross(&cam->right, up, &cam->view);
	normalize(&cam->right);

	cross(&cam->up, &cam->right, &cam->view);
	normalize(&cam->up);

	scale(&cam->right, width);
	scale(&cam->up, height);
}

Camera *cameraAlloc(vec3 *eye, vec3 *screen, vec3 *up, float width, float height) {
	Camera *cam;
	cam = malloc(sizeof(Camera));

	cameraInit(cam, eye, screen, up, width, height);
	
	return cam;
}

void cameraPoint(vec3 *screen, Camera *c, float sx, float sy) {
	vec3 offx, offy, center;

	offx = c->right;
	offy = c->up;

	scale(&offx, sx);
	scale(&offy, sy);

	add(screen, &offx, &offy);
	add(&center, &c->eye, &c->view);

	add(screen, screen, &center);
}

void lightReaching(Color *color, Light *light, vec3 *dest) {
	switch (light->type) {
		case LIGHT_POINT_SOURCE:
			*color = light->pointLight.lightColor;
			colorScale(color, light->pointLight.power);
			return;
	}
}

void lightCenter(vec3 *center, Light *light) {
	switch (light->type) {
		case LIGHT_POINT_SOURCE:
			*center = light->pointLight.pos;
			return;
	}
}

