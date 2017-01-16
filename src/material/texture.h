#ifndef MATERIAL_TEXTURE_H_H
#define MATERIAL_TEXTURE_H_H

#include "../core/common.h"
#include "../core/vec3f.h"
#include <opencv2/opencv.hpp>

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

// class BmpTexture : public Texture<Vec3f> {
// public:
//     BmpTexture(std::string filename, const Vec3f &origin, const Vec3f &u, const Vec3f &v, double du, double dv, double gamma = 1.)
//         : _origin(origin), _u(u), _v(v), _du(du), _dv(dv) {

//         cv::Mat raw = cv::imread(filename, 1);
//         cv::Mat rawf;
//         raw.convertTo(rawf, CV_32FC3, 1. / 255, 0);
//         if (abs(gamma - 1) > EPS) {
//             cv::pow(rawf, gamma, _picture);
//         } else {
//             _picture = rawf;
//         }
//     }

//     virtual Vec3f get(const Vec3f &pos, const Vec3f &norm) const override {
//         int uu = static_cast<int>(dot((pos - _origin), _u) * _du);
//         int vv = static_cast<int>(dot((pos - _origin), _v) * _dv);
//         uu %= _picture.cols, vv %= _picture.rows;
//         if (uu < 0) uu += _picture.cols;
//         if (vv < 0) vv += _picture.rows;
//         cv::Vec3f res = _picture.at<cv::Vec3f>(vv, uu);
//         return Vec3f(res[2], res[1], res[0]);
//     }

// protected:
//     cv::Mat _picture;
//     Vec3f _origin, _u, _v;
//     double _du, _dv;
// };

// typedef Texture<Vec3f> Vec3fTexture;
// typedef Texture<float>  FloatTexture;

#endif //RAYTRACE_TEXTURE_H_H
