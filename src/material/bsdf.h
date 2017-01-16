#ifndef MATERIAL_BSDF_H
#define MATERIAL_BSDF_H
#include "../core/vec3f.h"
#include "texture.h"
#include "../core/random.h"

enum class BSDFType : int {
    Scatter     = 0x0200,
    Dielectric  = 0x0400,

    None        = 0x0000,
    Lambertian  = 0x0001 | Scatter,
    Specular    = 0x0002 | Dielectric,
    Phong       = 0x0004 | Scatter,
    Refractive  = 0x0008 | Dielectric,
};

enum class BSDFQueryType : int {
    BRDF = 0x0001,
    BTDF = 0x0002,
    All  = BRDF | BTDF,
};

inline BSDFType operator |(BSDFType t1, BSDFType t2) {
    return static_cast<BSDFType>((static_cast<int>(t1) | static_cast<int>(t2)));
}
inline bool operator &(BSDFType t1, BSDFType t2) {
    return (static_cast<int>(t1) & static_cast<int>(t2)) != 0;
}
inline BSDFQueryType operator |(BSDFQueryType t1, BSDFQueryType t2) {
    return static_cast<BSDFQueryType>((static_cast<int>(t1) | static_cast<int>(t2)));
}

inline bool operator &(BSDFQueryType t1, BSDFQueryType t2) {
    return (static_cast<int>(t1) & static_cast<int>(t2)) != 0;
}

class BSDF {
public:
    BSDF(Texture *const reflectance) : _reflectance(reflectance) {

    }
    virtual void sample(const Ray &in, const Vec3f &pos, const Vec3f &norm, RandomStream *rng,
                        Ray &out, double &pdf) const = 0;

    virtual Vec3f get_reflectance(const Vec3f &pos, const Vec3f &norm) const {
        return _reflectance->get(pos, norm);
    }

    virtual BSDFType get_type() const = 0;

    Vec3f make_spherical(const Vec3f &norm, double phi, double costheta, double sintheta) const {
        Vec3f w = norm;
        Vec3f wo = w.x < -0.1 || w.x > 0.1 ? Vec3f(0, 1, 0) : Vec3f(1, 0, 0);
        Vec3f u = cross(wo, w).norm();
        Vec3f v = cross(w, u);

        return w * costheta + u * sintheta * cos(phi) + v * sintheta * sin(phi);
    }

private:
    Texture *_reflectance;
};

class LambertianBRDF : public BSDF {
public:
    LambertianBRDF(Texture *const reflectance) : BSDF(reflectance) { }

    virtual void sample(const Ray &in, const Vec3f &pos, const Vec3f &norm, RandomStream *rng,
                        Ray &out, double &pdf) const override {
        Vec3f abs_norm = (dot(norm, in.direct) < 0) ? norm : norm * -1;
        out.origin = pos;
        out.direct = rng->sample_hemisphere(abs_norm);
        pdf = 1;
    }

    virtual BSDFType get_type() const override { return BSDFType::Lambertian; }
};

class SpecularBRDF : public BSDF {
public:
    SpecularBRDF(Texture *const reflectance) : BSDF(reflectance) { }

    virtual void sample(const Ray &in, const Vec3f &pos, const Vec3f &norm, RandomStream *rng,
                        Ray &out, double &pdf) const override {
        // puts("-----------");
        out.origin = pos;
        out.direct = reflect(in.direct, norm);
        pdf = 1;
        // out.prt();
    }

    virtual BSDFType get_type() const override { return BSDFType::Specular; }
};

class PhongBRDF : public BSDF {
public:
    PhongBRDF(Texture *const reflectance, int coeff=5) : BSDF(reflectance), _coeff(coeff) {

    }

