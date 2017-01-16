/*
 * Xinyue Cui, cxy1227wm@163.com
 */

#ifndef _CORE_CANVAS_H
#define _CORE_CANVAS_H

#include "common.h"
#include "vec3f.h"

class Canvas {
public:
    uint8 *mat;
    int h, w, c;
    int st0, st1, size;

    Canvas(void) { }

    Canvas(int h_, int w_, int c_) : h(h_), w(w_), c(c_) {
        mat = new uint8[h_ * w_ * c_];
        st1 = c, st0 = c * w, size = c * w * h;
        memset(mat, 0, size * sizeof(uint8));
    }

    uint8 &get(int x, int y, int c = 0) {
        return mat[x * st0 + y * st1 + c];
    }

    void set(int x, int y, int c, uint8 v) {
        mat[x * st0 + y * st1 + c] = v;
    }

    void set(int x, int y, Vec3f color) {
        for (int i = 0; i < 3; ++ i){
            int c = min(255, max(0, int(color[i] * 255)));
            set(x, y, i, c);
        }
    }

    void show(const std::string &, bool wait = true);

    void write(const std::string &);

protected:
    cv::Mat _to_mat(void);
};

#endif 
