/*
 * Xinyue Cui, cxy1227wm@163.com
 */

#ifndef _CORE_VEC3F_H
#define _CORE_VEC3F_H

#include "common.h"

class Vec3f
{
public:

    //Constructors
    Vec3f();
    Vec3f(double x,double y, double z);
    Vec3f(const Vec3f& v);
    //Deconstructor
    virtual ~Vec3f();
public:
    //Operators

    //Operator []
    double& operator [](int index)
    {
        assert(index>=0&&index<3);
        return _p[index];
    }
    const double& operator [](int index) const
    {
        assert(index>=0&&index<3);
        return _p[index];
    }
    
    //Operator =
    Vec3f& operator = (const Vec3f& v);

    //Operators +=,-=, *=, /=
    void operator +=(const Vec3f& v);
    void operator +=(double f);
    void operator -=(const Vec3f& v);
    void operator -=(double f);
    void operator *=(const Vec3f& v);
    void operator *=(double f);
    void operator /=(const Vec3f& v);
    void operator /=(double f);

    //Operators +,-.*,/
    Vec3f operator +(const Vec3f&v) const;
    Vec3f operator +(double f) const;
    Vec3f operator -(const Vec3f&v) const;
    Vec3f operator -(double f) const;
    double operator &(const Vec3f&v) const;  //dot product
    Vec3f operator *(const Vec3f&v) const;
    Vec3f operator *(double f) const;
    Vec3f operator /(const Vec3f&v) const;
    Vec3f operator /(double f) const;
    Vec3f operator ^(const Vec3f&v) const;  //cross product

    Vec3f operator -() const;

public:
    void Normalize();
    Vec3f norm() const;
    double L2Norm_Sqr() const;
    void prt() const{
        printf("%lf %lf %lf\n", x,y,z);
    }
    Vec3f refelct(const Vec3f& in, const Vec3f& norm){
        Vec3f out = in + norm * ((norm & in) * 2);
        return out;
    }
 
public:
    union
    {
        struct
        { double _p[3]; };
        struct
        { double x,y,z; };
        struct
        { double r,g,b; };
    };
    enum {_len = 3};   
    static const Vec3f ZERO;
    static const Vec3f X_AXIS;
    static const Vec3f Y_AXIS;
}; 

double dot(const Vec3f& v1, const Vec3f& v2);
Vec3f cross(const Vec3f& v1, const Vec3f& v2);
Vec3f reflect(const Vec3f& v1, const Vec3f& v2);
#endif