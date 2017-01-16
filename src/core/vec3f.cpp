
#include "vec3f.h"
#include <math.h>

const Vec3f Vec3f::ZERO = Vec3f(0, 0, 0);
const Vec3f Vec3f::X_AXIS = Vec3f(1, 0, 0);
const Vec3f Vec3f::Y_AXIS = Vec3f(0, 1, 0);

//////////////////////////////////////////////////////////////////////////
//  Constructors and Deconstructors
    

    Vec3f::Vec3f(void)
    {
        memset(_p,0,sizeof(double)*_len);
    }
    
    Vec3f::Vec3f(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3f::Vec3f(const Vec3f &v)
    {
        memcpy(_p,v._p,sizeof(double)*_len);
    }

    Vec3f::~Vec3f(void)
    {

    }

//////////////////////////////////////////////////////////////////////////
// Operators

    Vec3f& Vec3f::operator =( const Vec3f& v)
    {
        memcpy(_p,v._p,sizeof(double)*_len);        
        return (*this);
    }

    void Vec3f::operator +=(const Vec3f& v)
    {
        for(int i=0;i<_len;i++)
            _p[i] += v._p[i];
    }
    void Vec3f::operator +=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] += f;
    }

    void Vec3f::operator -=(const Vec3f& v)
    {
        for(int i=0;i<_len;i++)
            _p[i] -= v._p[i];
    }
    void Vec3f::operator -=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] -= f;
    }

    void Vec3f::operator *=(const Vec3f& v)
    {
        for(int i=0;i<_len;i++)
            _p[i] *= v._p[i];
    }
    void Vec3f::operator *=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] *= f;
    }

    void Vec3f::operator /=(const Vec3f& v)
    {
        for(int i=0;i<_len;i++)
            _p[i] /= v._p[i];
    }
    void Vec3f::operator /=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] /= f;
    }

    Vec3f Vec3f::operator +(const Vec3f&v) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] + v[i];
        return res;
    }
    Vec3f Vec3f::operator +(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] + f;
        return res;
    }

    Vec3f Vec3f::operator -(const Vec3f&v) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] - v[i];
        return res;
    }
    Vec3f Vec3f::operator -(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] - f;
        return res;
    }

    double Vec3f::operator &(const Vec3f&v) const   //dot conduct
    {
        double res;
        for(int i=0;i<_len;i++)
            res += (*this)[i] * v[i];
        return res;
    }

    Vec3f Vec3f::operator *(const Vec3f&v) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] * v[i];
        return res;
    }
    Vec3f Vec3f::operator *(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] * f;
        return res;
    }

    Vec3f Vec3f::operator /(const Vec3f&v) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] / v[i];
        return res;
    }
    Vec3f Vec3f::operator /(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] / f;
        return res;
    }

    Vec3f Vec3f::operator - () const 
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = -(*this)[i];
        return res;
    }

    Vec3f Vec3f::operator ^(const Vec3f&v) const    //cross product
    {
        Vec3f res;  
        for(int i=0;i<_len;i++)
            res[i] = (*this)[(i+1)%3] * v[(i+2)%3] - (*this)[(i+2)%3] * v[(i+1)%3];
        return res;
    }

//////////////////////////////////////////////////////////////////////////
// Other Methods

    void Vec3f::Normalize()
    {
        double fSqr = L2Norm_Sqr();
        if(fSqr > EPS)
            (*this) *= 1.0f/sqrt(fSqr);
    }

    Vec3f Vec3f::norm() const
    {
        double fSqr = L2Norm_Sqr();
        if(fSqr > EPS)
            return (*this) / sqrt(fSqr);
        else
            return *this;
    }

    double Vec3f::L2Norm_Sqr() const
    {
        return _p[0]*_p[0] + _p[1]*_p[1] + _p[2]*_p[2];
    }

    double dot(const Vec3f& v1, const Vec3f& v2)  //dot conduct
    {
        double res;
        for(int i=0;i<v1._len;i++)
            res += v1[i] * v2[i];
        return res;
    }

    Vec3f cross(const Vec3f& v1, const Vec3f& v2)
    {
        Vec3f res;  
        for(int i=0;i<v1._len;i++)
            res[i] = v1[(i+1)%3] * v2[(i+2)%3] - v1[(i+2)%3] * v2[(i+1)%3];
        return res;
    }
