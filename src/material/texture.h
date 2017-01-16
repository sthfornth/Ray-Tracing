#ifndef MATERIAL_TEXTURE_H_H
#define MATERIAL_TEXTURE_H_H

#include "../core/common.h"
#include "../core/vec3f.h"

class Texture {
public:

    virtual Vec3f get(const Vec3f &pos, const Vec3f &norm) const = 0;
};

class ConstantTexture : public Texture {
public:
    Vec3f color;

    ConstantTexture(const Vec3f &c) : color(c) {
    }
    virtual Vec3f get(const Vec3f &pos, const Vec3f &norm) const override {
        return color;
    }
};

#endif