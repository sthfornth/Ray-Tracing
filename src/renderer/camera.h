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

class DoFCamera : public Camera {
public:
    double fov, fov_scale, lens_r, focus_distance;

    DoFCamera(void) { }

    DoFCamera(const Vec3f &e, const Vec3f &f, const Vec3f &u, double fov_, double lens_r_, double focus_distance_) {
        eye = e, front = f, up = u, right = cross(f, u);
        fov = fov_, fov_scale = tan(fov_ * 0.5 * PI / 180) * 2;
        lens_r = lens_r_, focus_distance = focus_distance_;
    }

    inline virtual Ray generate(double x, double y, RandomStream *rng) {
        Vec3f r = right * ((x - 0.5) * fov_scale);
        Vec3f u = up * ((y - 0.5) * fov_scale);
        Ray center_ray = Ray(eye, (front + r + u).norm());
        Ray disk_ray = rng->sample_disk(eye, lens_r, front);
        Vec3f look_at = center_ray.origin + center_ray.direct * focus_distance / dot(center_ray.direct, front);
        disk_ray.direct = (look_at - disk_ray.origin).norm();
        return disk_ray;
    }
};


#endif