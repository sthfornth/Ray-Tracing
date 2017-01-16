/*
 * Xinyue Cui
 * cxy1227wm@163.com
 */

#ifndef CORE_RAY_H
#define CORE_RAY_H
#include "vec3f.h"
class Ray {
public:
    Ray(void){}
    Ray(const Vec3f &o, const Vec3f &d) : origin(o), direct(d){}
    Ray(double ox, double oy, double oz, double dx, double dy, double dz){
        origin.x = ox, origin.y = oy, origin.z = oz;
        direct.x = dx, direct.y = dy, direct.z = dz;
    }
    Vec3f get(double t) const {
        return origin + direct * t;
    }
    void prt() const{
        origin.prt();
        direct.prt();
    }
    Vec3f origin;
    Vec3f direct;
};

#endif 
