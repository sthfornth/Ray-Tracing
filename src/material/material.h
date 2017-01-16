#ifndef RAYTRACE_MATERIAL_H
#define RAYTRACE_MATERIAL_H

#include "bsdf.h"
#include "../core/common.h"
#include "../core/vec3f.h"
#include "../core/ray.h"
#include "../core/random.h"
#include "texture.h"

class Material {
public:
    virtual BSDF *get_bsdf(const Vec3f &pos, const Vec3f &norm) const = 0;
};

class SimpleMaterial : public Material {
public:
    SimpleMaterial(BSDF *const bsdf = NULL) : _bsdf(bsdf) {

    }
    virtual BSDF *get_bsdf(const Vec3f &pos, const Vec3f &norm) const override {
        return _bsdf;
    }
    virtual void set_bsdf(BSDF *const bsdf) {
        _bsdf = bsdf;
    }
private:
    BSDF *_bsdf;
};

class Light {
public:
    virtual Vec3f get_emission(const Vec3f &pos, const Vec3f &direct) const = 0;
};

class SimpleLight : public Light {
public:
    SimpleLight(const Vec3f &emission = Vec3f::ZERO) : _emission(emission) {

    }
    virtual inline Vec3f get_emission(const Vec3f &pos, const Vec3f &direct) const {
        return _emission;
    }
private:
    Vec3f _emission;
};

#endif //RAYTRACE_MTLBASE_H
