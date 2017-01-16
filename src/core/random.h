/*
 * Xinyue Cui, cxy1227wm@163.com
 */

#ifndef _CORE_RANDOM_H
#define _CORE_RANDOM_H
#include "vec3f.h"


class RandomStream {
public:
    virtual double get() = 0;
    virtual ~RandomStream() {

    }

    Vec3f sample_hemisphere(Vec3f norm){
    	double r2 = get();
    	double r = sqrt(r2);
    	double w = get();	//angle
    	Vec3f x = norm.x < -0.1 || norm.x > 0.1 ? Vec3f::Y_AXIS : Vec3f::X_AXIS;
    	Vec3f u = cross(x, norm).norm();
    	Vec3f v = cross(norm, u);
    	Vec3f res = u * (r * cos(w * 2 * PI)) + v * (r * sin(w * 2 * PI)) + norm * sqrt(1 - r2);
        return res.norm();
    }

    Ray sample_disk(const Vec3f &center, double r, const Vec3f &norm) {
        double r0 = sqrt(get());
        double r1 = get() * (2.0 * PI);
        double rx = r * r0 * cos(r1);
        double ry = r * r0 * sin(r1);

        Vec3f w = norm;
        Vec3f wo = w.x < -0.1 || w.x > 0.1 ? Vec3f::Y_AXIS : Vec3f::X_AXIS;
        Vec3f u = cross(wo, w).norm();
        Vec3f v = cross(w, u);

        Ray result;
        result.origin = center + u * rx + v * ry;
        result.direct = norm;
        return result;
    }
};

class LCGStream : public RandomStream {
public:
    LCGStream(unsigned s = 54321) {
        seed = s;
    }

    virtual inline double get() {
        seed = 214013u * seed + 2531011u;
        return seed * (1.0 / 4294967296.0);
    }

protected:
    unsigned seed;
};

#endif
