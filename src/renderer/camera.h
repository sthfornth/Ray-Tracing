#ifndef _RENDERER_CAMERA_H
#define _RENDERER_CAMERA_H

#include "../core/common.h"
#include "../core/ray.h"
#include "../core/vec3f.h"

class Camera
{
public:
	Camera(){}
	Camera(const Vec3f &e, const Vec3f &f, const Vec3f &u, double fov_)
		:eye(e), front(f), up(u)
		{
			right = u ^ f;
			fov = 2 * tan(fov_ * PI/360);
		};
	
	Ray generate(double x, double y){

		Vec3f r = right * ((x - 0.5)* fov);
		Vec3f u = up * ((y - 0.5)* fov);
		Vec3f d = front + r + u;
		d.Normalize();
		return Ray(eye, d);
	}

	Vec3f eye;
	Vec3f front;
	Vec3f up;
	Vec3f right;
	double fov;	//lateral angle
	double fov_scale;
};

#endif