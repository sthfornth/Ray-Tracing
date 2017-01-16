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
    	Vec3f x = norm.x < -0.1 || norm.x > 0.1 ? Vec3f(0, 1, 0): Vec3f(1, 0, 0);
    	Vec3f u = cross(x, norm).norm();
    	Vec3f v = cross(norm, u);
    	Vec3f res = u * (r * cos(w * 2 * PI)) + v * (r * sin(w * 2 * PI)) + norm * sqrt(1 - r2);
        return res.norm();
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