    virtual void sample(const Ray &in, const Vec3f &pos, const Vec3f &norm, RandomStream *rng,
                        Ray &out, double &pdf) const override {

        Vec3f abs_norm = (dot(norm, in.direct) < 0) ? norm : norm * -1;
        Vec3f refl = reflect(in.direct, abs_norm);

        Vec3f w = refl;
        Vec3f wo = w.x < -0.1 || w.x > 0.1 ? Vec3f::Y_AXIS : Vec3f::X_AXIS;
        Vec3f u = cross(wo, w).norm();
        Vec3f v = cross(w, u);

        double theta = acos(pow(rng->get(), 1.0 / (_coeff + 1.0)));
        double phi   = 2.0 * PI * rng->get();

        pdf = 1.0;
        out.origin = pos;
        out.direct = (u * sin(theta) * cos(phi) + w * cos(theta) + v * sin(theta) * sin(phi)).norm();
    }

    virtual BSDFType get_type() const override { return BSDFType::Phong; }

private:
    int _coeff;
};

class RefractiveBTDF : public BSDF {
public:
    RefractiveBTDF(Texture *const reflectance, double beta = 1.5) : BSDF(reflectance), _beta(beta) {

    }

    virtual inline void sample(const Ray &in, const Vec3f &pos, const Vec3f &norm, RandomStream *rng,
                        Ray &out, double &pdf) const override {

        sample(in, pos, norm, rng, out, pdf, 0);
    }

    virtual void sample(const Ray &in, const Vec3f &pos, const Vec3f &norm, RandomStream *rng,
                        Ray &out, double &pdf, int next_id) const {

        Vec3f abs_norm = (dot(norm, in.direct) < 0) ? norm : norm * -1;
        bool   into = dot(norm, abs_norm) > 0;
        double alpha = 1, beta = _beta;
        double nnt = into ? alpha / beta : beta / alpha;
        double ddn = dot(in.direct, abs_norm);
        double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

        Vec3f d_refl = in.direct - norm * 2 * dot(norm, in.direct);

        out.origin = pos;
        if (cos2t < 0) {
            if (next_id == 0 || next_id == 1) {
                out.direct = d_refl;
                pdf = 1;
            } else {
                pdf = 0;
            }

        } else {
            Vec3f d_refr = (in.direct * nnt - norm * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();

            double a = beta - alpha, b = beta + alpha;
            double R0 = sqrt(a) / sqrt(b);
            double c = 1 - (into ? -ddn : dot(norm, d_refr));
            double Re = R0 + (1 - R0) * (c * c * c * c * c);
            double Tr = 1 - Re;
            double P = 0.25 + 0.5 * Re;

            if (next_id == 0) {
                if (rng->get() < P) {
                    out.direct = d_refl;
                    pdf = P / Re;
                } else {
                    out.direct = d_refr;
                    pdf = (1 - P) / Tr;
                }
            } else if (next_id == 1) {
                out.direct = d_refl;
                pdf = Re;
            } else if (next_id == 2) {
                out.direct = d_refr;
                pdf = Tr;
            }
        }
    }

    virtual BSDFType get_type() const override { return BSDFType::Refractive; }

private:
    double _beta;
};

class LambertianSpecularBSDF : public BSDF {
public:
    LambertianSpecularBSDF(Texture *const reflectance, double diff) : BSDF(reflectance), _diff(diff) { }

    virtual void sample(const Ray &in, const Vec3f &pos, const Vec3f &norm, RandomStream *rng,
                        Ray &out, double &pdf) const override {

        if (rng->get() < _diff) {
            Vec3f abs_norm = (dot(norm, in.direct) < 0) ? norm : norm * -1;
            out.origin = pos;
            out.direct = rng->sample_hemisphere(abs_norm);
            pdf = 1;
        } else {
            out.origin = pos;
            out.direct = reflect(in.direct, norm);
            pdf = 1;
        }
    }

    virtual BSDFType get_type() const override { return BSDFType::Lambertian | BSDFType::Specular ; }

private:
    double _diff;
};

#endif 
