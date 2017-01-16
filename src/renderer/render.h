#ifndef _RENDER_RENDER_H
#define _RENDER_RENDER_H

#include "../core/common.h"
#include "../core/ray.h"
#include "../core/vec3f.h"
#include "../core/canvas.h"
#include "camera.h"
#include "../object/object.h"

class Render {

public:
	Scene* scene;
	Render(Scene *scene) :scene(scene){
	}
	virtual void render(Camera *camera, Canvas *canvas) = 0;
};

class DepthRender : public Render{
public:
	DepthRender(Scene *scene): Render(scene){}
	virtual void render(Camera *camera, Canvas *canvas){
		int h = canvas->h, w = canvas->w;
		for (int i = 0; i < h; ++i)
			for (int j = 0; j < w; ++j){
				Ray ray = camera->generate(double(i)/(h-1), double(j)/(w-1));
				Intersection inter = scene->intersect(ray);
				uint8 color = int(255 / max(inter.distance / 200, 1.0));
				for (int k = 0; k < 3; ++k)
					canvas->set(i,j,k,color);
			}
	}
};

class PathTracingRender : public Render{
public:
	int Times;
	PathTracingRender(Scene *scene, int Times): Render(scene), Times(Times){}
	virtual void render(Camera *camera, Canvas *canvas){
		int h = canvas->h, w = canvas->w;
		RandomStream* rng = new LCGStream();
		int cnt = 0;
#pragma omp parallel for schedule(dynamic, 1)
		for (int i = 0; i < h; ++i)
			for (int j = 0; j < w; ++j){
				Ray ray = camera->generate(double(i)/(h-1), double(j)/(w-1));
				if (++cnt%100 == 0){
					printf("%d\n", cnt);
				}
				
				Vec3f color;
				for (int t = 0; t < Times; ++ t)
					color += trace(ray, 10, rng);
				canvas->set(i, j, color / Times);
			}
	}
	Vec3f trace(const Ray& ray, int depth, RandomStream* rng){
		depth --;
		Intersection inter = scene->intersect(ray);
		if (depth < 0 || inter.distance < EPS || inter.distance >= INF){
			return Vec3f::ZERO;
		}
		Object* obj = inter.object;
		Vec3f pos = inter.position;
		Vec3f norm = inter.norm;
		Ray out;
		double pdf;
		Vec3f res;
		if(obj->is_light){
			// printf("light depth = %d\n", depth);
			return obj->light->get_emission(pos, ray.direct);
		}
		// obj->material->texture->get(pos, norm);
		BSDF* bsdf = obj->material->get_bsdf(pos, norm);
		bsdf->sample(ray, pos, norm, rng, out, pdf);
		res = trace(out, depth, rng);
		res /= pdf;
		return res * bsdf->get_reflectance(pos, norm);
	}
};

#endif