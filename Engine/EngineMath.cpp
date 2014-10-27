/*
 *  EngineMath.cpp
 *
 *  Created by Michael DeCicco on 7/5/14.
 *
 */

#include "EngineMath.h"
#include "Engine.h"

#include <math.h>
#include <random>

namespace Reality
{
    Vector2::Vector2()
    {
    }
    Vector2::Vector2(Scalar _x,Scalar _y) : x(_x), y(_y)
    {
    }
    Vector2 Vector2::operator+(Scalar rval) const
    {
        return Vector2(x + rval,y + rval);
    }
    Vector2 Vector2::operator-(Scalar rval) const
    {
        return Vector2(x - rval,y - rval);
    }
    Vector2 Vector2::operator*(Scalar rval) const
    {
        return Vector2(x * rval,y * rval);
    }
    Vector2 Vector2::operator/(Scalar rval) const
    {
        return Vector2(x / rval,y / rval);
    }
    Vector2 Vector2::operator+=(Scalar rval)
    {
        return Vector2(x += rval,y += rval);
    }
    Vector2 Vector2::operator-=(Scalar rval)
    {
        return Vector2(x -= rval,y -= rval);
    }
    Vector2 Vector2::operator*=(Scalar rval)
    {
        return Vector2(x *= rval,y *= rval);
    }
    Vector2 Vector2::operator/=(Scalar rval)
    {
        return Vector2(x /= rval,y /= rval);
    }
    
    Vector2 Vector2::operator+(const Vector2& rval) const
    {
        return Vector2(x + rval.x,y + rval.y);
    }
    Vector2 Vector2::operator-(const Vector2& rval) const
    {
        return Vector2(x - rval.x,y - rval.y);
    }
    Vector2 Vector2::operator*(const Vector2& rval) const
    {
        return Vector2(x * rval.x,y * rval.y);
    }
    Vector2 Vector2::operator/(const Vector2& rval) const
    {
        return Vector2(x / rval.x,y / rval.y);
    }
    Vector2 Vector2::operator+=(const Vector2& rval)
    {
        return Vector2(x += rval.x,y += rval.y);
    }
    Vector2 Vector2::operator-=(const Vector2& rval)
    {
        return Vector2(x -= rval.x,y -= rval.y);
    }
    Vector2 Vector2::operator*=(const Vector2& rval)
    {
        return Vector2(x *= rval.x,y *= rval.y);
    }
    Vector2 Vector2::operator/=(const Vector2& rval)
    {
        return Vector2(x /= rval.x,y /= rval.y);
    }

    Scalar Vector2::Magnitude() const
    {
        #ifdef USE_Scalar_SCALAR
        return sqrt((x * x) + (y * y));
        #else
        return sqrtf((x * x) + (y * y));
        #endif
    }
    Scalar Vector2::MagnitudeSq() const
    {
        return (x * x) + (y * y);
    }
    void Vector2::Normalize()
    {
        Scalar mag = Magnitude();
        x /= mag;
        y /= mag;
    }
    Vector2 Vector2::Normalized() const
    {
        Scalar mag = Magnitude();
        return Vector2(x / mag,y / mag);
    }
    Vector2 Vector2::Perpendicular() const
    {
        return Vector2(-y,x);
    }
    Scalar Vector2::Dot(const Vector2& rval) const
    {
        return (x * rval.x) + (y * rval.y);
    }
    Scalar Vector2::Angle() const
    {
        #ifdef USE_Scalar_SCALAR
        return atan2(x,y);
        #else
        return atan2f(x,y);
        #endif
    }
    void Vector2::ClampMagnitude(Scalar Magnitude)
    {
        Normalize();
        x *= Magnitude;
        y *= Magnitude;
    }
    Vector2 Vector2::ClampMagnitude(Scalar Magnitude) const
    {
        return Normalized() * Magnitude;
    }
    
    Vector3::Vector3() : x(Scalar(0)), y(Scalar(0)), z(Scalar(0))
    {
    }
    Vector3::Vector3(Scalar _x,Scalar _y,Scalar _z) : x(_x), y(_y), z(_z)
    {
    }
    Vector3::Vector3(const Vector2& xy,Scalar _z) : x(xy.x), y(xy.y), z(_z)
    {
    }
    Vector3::Vector3(Scalar _x,const Vector2& yz) : x(_x), y(yz.x), z(yz.y)
    {
    }
    Vector3::Vector3(const Vector3& xyz) : x(xyz.x), y(xyz.y), z(xyz.z)
    {
    }
    Vector3::Vector3(const btVector3& xyz) : x(xyz.x()), y(xyz.y()), z(xyz.z())
    {
    }
    Vector3& Vector3::operator=(const Vector3& rval)
    {
        x = rval.x;
        y = rval.y;
        z = rval.z;
        return *this;
    }
    Vector3 Vector3::operator+(Scalar rval) const
    {
        return Vector3(x + rval,y + rval,z + rval);
    }
    Vector3 Vector3::operator-(Scalar rval) const
    {
        return Vector3(x - rval,y - rval,z - rval);
    }
    Vector3 Vector3::operator*(Scalar rval) const
    {
        return Vector3(x * rval,y * rval,z * rval);
    }
    Vector3 Vector3::operator/(Scalar rval) const
    {
        return Vector3(x / rval,y / rval,z / rval);
    }
    Vector3& Vector3::operator+=(Scalar rval)
    {
        x += rval;
        y += rval;
        z += rval;
        return *this;
    }
    Vector3& Vector3::operator-=(Scalar rval)
    {
        x -= rval;
        y -= rval;
        z -= rval;
        return *this;
    }
    Vector3& Vector3::operator*=(Scalar rval)
    {
        x *= rval;
        y *= rval;
        z *= rval;
        return *this;
    }
    Vector3& Vector3::operator/=(Scalar rval)
    {
        x *= rval;
        y *= rval;
        z *= rval;
        return *this;
    }
    
    Vector3 Vector3::operator+(const Vector3& rval) const
    {
        return Vector3(x + rval.x,y + rval.y,z + rval.z);
    }
    Vector3 Vector3::operator-(const Vector3& rval) const
    {
        return Vector3(x - rval.x,y - rval.y,z - rval.z);
    }
    Vector3 Vector3::operator*(const Vector3& rval) const
    {
        return Vector3(x * rval.x,y * rval.y,z * rval.z);
    }
    Vector3 Vector3::operator/(const Vector3& rval) const
    {
        return Vector3(x / rval.x,y / rval.y,z / rval.z);
    }
    Vector3& Vector3::operator+=(const Vector3& rval)
    {
        x += rval.x;
        y += rval.y;
        z += rval.z;
        return *this;
    }
    Vector3& Vector3::operator-=(const Vector3& rval)
    {
        x -= rval.x;
        y -= rval.y;
        z -= rval.z;
        return *this;
    }
    Vector3& Vector3::operator*=(const Vector3& rval)
    {
        x *= rval.x;
        y *= rval.y;
        z *= rval.z;
        return *this;
    }
    Vector3& Vector3::operator/=(const Vector3& rval)
    {
        x *= rval.x;
        y *= rval.y;
        z *= rval.z;
        return *this;
    }

    Scalar Vector3::Magnitude() const
    {
        #ifdef USE_Scalar_SCALAR
        return sqrt((x * x) + (y * y) + (z * z));
        #else
        return sqrtf((x * x) + (y * y) + (z * z));
        #endif
    }
    Scalar Vector3::MagnitudeSq() const
    {
        return (x * x) + (y * y) + (z * z);
    }
    void Vector3::Normalize()
    {
        Scalar mag = Magnitude();
        x /= mag;
        y /= mag;
        z /= mag;
    }
    Vector3 Vector3::Normalized() const
    {
        Scalar mag = Magnitude();
        return Vector3(x / mag,y / mag,z / mag);
    }
    Vector3 Vector3::Cross(const Vector3& rval) const
    {
        return Vector3(y * rval.z - z * rval.y, z * rval.x - x * rval.z, x * rval.y - y * rval.x);
    }
    Scalar Vector3::Dot(const Vector3& rval) const
    {
        return (x * rval.x) + (y * rval.y) + (z * rval.z);
    }
    void Vector3::ClampMagnitude(Scalar Magnitude)
    {
        Normalize();
        x *= Magnitude;
        y *= Magnitude;
        z *= Magnitude;
    }
    Vector3 Vector3::ClampMagnitude(Scalar Magnitude) const
    {
        return Normalized() * Magnitude;
    }
    Scalar Vector3::Angle(const Vector3 &With) const
    {
        Scalar LeftLength = Magnitude();
        Scalar RightLength = With.Magnitude();
        if(LeftLength > 0.0f && RightLength > 0.0f)
        {
            Scalar v = Dot(With) / LeftLength / RightLength;
            if(v < -1.0f) v = 1.0f;
            if(v > 1.0f) v = 1.0f;
            return acosf(v);
        }
        return 0.0f;
    }
    Vector2 Vector3::xx() const { return Vector2(x,x); }
    Vector2 Vector3::xy() const { return Vector2(x,y); }
    Vector2 Vector3::xz() const { return Vector2(x,z); }
    Vector2 Vector3::yx() const { return Vector2(y,x); }
    Vector2 Vector3::yy() const { return Vector2(y,y); }
    Vector2 Vector3::yz() const { return Vector2(y,z); }
    Vector2 Vector3::zx() const { return Vector2(z,x); }
    Vector2 Vector3::zy() const { return Vector2(z,y); }
    Vector2 Vector3::zz() const { return Vector2(z,z); }
    
    Vector4::Vector4() : x(Scalar(0)), y(Scalar(0)), z(Scalar(0)), w(Scalar(0))
    {
    }
    Vector4::Vector4(Scalar _x,Scalar _y,Scalar _z,Scalar _w) : x(_x), y(_y), z(_z), w(_w)
    {
    }
    Vector4::Vector4(const Vector3& xyz,Scalar _w) : x(xyz.x), y(xyz.y), z(xyz.z), w(_w)
    {
    }
    Vector4::Vector4(Scalar _x,const Vector3& yzw) : x(_x), y(yzw.x), z(yzw.y), w(yzw.z)
    {
    }
    Vector4 Vector4::operator+(Scalar rval) const
    {
        return Vector4(x + rval,y + rval,z + rval,w + rval);
    }
    Vector4 Vector4::operator-(Scalar rval) const
    {
        return Vector4(x - rval,y - rval,z - rval,w - rval);
    }
    Vector4 Vector4::operator*(Scalar rval) const
    {
        return Vector4(x * rval,y * rval,z * rval,w * rval);
    }
    Vector4 Vector4::operator/(Scalar rval) const
    {
        return Vector4(x / rval,y / rval,z / rval,w / rval);
    }
    Vector4 Vector4::operator+=(Scalar rval)
    {
        return Vector4(x += rval,y += rval,z += rval,w += rval);
    }
    Vector4 Vector4::operator-=(Scalar rval)
    {
        return Vector4(x -= rval,y -= rval,z -= rval,w -= rval);
    }
    Vector4 Vector4::operator*=(Scalar rval)
    {
        return Vector4(x *= rval,y *= rval,z *= rval,w *= rval);
    }
    Vector4 Vector4::operator/=(Scalar rval)
    {
        return Vector4(x /= rval,y /= rval,z /= rval,w /= rval);
    }
    
    Vector4 Vector4::operator+(const Vector4& rval) const
    {
        return Vector4(x + rval.x,y + rval.y,z + rval.z,w + rval.w);
    }
    Vector4 Vector4::operator-(const Vector4& rval) const
    {
        return Vector4(x - rval.x,y - rval.y,z - rval.z,w - rval.w);
    }
    Vector4 Vector4::operator*(const Vector4& rval) const
    {
        return Vector4(x * rval.x,y * rval.y,z * rval.z,w * rval.w);
    }
    Vector4 Vector4::operator/(const Vector4& rval) const
    {
        return Vector4(x / rval.x,y / rval.y,z / rval.z,w / rval.w);
    }
    Vector4 Vector4::operator+=(const Vector4& rval)
    {
        return Vector4(x += rval.x,y += rval.y,z += rval.z,w += rval.w);
    }
    Vector4 Vector4::operator-=(const Vector4& rval)
    {
        return Vector4(x -= rval.x,y -= rval.y,z -= rval.z,w -= rval.w);
    }
    Vector4 Vector4::operator*=(const Vector4& rval)
    {
        return Vector4(x *= rval.x,y *= rval.y,z *= rval.z,w *= rval.w);
    }
    Vector4 Vector4::operator/=(const Vector4& rval)
    {
        return Vector4(x /= rval.x,y /= rval.y,z /= rval.z,w /= rval.w);
    }

    Scalar Vector4::Magnitude() const
    {
        #ifdef USE_Scalar_SCALAR
        return sqrt((x * x) + (y * y) + (z * z) + (w * w));
        #else
        return sqrtf((x * x) + (y * y) + (z * z) + (w * w));
        #endif
    }
    Scalar Vector4::MagnitudeSq() const
    {
        return (x * x) + (y * y) + (z * z) + (w * w);
    }
    void Vector4::Normalize()
    {
        Scalar mag = Magnitude();
        x /= mag;
        y /= mag;
        z /= mag;
        w /= mag;
    }
    Vector4 Vector4::Normalized() const
    {
        Scalar mag = Magnitude();
        return Vector4(x / mag,y / mag,z / mag,w / mag);
    }
    Scalar Vector4::Dot(const Vector4& rval) const
    {
        return (x * rval.x) + (y * rval.y) + (z * rval.z) + (w * rval.w);
    }
    void Vector4::ClampMagnitude(Scalar Magnitude)
    {
        Normalize();
        x *= Magnitude;
        y *= Magnitude;
        z *= Magnitude;
        w *= Magnitude;
    }
    Vector4 Vector4::ClampMagnitude(Scalar Magnitude) const
    {
        return Normalized() * Magnitude;
    }
    Vector2 Vector4::xx() const { return Vector2(x,x); }
    Vector2 Vector4::xy() const { return Vector2(x,y); }
    Vector2 Vector4::xz() const { return Vector2(x,z); }
    Vector2 Vector4::xw() const { return Vector2(x,w); }
    Vector2 Vector4::yx() const { return Vector2(y,x); }
    Vector2 Vector4::yy() const { return Vector2(y,y); }
    Vector2 Vector4::yz() const { return Vector2(y,z); }
    Vector2 Vector4::yw() const { return Vector2(y,w); }
    Vector2 Vector4::zx() const { return Vector2(z,x); }
    Vector2 Vector4::zy() const { return Vector2(z,y); }
    Vector2 Vector4::zz() const { return Vector2(z,z); }
    Vector2 Vector4::zw() const { return Vector2(z,w); }
    Vector3 Vector4::xxx() const { return Vector3(x,x,x); }
    Vector3 Vector4::xxy() const { return Vector3(x,x,y); }
    Vector3 Vector4::xxz() const { return Vector3(x,x,z); }
    Vector3 Vector4::xxw() const { return Vector3(x,x,w); }
    Vector3 Vector4::xyx() const { return Vector3(x,y,x); }
    Vector3 Vector4::xyy() const { return Vector3(x,y,y); }
    Vector3 Vector4::xyz() const { return Vector3(x,y,z); }
    Vector3 Vector4::xyw() const { return Vector3(x,y,w); }
    Vector3 Vector4::xzx() const { return Vector3(x,z,x); }
    Vector3 Vector4::xzy() const { return Vector3(x,z,y); }
    Vector3 Vector4::xzz() const { return Vector3(x,z,z); }
    Vector3 Vector4::xzw() const { return Vector3(x,z,w); }
    Vector3 Vector4::xwx() const { return Vector3(x,w,x); }
    Vector3 Vector4::xwy() const { return Vector3(x,w,y); }
    Vector3 Vector4::xwz() const { return Vector3(x,w,z); }
    Vector3 Vector4::xww() const { return Vector3(x,w,w); }
    Vector3 Vector4::yxx() const { return Vector3(y,x,x); }
    Vector3 Vector4::yxy() const { return Vector3(y,x,y); }
    Vector3 Vector4::yxz() const { return Vector3(y,x,z); }
    Vector3 Vector4::yxw() const { return Vector3(y,x,w); }
    Vector3 Vector4::yyx() const { return Vector3(y,y,x); }
    Vector3 Vector4::yyy() const { return Vector3(y,y,y); }
    Vector3 Vector4::yyz() const { return Vector3(y,y,z); }
    Vector3 Vector4::yyw() const { return Vector3(y,y,w); }
    Vector3 Vector4::yzx() const { return Vector3(y,z,x); }
    Vector3 Vector4::yzy() const { return Vector3(y,z,y); }
    Vector3 Vector4::yzz() const { return Vector3(y,z,z); }
    Vector3 Vector4::yzw() const { return Vector3(y,z,w); }
    Vector3 Vector4::ywx() const { return Vector3(y,w,x); }
    Vector3 Vector4::ywy() const { return Vector3(y,w,y); }
    Vector3 Vector4::ywz() const { return Vector3(y,w,z); }
    Vector3 Vector4::yww() const { return Vector3(y,w,w); }
    Vector3 Vector4::zxx() const { return Vector3(z,x,x); }
    Vector3 Vector4::zxy() const { return Vector3(z,x,y); }
    Vector3 Vector4::zxz() const { return Vector3(z,x,z); }
    Vector3 Vector4::zxw() const { return Vector3(z,x,w); }
    Vector3 Vector4::zyx() const { return Vector3(z,y,x); }
    Vector3 Vector4::zyy() const { return Vector3(z,y,y); }
    Vector3 Vector4::zyz() const { return Vector3(z,y,z); }
    Vector3 Vector4::zyw() const { return Vector3(z,y,w); }
    Vector3 Vector4::zzx() const { return Vector3(z,z,x); }
    Vector3 Vector4::zzy() const { return Vector3(z,z,y); }
    Vector3 Vector4::zzz() const { return Vector3(z,z,z); }
    Vector3 Vector4::zzw() const { return Vector3(z,z,w); }
    Vector3 Vector4::zwx() const { return Vector3(z,w,x); }
    Vector3 Vector4::zwy() const { return Vector3(z,w,y); }
    Vector3 Vector4::zwz() const { return Vector3(z,w,z); }
    Vector3 Vector4::zww() const { return Vector3(z,w,w); }
    Vector3 Vector4::wxx() const { return Vector3(w,x,x); }
    Vector3 Vector4::wxy() const { return Vector3(w,x,y); }
    Vector3 Vector4::wxz() const { return Vector3(w,x,z); }
    Vector3 Vector4::wxw() const { return Vector3(w,x,w); }
    Vector3 Vector4::wyx() const { return Vector3(w,y,x); }
    Vector3 Vector4::wyy() const { return Vector3(w,y,y); }
    Vector3 Vector4::wyz() const { return Vector3(w,y,z); }
    Vector3 Vector4::wyw() const { return Vector3(w,y,w); }
    Vector3 Vector4::wzx() const { return Vector3(w,z,x); }
    Vector3 Vector4::wzy() const { return Vector3(w,z,y); }
    Vector3 Vector4::wzz() const { return Vector3(w,z,z); }
    Vector3 Vector4::wzw() const { return Vector3(w,z,w); }
    Vector3 Vector4::wwx() const { return Vector3(w,w,x); }
    Vector3 Vector4::wwy() const { return Vector3(w,w,y); }
    Vector3 Vector4::wwz() const { return Vector3(w,w,z); }
    Vector3 Vector4::www() const { return Vector3(w,w,w); }
    
    Vector3 Quaternion::operator *(const Vector3& rVal) const
    {
        Vector3 p0(x,y,z);
        Vector3 t = p0.Cross(rVal) * 2.0f;
        Vector3 v1 = rVal + (t * w) + p0.Cross(t);
        return v1;
    }
    Matrix4::Matrix4()
    {
    }
    Matrix4::Matrix4(Scalar xx,Scalar xy,Scalar xz,Scalar xw,
                     Scalar yx,Scalar yy,Scalar yz,Scalar yw,
                     Scalar zx,Scalar zy,Scalar zz,Scalar zw,
                     Scalar wx,Scalar wy,Scalar wz,Scalar ww)
    {
        x = Vector4(xx,xy,xz,xw);
        y = Vector4(yx,yy,yz,yw);
        z = Vector4(zx,zy,zz,zw);
        w = Vector4(wx,wy,wz,ww);
    }
    Matrix4::Matrix4(const Matrix4& m)
    {
        memcpy(&x,&m.x,sizeof(Vector4) * 4);
    }
    Matrix4::~Matrix4()
    {
    }
    Scalar Matrix4::Determinant() const
    {
        Scalar Result[4][4];
        Scalar tmp[12]; /* temp array for pairs */
        Scalar src[16]; /* array of transpose source matrix */
        
        for(u32 i = 0; i < 4; i++)
        {
            src[i + 0 ] = (*this)[i][0];
            src[i + 4 ] = (*this)[i][1];
            src[i + 8 ] = (*this)[i][2];
            src[i + 12] = (*this)[i][3];
        }
        
        tmp[0 ] = src[10] * src[15];
        tmp[1 ] = src[11] * src[14];
        tmp[2 ] = src[9 ] * src[15];
        tmp[3 ] = src[11] * src[13];
        tmp[4 ] = src[9 ] * src[14];
        tmp[5 ] = src[10] * src[13];
        tmp[6 ] = src[8 ] * src[15];
        tmp[7 ] = src[11] * src[12];
        tmp[8 ] = src[8 ] * src[14];
        tmp[9 ] = src[10] * src[12];
        tmp[10] = src[8 ] * src[13];
        tmp[11] = src[9 ] * src[12];
        
        Result[0][0]  = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4 ] * src[7];
        Result[0][0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5 ] * src[7];
        Result[0][1]  = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9 ] * src[7];
        Result[0][1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8 ] * src[7];
        Result[0][2]  = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
        Result[0][2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
        Result[0][3]  = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
        Result[0][3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
        Result[1][0]  = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5 ] * src[3];
        Result[1][0] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4 ] * src[3];
        Result[1][1]  = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8 ] * src[3];
        Result[1][1] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9 ] * src[3];
        Result[1][2]  = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
        Result[1][2] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
        Result[1][3]  = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
        Result[1][3] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
        
        tmp[0 ] = src[2] * src[7];
        tmp[1 ] = src[3] * src[6];
        tmp[2 ] = src[1] * src[7];
        tmp[3 ] = src[3] * src[5];
        tmp[4 ] = src[1] * src[6];
        tmp[5 ] = src[2] * src[5];

        tmp[6 ] = src[0] * src[7];
        tmp[7 ] = src[3] * src[4];
        tmp[8 ] = src[0] * src[6];
        tmp[9 ] = src[2] * src[4];
        tmp[10] = src[0] * src[5];
        tmp[11] = src[1] * src[4];
        
        Result[2][0]  = tmp[0 ] * src[13] + tmp[3 ] * src[14] + tmp[4 ] * src[15];
        Result[2][0] -= tmp[1 ] * src[13] + tmp[2 ] * src[14] + tmp[5 ] * src[15];
        Result[2][1]  = tmp[1 ] * src[12] + tmp[6 ] * src[14] + tmp[9 ] * src[15];
        Result[2][1] -= tmp[0 ] * src[12] + tmp[7 ] * src[14] + tmp[8 ] * src[15];
        Result[2][2]  = tmp[2 ] * src[12] + tmp[7 ] * src[13] + tmp[10] * src[15];
        Result[2][2] -= tmp[3 ] * src[12] + tmp[6 ] * src[13] + tmp[11] * src[15];
        Result[2][3]  = tmp[5 ] * src[12] + tmp[8 ] * src[13] + tmp[11] * src[14];
        Result[2][3] -= tmp[4 ] * src[12] + tmp[9 ] * src[13] + tmp[10] * src[14];
        Result[3][0]  = tmp[2 ] * src[10] + tmp[5 ] * src[11] + tmp[1 ] * src[9 ];
        Result[3][0] -= tmp[4 ] * src[11] + tmp[0 ] * src[9 ] + tmp[3 ] * src[10];
        Result[3][1]  = tmp[8 ] * src[11] + tmp[0 ] * src[8 ] + tmp[7 ] * src[10];
        Result[3][1] -= tmp[6 ] * src[10] + tmp[9 ] * src[11] + tmp[1 ] * src[8 ];
        Result[3][2]  = tmp[6 ] * src[9 ] + tmp[11] * src[11] + tmp[3 ] * src[8 ];
        Result[3][2] -= tmp[10] * src[11] + tmp[2 ] * src[8 ] + tmp[7 ] * src[9 ];
        Result[3][3]  = tmp[10] * src[10] + tmp[4 ] * src[8 ] + tmp[9 ] * src[9 ];
        Result[3][3] -= tmp[8 ] * src[9 ] + tmp[11] * src[10] + tmp[5 ] * src[8 ];
        
        return 1.0f / (src[0] * Result[0][0] + src[1] * Result[0][1] + src[2] * Result[0][2] + src[3] * Result[0][3]);
    }
    Matrix4 Matrix4::Inverse() const
    {
        Scalar Result[4][4];
        Scalar tmp[12]; /* temp array for pairs */
        Scalar src[16]; /* array of transpose source matrix */
        Scalar det;     /* determinant */
        
        for(u32 i = 0; i < 4; i++)
        {
            src[i + 0 ] = (*this)[i][0];
            src[i + 4 ] = (*this)[i][1];
            src[i + 8 ] = (*this)[i][2];
            src[i + 12] = (*this)[i][3];
        }
        
        tmp[0 ] = src[10] * src[15];
        tmp[1 ] = src[11] * src[14];
        tmp[2 ] = src[9 ] * src[15];
        tmp[3 ] = src[11] * src[13];
        tmp[4 ] = src[9 ] * src[14];
        tmp[5 ] = src[10] * src[13];
        tmp[6 ] = src[8 ] * src[15];
        tmp[7 ] = src[11] * src[12];
        tmp[8 ] = src[8 ] * src[14];
        tmp[9 ] = src[10] * src[12];
        tmp[10] = src[8 ] * src[13];
        tmp[11] = src[9 ] * src[12];
        
        Result[0][0]  = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4 ] * src[7];
        Result[0][0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5 ] * src[7];
        Result[0][1]  = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9 ] * src[7];
        Result[0][1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8 ] * src[7];
        Result[0][2]  = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
        Result[0][2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
        Result[0][3]  = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
        Result[0][3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
        Result[1][0]  = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5 ] * src[3];
        Result[1][0] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4 ] * src[3];
        Result[1][1]  = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8 ] * src[3];
        Result[1][1] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9 ] * src[3];
        Result[1][2]  = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
        Result[1][2] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
        Result[1][3]  = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
        Result[1][3] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
        
        tmp[0 ] = src[2] * src[7];
        tmp[1 ] = src[3] * src[6];
        tmp[2 ] = src[1] * src[7];
        tmp[3 ] = src[3] * src[5];
        tmp[4 ] = src[1] * src[6];
        tmp[5 ] = src[2] * src[5];

        tmp[6 ] = src[0] * src[7];
        tmp[7 ] = src[3] * src[4];
        tmp[8 ] = src[0] * src[6];
        tmp[9 ] = src[2] * src[4];
        tmp[10] = src[0] * src[5];
        tmp[11] = src[1] * src[4];
        
        Result[2][0]  = tmp[0 ] * src[13] + tmp[3 ] * src[14] + tmp[4 ] * src[15];
        Result[2][0] -= tmp[1 ] * src[13] + tmp[2 ] * src[14] + tmp[5 ] * src[15];
        Result[2][1]  = tmp[1 ] * src[12] + tmp[6 ] * src[14] + tmp[9 ] * src[15];
        Result[2][1] -= tmp[0 ] * src[12] + tmp[7 ] * src[14] + tmp[8 ] * src[15];
        Result[2][2]  = tmp[2 ] * src[12] + tmp[7 ] * src[13] + tmp[10] * src[15];
        Result[2][2] -= tmp[3 ] * src[12] + tmp[6 ] * src[13] + tmp[11] * src[15];
        Result[2][3]  = tmp[5 ] * src[12] + tmp[8 ] * src[13] + tmp[11] * src[14];
        Result[2][3] -= tmp[4 ] * src[12] + tmp[9 ] * src[13] + tmp[10] * src[14];
        Result[3][0]  = tmp[2 ] * src[10] + tmp[5 ] * src[11] + tmp[1 ] * src[9 ];
        Result[3][0] -= tmp[4 ] * src[11] + tmp[0 ] * src[9 ] + tmp[3 ] * src[10];
        Result[3][1]  = tmp[8 ] * src[11] + tmp[0 ] * src[8 ] + tmp[7 ] * src[10];
        Result[3][1] -= tmp[6 ] * src[10] + tmp[9 ] * src[11] + tmp[1 ] * src[8 ];
        Result[3][2]  = tmp[6 ] * src[9 ] + tmp[11] * src[11] + tmp[3 ] * src[8 ];
        Result[3][2] -= tmp[10] * src[11] + tmp[2 ] * src[8 ] + tmp[7 ] * src[9 ];
        Result[3][3]  = tmp[10] * src[10] + tmp[4 ] * src[8 ] + tmp[9 ] * src[9 ];
        Result[3][3] -= tmp[8 ] * src[9 ] + tmp[11] * src[10] + tmp[5 ] * src[8 ];
        
        det = src[0] * Result[0][0] + src[1] * Result[0][1] + src[2] * Result[0][2] + src[3] * Result[0][3];
        
        det = 1.0f / det;

        Matrix4 r;
        for(u32 i = 0; i < 4; i++)
        {
            for(u32 j = 0; j < 4; j++)
            {
                r[i][j] = Scalar(Result[i][j] * det);
            }
        }
        return r;
    }

    Matrix4 Matrix4::Transpose() const
    {
        Matrix4 Result;
        for(u32 i = 0;i < 4;i++)
        {
            for(u32 j = 0;j < 4;j++)
            {
                Result[j][i] = (*this)[i][j];
            }
        }
        return Result;
    }

    Matrix4 Matrix4::operator*(const Matrix4& rVal) const
    {
        Matrix4 r;
        for(i32 i = 0; i < 4; i++)
        {
            for(i32 j = 0;j < 4;j++)
            {
                Scalar Total = 0.0f;
                for(i32 k = 0;k < 4;k++)
                {
                    Total += (*this)[i][k] * rVal[k][j];
                }
                r[i][j] = Total;
            }
        }
        return r;
    }
    Matrix4& Matrix4::operator *=(const Matrix4& rVal)
    {
        Matrix4 r;
        for(i32 i = 0; i < 4; i++)
        {
            for(i32 j = 0;j < 4;j++)
            {
                Scalar Total = 0.0f;
                for(i32 k = 0;k < 4;k++)
                {
                    Total += (*this)[i][k] * rVal[k][j];
                }
                r[i][j] = Total;
            }
        }
        return *this = r;
    }
    Vector4 Matrix4::operator*(const Vector4& rVal) const
    {
        Scalar W = rVal.x * x.w +
                   rVal.y * y.w +
                   rVal.z * z.w +
                   rVal.w * w.w;
        if(W)
        {
            const Scalar invW = 1.0f / W;
            return Vector4((rVal.x * x.x + rVal.y * y.x + rVal.z * z.x + w.x) * invW,
                           (rVal.x * x.y + rVal.y * y.y + rVal.z * z.y + w.y) * invW,
                           (rVal.x * x.z + rVal.y * y.z + rVal.z * z.z + w.z) * invW,
                           (rVal.x * x.w + rVal.y * y.w + rVal.z * z.w + w.w) * invW);
        }
        return Vector4();
    }
    Vector3 Matrix4::operator*(const Vector3& rVal) const
    {
        Scalar W = rVal.x * x.w +
                   rVal.y * y.w +
                   rVal.z * z.w + w.w;
        if(W)
        {
            const Scalar invW = 1.0f / W;
            return Vector3((rVal.x * x.x + rVal.y * y.x + rVal.z * z.x + w.x) * invW,
                           (rVal.x * x.y + rVal.y * y.y + rVal.z * z.y + w.y) * invW,
                           (rVal.x * x.z + rVal.y * y.z + rVal.z * z.z + w.z) * invW);
        }
        return Vector3();
    }
    Matrix4 Rotation(const Vector3& _Basis1, const Vector3& _Basis2, const Vector3& _Basis3)
    {
        Vector3 Basis1 = _Basis1.Normalized();
        Vector3 Basis2 = _Basis2.Normalized();
        Vector3 Basis3 = _Basis3.Normalized();

        Matrix4 Result;
        Result[0][0] = Basis1.x;
        Result[1][0] = Basis1.y;
        Result[2][0] = Basis1.z;
        Result[3][0] = 0.0f;

        Result[0][1] = Basis2.x;
        Result[1][1] = Basis2.y;
        Result[2][1] = Basis2.z;
        Result[3][1] = 0.0f;

        Result[0][2] = Basis3.x;
        Result[1][2] = Basis3.y;
        Result[2][2] = Basis3.z;
        Result[3][2] = 0.0f;

        Result[0][3] = 0.0f;
        Result[1][3] = 0.0f;
        Result[2][3] = 0.0f;
        Result[3][3] = 1.0f;
        return Result;
    }

    Matrix4 Camera(const Vector3& Eye,const Vector3& _Look,const Vector3& _Up,const Vector3& _Right)
    {
        Vector3 Look = _Look.Normalized();
        Vector3 Up = _Up.Normalized();
        Vector3 Right = _Right.Normalized();

        Matrix4 Result;
        Result[0][0] = Right.x;
        Result[1][0] = Right.y;
        Result[2][0] = Right.z;
        Result[3][0] = -Right.Dot(Eye);

        Result[0][1] = Up.x;
        Result[1][1] = Up.y;
        Result[2][1] = Up.z;
        Result[3][1] = -Up.Dot(Eye);

        Result[0][2] = Look.x;
        Result[1][2] = Look.y;
        Result[2][2] = Look.z;
        Result[3][2] = -Look.Dot(Eye);

        Result[0][3] = 0.0f;
        Result[1][3] = 0.0f;
        Result[2][3] = 0.0f;
        Result[3][3] = 1.0f;
        return Result;
    }

    Matrix4 LookAt(const Vector3& Eye, const Vector3& At, const Vector3& Up)
    {
        Vector3 XAxis, YAxis, ZAxis;
        ZAxis = (Eye - At);
        XAxis = Up.Cross(ZAxis).Normalized();
        YAxis = ZAxis.Cross(XAxis).Normalized();

        Matrix4 Result;
        Result[0][0] = XAxis.x;
        Result[1][0] = XAxis.y;
        Result[2][0] = XAxis.z;
        Result[3][0] = -XAxis.Dot(Eye);

        Result[0][1] = YAxis.x;
        Result[1][1] = YAxis.y;
        Result[2][1] = YAxis.z;
        Result[3][1] = -YAxis.Dot(Eye);

        Result[0][2] = ZAxis.x;
        Result[1][2] = ZAxis.y;
        Result[2][2] = ZAxis.z;
        Result[3][2] = -ZAxis.Dot(Eye);

        Result[0][3] = 0.0f;
        Result[1][3] = 0.0f;
        Result[2][3] = 0.0f;
        Result[3][3] = 1.0f;
        return Result;
    }

    Matrix4 Orthogonal(Scalar Width, Scalar Height, Scalar Near, Scalar Far)
    {
        Matrix4 Result;
        Result[0][0] = 2.0f / Width;
        Result[1][0] = 0.0f;
        Result[2][0] = 0.0f;
        Result[3][0] = 0.0f;

        Result[0][1] = 0.0f;
        Result[1][1] = 2.0f / Height;
        Result[2][1] = 0.0f;
        Result[3][1] = 0.0f;

        Result[0][2] = 0.0f;
        Result[1][2] = 0.0f;
        Result[2][2] = 1.0f / (Near - Far);
        Result[3][2] = Near / (Near - Far);

        Result[0][3] = 0.0f;
        Result[1][3] = 0.0f;
        Result[2][3] = 0.0f;
        Result[3][3] = 1.0f;
        return Result;
    }
    Matrix4 Orthogonal(Scalar Left,Scalar Right,Scalar Top,Scalar Bottom, Scalar Near, Scalar Far)
    {
        Scalar Width = Right - Left;
        Scalar Height = Top - Bottom;
        Scalar Length = Far - Near  ;
        Matrix4 Result;
        Result[0][0] = 2.0f / Width;
        Result[1][0] = 0.0f;
        Result[2][0] = 0.0f;
        Result[3][0] = 0.0f;

        Result[0][1] = 0.0f;
        Result[1][1] = 2.0f / Height;
        Result[2][1] = 0.0f;
        Result[3][1] = 0.0f;

        Result[0][2] = 0.0f;
        Result[1][2] = 0.0f;
        Result[2][2] = -2.0f / Length;
        Result[3][2] = Near / (Near - Far);

        Result[0][3] = -((Right + Left) / Width );
        Result[1][3] = -((Top + Bottom) / Height);
        Result[2][3] = 0.0f;
        Result[3][3] = 1.0f;
        return Result;
    }

    Matrix4 Perspective(Scalar Width, Scalar Height, Scalar Near, Scalar Far)
    {
        Matrix4 Result;
        Result.x.x = 2.0f * Near / Width;
        Result.y.y = 2.0f * Near / Height;
        Result.z.z = Far / (Near - Far);
        Result.w.z = Far * Near / (Near - Far);
        Result.z.w = -1.0f;
        return Result;
    }

    Matrix4 PerspectiveFov(Scalar FOV, Scalar Aspect, Scalar Near, Scalar Far)
    {
        Scalar NearmFar = Near - Far;
        Scalar tFov = 1.0f / tanf(PI_OVER_180 * FOV * 0.5f);
        return Matrix4(
            tFov / Aspect, 0   , 0                      , 0,
            0            , tFov, 0                      , 0,
            0            , 0   , (Far + Near) / NearmFar,2.0f * Far * Near / NearmFar,
            0            , 0   ,-1                      , 0);
    }

    Matrix4 PerspectiveMultiFov(Scalar FovX, Scalar FovY, Scalar Near, Scalar Far)
    {
        Scalar NearmFar = Near - Far;
        return Matrix4(
            1.0f / tanf(PI_OVER_180 * FovX * 0.5f), 0, 0, 0,
            0, 1.0f / tanf(PI_OVER_180 * FovY * 0.5f), 0, 0,
            0, 0, (Far + Near) / NearmFar               ,2.0f * Far * Near / NearmFar,
            0, 0, -1                                    , 0);
    }

    Matrix4 Rotation(const Vector3& Axis, Scalar Angle)
    {
        Scalar c = cosf(Angle);
        Scalar s = sinf(Angle);
        Scalar t = 1.0f - c;

        Vector3 NormalizedAxis = Axis.Normalized();
        Scalar x = NormalizedAxis.x;
        Scalar y = NormalizedAxis.y;
        Scalar z = NormalizedAxis.z;

        Matrix4 Result;
        Result[0][0] = 1 + t*(x*x-1);
        Result[0][1] = z*s+t*x*y;
        Result[0][2] = -y*s+t*x*z;
        Result[0][3] = 0.0f;

        Result[1][0] = -z*s+t*x*y;
        Result[1][1] = 1+t*(y*y-1);
        Result[1][2] = x*s+t*y*z;
        Result[1][3] = 0.0f;

        Result[2][0] = y*s+t*x*z;
        Result[2][1] = -x*s+t*y*z;
        Result[2][2] = 1+t*(z*z-1);
        Result[2][3] = 0.0f;

        Result[3][0] = 0.0f;
        Result[3][1] = 0.0f;
        Result[3][2] = 0.0f;
        Result[3][3] = 1.0f;
        return Result;
    }

    Matrix4 Rotation(Scalar Yaw, Scalar Pitch, Scalar Roll)
    {
        return RotationY(Yaw) * RotationX(Pitch) * RotationZ(Roll);
    }

    Matrix4 Rotation(const Vector3& Axis, Scalar Angle, const Vector3& Center)
    {
        return Translation(-Center) * Rotation(Axis, Angle) * Translation(Center);
    }

    Matrix4 RotationX(Scalar Theta)
    {
        Scalar rad = Theta * PI_OVER_180;
        Scalar CosT = cosf(rad);
        Scalar SinT = sinf(rad);

        Matrix4 Result = Matrix4::Identity;
        Result[1][1] = CosT;
        Result[1][2] = SinT;
        Result[2][1] = -SinT;
        Result[2][2] = CosT;
        return Result;
    }

    Matrix4 RotationY(Scalar Theta)
    {
        Scalar rad = Theta * PI_OVER_180;
        Scalar CosT = cosf(rad);
        Scalar SinT = sinf(rad);

        Matrix4 Result = Matrix4::Identity;
        Result[0][0] = CosT;
        Result[0][2] = SinT;
        Result[2][0] = -SinT;
        Result[2][2] = CosT;
        return Result;
    }

    Matrix4 RotationZ(Scalar Theta)
    {
        Scalar rad = Theta * PI_OVER_180;
        Scalar CosT = cosf(rad);
        Scalar SinT = sinf(rad);

        Matrix4 Result = Matrix4::Identity;
        Result[0][0] = CosT;
        Result[0][1] = SinT;
        Result[1][0] = -SinT;
        Result[1][1] = CosT;
        return Result;
    }

    Matrix4 Scale(const Vector3& ScaleFactors)
    {
        Matrix4 Result;
        Result[0][0] = ScaleFactors.x;
        Result[1][0] = 0.0f;
        Result[2][0] = 0.0f;
        Result[3][0] = 0.0f;

        Result[0][1] = 0.0f;
        Result[1][1] = ScaleFactors.y;
        Result[2][1] = 0.0f;
        Result[3][1] = 0.0f;

        Result[0][2] = 0.0f;
        Result[1][2] = 0.0f;
        Result[2][2] = ScaleFactors.z;
        Result[3][2] = 0.0f;

        Result[0][3] = 0.0f;
        Result[1][3] = 0.0f;
        Result[2][3] = 0.0f;
        Result[3][3] = 1.0f;
        return Result;
    }
    Matrix4 Scale(Scalar s)
    {
        return Scale(Vector3(s,s,s));
    }
    Matrix4 Translation(const Vector3& Pos)
    {
        Matrix4 Result = Matrix4::Identity;
        Result.x.w = Pos.x;
        Result.y.w = Pos.y;
        Result.z.w = Pos.z;
        return Result;
    }

    Matrix4 Viewport(Scalar Width,Scalar Height)
    {
        return Scale(Vector3(Width * 0.5f,-Height * 0.5f,1.0f)) * Translation(Vector3(Width * 0.5f,Height * 0.5f,0.0f));
    }
    Matrix4 Matrix4::Identity = Matrix4(1,0,0,0,
                                        0,1,0,0,
                                        0,0,1,0,
                                        0,0,0,1);
    Quaternion::Quaternion() : x(0), y(0), z(0), w(0)
    {
    }
    Quaternion::Quaternion(Scalar Ax,Scalar Ay,Scalar Az,Scalar Angle)
    {
        Scalar a = (Angle * PI_OVER_180) * 0.5f;
        Scalar r = Scalar(sin(a));
        w = Scalar(cos(a));
        x = Ax * r;
        y = Ay * r;
        z = Az * r;
    }
    Quaternion::Quaternion(const Vector3& Axis,Scalar Angle)
    {
        Scalar a = (Angle * PI_OVER_180) * 0.5f;
        Scalar r = Scalar(sin(a));
        w = Scalar(cos(a));
        x = Axis.x * r;
        y = Axis.y * r;
        z = Axis.z * r;
    }
    Quaternion::Quaternion(const Quaternion& q)
    {
        x = q.x;
        y = q.y;
        z = q.z;
        w = q.w;
    }
    Quaternion::~Quaternion()
    {
    }

    void Quaternion::FromAxisAngle(Scalar Ax,Scalar Ay,Scalar Az,Scalar Angle)
    {
        Scalar a = (Angle * PI_OVER_180) * 0.5f;
        Scalar r = Scalar(sin(a));
        w = Scalar(cos(a));
        x = Ax * r;
        y = Ay * r;
        z = Az * r;
    }
    void Quaternion::FromAxisAngle(const Vector3& Axis,Scalar Angle)
    {
        Scalar a = (Angle * PI_OVER_180) * 0.5f;
        Scalar r = Scalar(sin(a));
        w = Scalar(cos(a));
        x = Axis.x * r;
        y = Axis.y * r;
        z = Axis.z * r;
    }
    Matrix4 Quaternion::ToMatrix() const
    {
        Matrix4 r;
        r.x.x = 1.0f - 2.0f * (y * y + z * z);
	    r.x.y = 2.0f * (x * y - w * z);
	    r.x.z = 2.0f * (x * z + w * y);
	    r.x.w = 0.0f;
	    r.y.x = 2.0f * (x * y + w * z);
	    r.y.y = 1.0f - 2.0f * (x * x + z * z);
	    r.y.z = 2.0f * (y * z - w * x);
	    r.y.w = 0.0f;
	    r.z.x = 2.0f * (x * z - w * y);
	    r.z.y = 2.0f * (y * z + w * x);
	    r.z.z = 1.0f - 2.0f * (x * x + y * y);
	    r.z.w = 0.0f;
	    r.w.x = r.w.y = r.w.z = 0;
	    r.w.w = 1.0f;
        return r;
    }

    Quaternion Quaternion::operator *(const Quaternion& q) const
    {
        Quaternion r;

        r.w = w * q.w - x * q.x - y * q.y - z * q.z;
        r.x = w * q.x + x * q.w + y * q.z - z * q.y;
        r.y = w * q.y + y * q.w + z * q.x - x * q.z;
        r.z = w * q.z + z * q.w + x * q.y - y * q.x;
	
        return r;
    }
    Quaternion& Quaternion::operator *=(const Quaternion& q)
    {
        Quaternion r;
        r.w = w * q.w - x * q.x - y * q.y - z * q.z;
        r.x = w * q.x + x * q.w + y * q.z - z * q.y;
        r.y = w * q.y + y * q.w + z * q.x - x * q.z;
        r.z = w * q.z + z * q.w + x * q.y - y * q.x;
        
        x = r.x;
        y = r.y;
        z = r.z;
        w = r.w;
        
        return *this;
    }
    bool BoundingVolume::IntersectsVolume(const BoundingVolume& v) const
    {
        Scalar Max_x,Min_x;
        Scalar Max_y,Min_y;
        Scalar Max_z,Min_z;
        if(TargetCenter->x < v.TargetCenter->x) { Min_x = TargetCenter->x; Max_x = v.TargetCenter->x; }
        else { Max_x = TargetCenter->x; Min_x = v.TargetCenter->x; }
        if(TargetCenter->y < v.TargetCenter->y) { Min_y = TargetCenter->y; Max_y = v.TargetCenter->y; }
        else { Max_y = TargetCenter->y; Min_y = v.TargetCenter->y; }
        if(TargetCenter->z < v.TargetCenter->z) { Min_z = TargetCenter->z; Max_z = v.TargetCenter->z; }
        else { Max_z = TargetCenter->z; Min_z = v.TargetCenter->z; }
        
        if(Max_x - Min_x > (Dimensions.x + v.Dimensions.x) * 0.5f) return false;
        if(Max_y - Min_y > (Dimensions.y + v.Dimensions.y) * 0.5f) return false;
        if(Max_z - Min_z > (Dimensions.z + v.Dimensions.z) * 0.5f) return false;
        return true;
    }
    u8 BoundingVolume::IntersectsFrustum(Vector4* Planes)
    {
        Scalar Radius = Dimensions.Magnitude() * 0.5f;
        int c = 0;
        Scalar d;
        if(m_LastRejectedPlaneIndex != 6)
        {
            d = Planes[m_LastRejectedPlaneIndex].xyz().Dot(*TargetCenter) + Planes[m_LastRejectedPlaneIndex].w;
            
            if(d <= -Radius)
            {
                return 0;
            }
            else m_LastRejectedPlaneIndex = 6;
        }
        
        for(u8 p = 0;p < 6;p++)
        {
            d = Planes[p].xyz().Dot(*TargetCenter) + Planes[p].w;
            if(d <= -Radius)
            {
                m_LastRejectedPlaneIndex = p;
                return 0;
            }
            if(d > Radius) c++;
        }
        return (c == 6) ? 2 : 1;
    }
    bool BoundingVolume::IsPointInside(const Vector3& p) const
    {
        Scalar Max_x,Min_x;
        Scalar Max_y,Min_y;
        Scalar Max_z,Min_z;
        if(TargetCenter->x < p.x) { Min_x = TargetCenter->x; Max_x = p.x; }
        else { Max_x = TargetCenter->x; Min_x = p.x; }
        if(TargetCenter->y < p.y) { Min_y = TargetCenter->y; Max_y = p.y; }
        else { Max_y = TargetCenter->y; Min_y = p.y; }
        if(TargetCenter->z < p.z) { Min_z = TargetCenter->z; Max_z = p.z; }
        else { Max_z = TargetCenter->z; Min_z = p.z; }
        
        if(Max_x - Min_x > Dimensions.x * 0.5f) return false;
        if(Max_y - Min_y > Dimensions.y * 0.5f) return false;
        if(Max_z - Min_z > Dimensions.z * 0.5f) return false;
        return true;
    }
    
    void Vec2(Scalar x,Scalar y,void *Memory) { new(Memory) Vector2(x,y); }
    void Vec2(const Vector2& xy,void *Memory) { new(Memory) Vector2(xy); }
    
    void Vec3(Scalar x,Scalar y,Scalar z,void *Memory) { new(Memory) Vector3(x,y,z); }
    void Vec3(const Vector2& xy,Scalar z,void *Memory) { new(Memory) Vector3(xy,z); }
    void Vec3(Scalar x,const Vector2& yz,void *Memory) { new(Memory) Vector3(x,yz); }
    void Vec3(const Vector3& xyz,void *Memory) { new(Memory) Vector3(xyz); }
    
    void Vec4(Scalar x,Scalar y,Scalar z,Scalar w,void *Memory) { new(Memory) Vector4(x,y,z,w); }
    void Vec4(const Vector3& xyz,Scalar w,void *Memory) { new(Memory) Vector4(xyz,w); }
    void Vec4(const Vector4& xyzw,void *Memory) { new(Memory) Vector4(xyzw); }
    void Vec4(Scalar x,const Vector3& yzw,void *Memory) { new(Memory) Vector4(x,yzw); }
    
    void Mat4_0(Scalar xx,Scalar xy,Scalar xz,Scalar xw,
                Scalar yx,Scalar yy,Scalar yz,Scalar yw,
                Scalar zx,Scalar zy,Scalar zz,Scalar zw,
                Scalar wx,Scalar wy,Scalar wz,Scalar ww,
                void* Memory) { new(Memory) Matrix4(xx,xy,xz,xw,yx,yy,yz,yw,zx,zy,zz,zw,wx,wy,wz,ww); }
    void Mat4_1(const Matrix4& m,void* Memory) { new(Memory) Matrix4(m); }
    
    void Quat(Scalar x,Scalar y,Scalar z,Scalar w,void *Memory) { new(Memory) Quaternion(x,y,z,w); }
    void Quat(const Vector3& xyz,Scalar w,void *Memory) { new(Memory) Quaternion(xyz,w); }
    void Quat(const Quaternion& q,void *Memory) { new(Memory) Quaternion(q); }
    
    static std::minstd_rand0 gen;
    void SeedRandom(i32 Seed) { if(Seed == -1) { gen.seed((i32)time(0)); } else gen.seed(Seed); }
    Scalar Random(Scalar Min,Scalar Max) { uniform_real_distribution<> dist(Min,Max); return dist(gen);  }
    Vector3 ColorFunc(Scalar Factor)
    {
        Scalar Div = (abs(fmodf(Factor,1.0f)) * 6.0f);
        Scalar Ascending = fmodf(Div,1.0f);
        Scalar Descending = 1.0f - Ascending;

        switch(int(Div))
        {
            case 0:  { return Vector3(1.0f,Ascending,0.0f);  }
            case 1:  { return Vector3(Descending,1.0f,0.0f); }
            case 2:  { return Vector3(0.0f,1.0f,Ascending);  }
            case 3:  { return Vector3(0.0f,Descending,1.0f); }
            case 4:  { return Vector3(Ascending,0.0f,1.0f);  }
            default: { return Vector3(1.0f,0.0f,Descending); }
        }
        
        return Vector3(1.0f,1.0f,1.0f);
    }
    
    void RegisterScriptMath()
    {
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        
        /* Types */
        
        #ifdef USE_Scalar_SCALAR
        Sys->RegisterTypedef("Scalar","Scalar");
        #else
        Sys->RegisterTypedef("Scalar","float");
        #endif
        
        Sys->RegisterEnum("UNIT");
        Sys->RegisterEnumValue("UNIT","GIGAPARSEC",GIGAPARSEC);
        Sys->RegisterEnumValue("UNIT","MEGAPARSEC",MEGAPARSEC);
        Sys->RegisterEnumValue("UNIT","KILOPARSEC",KILOPARSEC);
        Sys->RegisterEnumValue("UNIT","PARSEC",PARSEC);
        Sys->RegisterEnumValue("UNIT","MILLIPARSEC",MILLIPARSEC);
        Sys->RegisterEnumValue("UNIT","MICROPARSEC",MICROPARSEC);
        Sys->RegisterEnumValue("UNIT","NANOOPARSEC",NANOPARSEC);
        Sys->RegisterEnumValue("UNIT","PICOPARSEC",PICOPARSEC);
        Sys->RegisterEnumValue("UNIT","FEMTOPARSEC",FEMTOPARSEC);
        Sys->RegisterEnumValue("UNIT","ATTOPARSEC",ATTOPARSEC);
        
        /* Misc funcs */
        Sys->RegisterFunction("Scalar Random(Scalar Min,Scalar Max)",asFUNCTION(Random));
        Sys->RegisterFunction("void SeedRandom(int Seed)",asFUNCTION(SeedRandom));
        
        /* Classes */
        Sys->RegisterClass("Vector2",sizeof(Vector2),asOBJ_POD | asOBJ_VALUE | asOBJ_APP_CLASS_CDAK | asOBJ_APP_CLASS_ALLFLOATS);
        Sys->RegisterClassBehavior("Vector2",asBEHAVE_CONSTRUCT,"void f(Scalar x,Scalar y)",asFUNCTIONPR(Vec2,(Scalar,Scalar,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Vector2",asBEHAVE_CONSTRUCT,"void f(const Vector2& in)",asFUNCTIONPR(Vec2,(const Vector2&,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassProperty("Vector2","Scalar x",asOFFSET(Vector2,x));
        Sys->RegisterClassProperty("Vector2","Scalar y",asOFFSET(Vector2,y));
        Sys->RegisterClassMethod("Vector2","Vector2 opAdd(Scalar)",asMETHODPR(Vector2,operator+,(Scalar) const,Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opAdd(const Vector2& in) const",asMETHODPR(Vector2,operator+,(const Vector2&) const,Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opSub(Scalar) const",asMETHODPR(Vector2,operator-,(Scalar) const,Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opSub(const Vector2& in) const",asMETHODPR(Vector2,operator-,(const Vector2&) const,Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opMul(Scalar) const",asMETHODPR(Vector2,operator*,(Scalar) const,Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opMul(const Vector2& in) const",asMETHODPR(Vector2,operator*,(const Vector2&) const,Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opDiv(Scalar) const",asMETHODPR(Vector2,operator/,(Scalar) const,Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opDiv(const Vector2& in) const",asMETHODPR(Vector2,operator/,(const Vector2&) const,Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opAddAssign(Scalar)",asMETHODPR(Vector2,operator+=,(Scalar),Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opAddAssign(const Vector2& in)",asMETHODPR(Vector2,operator+=,(const Vector2&),Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opSubAssign(Scalar)",asMETHODPR(Vector2,operator+=,(Scalar),Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opSubAssign(const Vector2& in)",asMETHODPR(Vector2,operator-=,(const Vector2&),Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opMulAssign(Scalar)",asMETHODPR(Vector2,operator*=,(Scalar),Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opMulAssign(const Vector2& in)",asMETHODPR(Vector2,operator*=,(const Vector2&),Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opDivAssign(Scalar)",asMETHODPR(Vector2,operator/=,(Scalar),Vector2));
        Sys->RegisterClassMethod("Vector2","Vector2 opDivAssign(const Vector2& in)",asMETHODPR(Vector2,operator/=,(const Vector2&),Vector2));
        Sys->RegisterClassMethod("Vector2","Scalar Magnitude() const",asMETHOD(Vector2,Magnitude));
        Sys->RegisterClassMethod("Vector2","Scalar MagnitudeSq() const",asMETHOD(Vector2,MagnitudeSq));
        Sys->RegisterClassMethod("Vector2","void Normalize()",asMETHOD(Vector2,Normalize));
        Sys->RegisterClassMethod("Vector2","Vector2 Normalized() const",asMETHOD(Vector2,Normalized));
        Sys->RegisterClassMethod("Vector2","Vector2 Perpendicular() const",asMETHOD(Vector2,Perpendicular));
        Sys->RegisterClassMethod("Vector2","Scalar Dot(const Vector2& in) const",asMETHOD(Vector2,Dot));
        Sys->RegisterClassMethod("Vector2","Scalar Angle() const",asMETHOD(Vector2,Angle));
        Sys->RegisterClassMethod("Vector2","void ClampMagnitude(Scalar Magnitude)",asMETHODPR(Vector2,ClampMagnitude,(Scalar),void));
        Sys->RegisterClassMethod("Vector2","Vector2 ClampMagnitude(Scalar Magnitude) const",asMETHODPR(Vector2,ClampMagnitude,(Scalar) const,Vector2));
        
        Sys->RegisterClass("Vector3",sizeof(Vector3),asOBJ_POD | asOBJ_VALUE | asOBJ_APP_CLASS_CDAK | asOBJ_APP_CLASS_ALLFLOATS);
        Sys->RegisterClassBehavior("Vector3",asBEHAVE_CONSTRUCT,"void f(Scalar x,Scalar y,Scalar z)",asFUNCTIONPR(Vec3,(Scalar,Scalar,Scalar,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Vector3",asBEHAVE_CONSTRUCT,"void f(const Vector2& in,Scalar z)",asFUNCTIONPR(Vec3,(const Vector2&,Scalar,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Vector3",asBEHAVE_CONSTRUCT,"void f(Scalar x,const Vector2& in)",asFUNCTIONPR(Vec3,(Scalar,const Vector2&,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Vector3",asBEHAVE_CONSTRUCT,"void f(const Vector3& in)",asFUNCTIONPR(Vec3,(const Vector3&,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassProperty("Vector3","Scalar x",asOFFSET(Vector3,x));
        Sys->RegisterClassProperty("Vector3","Scalar y",asOFFSET(Vector3,y));
        Sys->RegisterClassProperty("Vector3","Scalar z",asOFFSET(Vector3,z));
        Sys->RegisterClassMethod("Vector3","Vector3& opAssign(const Vector3& in)",asMETHOD(Vector3,operator=));
        Sys->RegisterClassMethod("Vector3","Vector3 opAdd(Scalar x)",asMETHODPR(Vector3,operator+,(Scalar) const,Vector3));
        Sys->RegisterClassMethod("Vector3","Vector3 opAdd(const Vector3& in) const",asMETHODPR(Vector3,operator+,(const Vector3&) const,Vector3));
        Sys->RegisterClassMethod("Vector3","Vector3 opSub(Scalar x) const",asMETHODPR(Vector3,operator-,(Scalar) const,Vector3));
        Sys->RegisterClassMethod("Vector3","Vector3 opSub(const Vector3& in) const",asMETHODPR(Vector3,operator-,(const Vector3&) const,Vector3));
        Sys->RegisterClassMethod("Vector3","Vector3 opMul(Scalar x) const",asMETHODPR(Vector3,operator*,(Scalar) const,Vector3));
        Sys->RegisterClassMethod("Vector3","Vector3 opMul(const Vector3& in) const",asMETHODPR(Vector3,operator*,(const Vector3&) const,Vector3));
        Sys->RegisterClassMethod("Vector3","Vector3 opDiv(Scalar x) const",asMETHODPR(Vector3,operator/,(Scalar) const,Vector3));
        Sys->RegisterClassMethod("Vector3","Vector3 opDiv(const Vector3& in) const",asMETHODPR(Vector3,operator/,(const Vector3&) const,Vector3));
        Sys->RegisterClassMethod("Vector3","Vector3 &opAddAssign(Scalar x)",asMETHODPR(Vector3,operator+=,(Scalar),Vector3&));
        Sys->RegisterClassMethod("Vector3","Vector3 &opAddAssign(const Vector3& in)",asMETHODPR(Vector3,operator+=,(const Vector3&),Vector3&));
        Sys->RegisterClassMethod("Vector3","Vector3 &opSubAssign(Scalar x)",asMETHODPR(Vector3,operator-=,(Scalar),Vector3&));
        Sys->RegisterClassMethod("Vector3","Vector3 &opSubAssign(const Vector3& in)",asMETHODPR(Vector3,operator-=,(const Vector3&),Vector3&));
        Sys->RegisterClassMethod("Vector3","Vector3 &opMulAssign(Scalar x)",asMETHODPR(Vector3,operator*=,(Scalar),Vector3&));
        Sys->RegisterClassMethod("Vector3","Vector3 &opMulAssign(const Vector3& in)",asMETHODPR(Vector3,operator*=,(const Vector3&),Vector3&));
        Sys->RegisterClassMethod("Vector3","Vector3 &opDivAssign(Scalar x)",asMETHODPR(Vector3,operator/=,(Scalar),Vector3&));
        Sys->RegisterClassMethod("Vector3","Vector3 &opDivAssign(const Vector3& in)",asMETHODPR(Vector3,operator/=,(const Vector3&),Vector3&));
        Sys->RegisterClassMethod("Vector3","Scalar Magnitude() const",asMETHOD(Vector3,Magnitude));
        Sys->RegisterClassMethod("Vector3","Scalar MagnitudeSq() const",asMETHOD(Vector3,MagnitudeSq));
        Sys->RegisterClassMethod("Vector3","void Normalize()",asMETHOD(Vector3,Normalize));
        Sys->RegisterClassMethod("Vector3","Vector3 Normalized() const",asMETHOD(Vector3,Normalized));
        Sys->RegisterClassMethod("Vector3","Vector3 Cross(const Vector3& in) const",asMETHOD(Vector3,Cross));
        Sys->RegisterClassMethod("Vector3","Scalar Dot(const Vector3& in) const",asMETHOD(Vector3,Dot));
        Sys->RegisterClassMethod("Vector3","void ClampMagnitude(Scalar Magnitude)",asMETHODPR(Vector3,ClampMagnitude,(Scalar),void));
        Sys->RegisterClassMethod("Vector3","Vector3 ClampMagnitude(Scalar Magnitude) const",asMETHODPR(Vector3,ClampMagnitude,(Scalar) const,Vector3));
        Sys->RegisterClassMethod("Vector3","Vector2 xx() const",asMETHOD(Vector3,xx));
        Sys->RegisterClassMethod("Vector3","Vector2 xy() const",asMETHOD(Vector3,xy));
        Sys->RegisterClassMethod("Vector3","Vector2 xz() const",asMETHOD(Vector3,xz));
        Sys->RegisterClassMethod("Vector3","Vector2 yx() const",asMETHOD(Vector3,yx));
        Sys->RegisterClassMethod("Vector3","Vector2 yy() const",asMETHOD(Vector3,yy));
        Sys->RegisterClassMethod("Vector3","Vector2 yz() const",asMETHOD(Vector3,yz));
        Sys->RegisterClassMethod("Vector3","Vector2 zx() const",asMETHOD(Vector3,zx));
        Sys->RegisterClassMethod("Vector3","Vector2 zy() const",asMETHOD(Vector3,zy));
        Sys->RegisterClassMethod("Vector3","Vector2 zz() const",asMETHOD(Vector3,zz));
        
        Sys->RegisterClass("Vector4",sizeof(Vector4),asOBJ_POD | asOBJ_VALUE | asOBJ_APP_CLASS_CDAK | asOBJ_APP_CLASS_ALLFLOATS);
        Sys->RegisterClassBehavior("Vector4",asBEHAVE_CONSTRUCT,"void f(Scalar x,Scalar y,Scalar z,Scalar w)",asFUNCTIONPR(Vec4,(Scalar,Scalar,Scalar,Scalar,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Vector4",asBEHAVE_CONSTRUCT,"void f(const Vector3& in,Scalar w)",asFUNCTIONPR(Vec4,(const Vector3&,Scalar,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Vector4",asBEHAVE_CONSTRUCT,"void f(Scalar x,const Vector3& in)",asFUNCTIONPR(Vec4,(Scalar,const Vector3&,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Vector4",asBEHAVE_CONSTRUCT,"void f(const Vector4& in)",asFUNCTIONPR(Vec4,(const Vector4&,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassProperty("Vector4","Scalar x",asOFFSET(Vector4,x));
        Sys->RegisterClassProperty("Vector4","Scalar y",asOFFSET(Vector4,y));
        Sys->RegisterClassProperty("Vector4","Scalar z",asOFFSET(Vector4,z));
        Sys->RegisterClassProperty("Vector4","Scalar w",asOFFSET(Vector4,w));
        Sys->RegisterClassMethod("Vector4","Vector4 opAdd(Scalar)",asMETHODPR(Vector4,operator+,(Scalar) const,Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opAdd(const Vector4& in) const",asMETHODPR(Vector4,operator+,(const Vector4&) const,Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opSub(Scalar) const",asMETHODPR(Vector4,operator-,(Scalar) const,Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opSub(const Vector4& in) const",asMETHODPR(Vector4,operator-,(const Vector4&) const,Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opMul(Scalar) const",asMETHODPR(Vector4,operator*,(Scalar) const,Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opMul(const Vector4& in) const",asMETHODPR(Vector4,operator*,(const Vector4&) const,Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opDiv(Scalar) const",asMETHODPR(Vector4,operator/,(Scalar) const,Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opDiv(const Vector4& in) const",asMETHODPR(Vector4,operator/,(const Vector4&) const,Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opAddAssign(Scalar)",asMETHODPR(Vector4,operator+=,(Scalar),Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opAddAssign(const Vector4& in)",asMETHODPR(Vector4,operator+=,(const Vector4&),Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opSubAssign(Scalar)",asMETHODPR(Vector4,operator+=,(Scalar),Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opSubAssign(const Vector4& in)",asMETHODPR(Vector4,operator-=,(const Vector4&),Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opMulAssign(Scalar)",asMETHODPR(Vector4,operator*=,(Scalar),Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opMulAssign(const Vector4& in)",asMETHODPR(Vector4,operator*=,(const Vector4&),Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opDivAssign(Scalar)",asMETHODPR(Vector4,operator/=,(Scalar),Vector4));
        Sys->RegisterClassMethod("Vector4","Vector4 opDivAssign(const Vector4& in)",asMETHODPR(Vector4,operator/=,(const Vector4&),Vector4));
        Sys->RegisterClassMethod("Vector4","Scalar Magnitude() const",asMETHOD(Vector4,Magnitude));
        Sys->RegisterClassMethod("Vector4","Scalar MagnitudeSq() const",asMETHOD(Vector4,MagnitudeSq));
        Sys->RegisterClassMethod("Vector4","void Normalize()",asMETHOD(Vector4,Normalize));
        Sys->RegisterClassMethod("Vector4","Vector4 Normalized() const",asMETHOD(Vector4,Normalized));
        Sys->RegisterClassMethod("Vector4","Scalar Dot(const Vector4& in) const",asMETHOD(Vector4,Dot));
        Sys->RegisterClassMethod("Vector4","void ClampMagnitude(Scalar Magnitude)",asMETHODPR(Vector4,ClampMagnitude,(Scalar),void));
        Sys->RegisterClassMethod("Vector4","Vector4 ClampMagnitude(Scalar Magnitude) const",asMETHODPR(Vector4,ClampMagnitude,(Scalar) const,Vector4));Sys->RegisterClassMethod("Vector4","Vector2 xx() const",asMETHOD(Vector4,xx));
        Sys->RegisterClassMethod("Vector4","Vector2 xy() const",asMETHOD(Vector4,xy));
        Sys->RegisterClassMethod("Vector4","Vector2 xz() const",asMETHOD(Vector4,xz));
        Sys->RegisterClassMethod("Vector4","Vector2 xw() const",asMETHOD(Vector4,xw));
        Sys->RegisterClassMethod("Vector4","Vector2 yx() const",asMETHOD(Vector4,yx));
        Sys->RegisterClassMethod("Vector4","Vector2 yy() const",asMETHOD(Vector4,yy));
        Sys->RegisterClassMethod("Vector4","Vector2 yz() const",asMETHOD(Vector4,yz));
        Sys->RegisterClassMethod("Vector4","Vector2 yw() const",asMETHOD(Vector4,yw));
        Sys->RegisterClassMethod("Vector4","Vector2 zx() const",asMETHOD(Vector4,zx));
        Sys->RegisterClassMethod("Vector4","Vector2 zy() const",asMETHOD(Vector4,zy));
        Sys->RegisterClassMethod("Vector4","Vector2 zz() const",asMETHOD(Vector4,zz));
        Sys->RegisterClassMethod("Vector4","Vector2 zw() const",asMETHOD(Vector4,zw));
        #define RegShortcutFunc(f) Sys->RegisterClassMethod("Vector4","Vector3 " #f "() const",asMETHOD(Vector4,f));
        RegShortcutFunc(xxx);
        RegShortcutFunc(xxy);
        RegShortcutFunc(xxz);
        RegShortcutFunc(xxw);
        RegShortcutFunc(xyx);
        RegShortcutFunc(xyy);
        RegShortcutFunc(xyz);
        RegShortcutFunc(xyw);
        RegShortcutFunc(xzx);
        RegShortcutFunc(xzy);
        RegShortcutFunc(xzz);
        RegShortcutFunc(xzw);
        RegShortcutFunc(xwx);
        RegShortcutFunc(xwy);
        RegShortcutFunc(xwz);
        RegShortcutFunc(xww);
        RegShortcutFunc(yxx);
        RegShortcutFunc(yxy);
        RegShortcutFunc(yxz);
        RegShortcutFunc(yxw);
        RegShortcutFunc(yyx);
        RegShortcutFunc(yyy);
        RegShortcutFunc(yyz);
        RegShortcutFunc(yyw);
        RegShortcutFunc(yzx);
        RegShortcutFunc(yzy);
        RegShortcutFunc(yzz);
        RegShortcutFunc(yzw);
        RegShortcutFunc(ywx);
        RegShortcutFunc(ywy);
        RegShortcutFunc(ywz);
        RegShortcutFunc(yww);
        RegShortcutFunc(zxx);
        RegShortcutFunc(zxy);
        RegShortcutFunc(zxz);
        RegShortcutFunc(zxw);
        RegShortcutFunc(zyx);
        RegShortcutFunc(zyy);
        RegShortcutFunc(zyz);
        RegShortcutFunc(zyw);
        RegShortcutFunc(zzx);
        RegShortcutFunc(zzy);
        RegShortcutFunc(zzz);
        RegShortcutFunc(zzw);
        RegShortcutFunc(zwx);
        RegShortcutFunc(zwy);
        RegShortcutFunc(zwz);
        RegShortcutFunc(zww);
        RegShortcutFunc(wxx);
        RegShortcutFunc(wxy);
        RegShortcutFunc(wxz);
        RegShortcutFunc(wxw);
        RegShortcutFunc(wyx);
        RegShortcutFunc(wyy);
        RegShortcutFunc(wyz);
        RegShortcutFunc(wyw);
        RegShortcutFunc(wzx);
        RegShortcutFunc(wzy);
        RegShortcutFunc(wzz);
        RegShortcutFunc(wzw);
        RegShortcutFunc(wwx);
        RegShortcutFunc(wwy);
        RegShortcutFunc(wwz);
        RegShortcutFunc(www);
        
        Sys->RegisterClass("Matrix4",sizeof(Matrix4),asOBJ_POD | asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
        Sys->RegisterClassBehavior("Matrix4",asBEHAVE_CONSTRUCT,"void f(Scalar xx,Scalar xy,Scalar xz,Scalar xw,Scalar yx,Scalar yy,Scalar yz,Scalar yw,Scalar zx,Scalar zy,Scalar zz,Scalar zw,Scalar wx,Scalar wy,Scalar wz,Scalar ww)",asFUNCTION(Mat4_0),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Matrix4",asBEHAVE_CONSTRUCT,"void f(const Matrix4& in)",asFUNCTION(Mat4_1),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassProperty("Matrix4","Vector4 x",asOFFSET(Matrix4,x));
        Sys->RegisterClassProperty("Matrix4","Vector4 y",asOFFSET(Matrix4,y));
        Sys->RegisterClassProperty("Matrix4","Vector4 z",asOFFSET(Matrix4,z));
        Sys->RegisterClassProperty("Matrix4","Vector4 w",asOFFSET(Matrix4,w));
        Sys->RegisterClassMethod("Matrix4","Scalar Determinant() const",asMETHOD(Matrix4,Determinant));
        Sys->RegisterClassMethod("Matrix4","Matrix4 Inverse() const",asMETHOD(Matrix4,Inverse));
        Sys->RegisterClassMethod("Matrix4","Matrix4 Transpose() const",asMETHOD(Matrix4,Transpose));
        Sys->RegisterClassMethod("Matrix4","Matrix4 opMul(const Matrix4& in) const",asMETHODPR(Matrix4,operator*,(const Matrix4&) const,Matrix4));
        Sys->RegisterClassMethod("Matrix4","Matrix4& opMulAssign(const Matrix4& in)",asMETHODPR(Matrix4,operator*=,(const Matrix4&),Matrix4&));
        Sys->RegisterClassMethod("Matrix4","Vector4 opMul(const Vector4& in) const",asMETHODPR(Matrix4,operator*,(const Vector4&) const,Vector4));
        Sys->RegisterClassMethod("Matrix4","Vector3 opMul(const Vector3& in) const",asMETHODPR(Matrix4,operator*,(const Vector3&) const,Vector3));
        
        Sys->RegisterFunction("Matrix4 Scale(const Vector3& in)",asFUNCTIONPR(Scale,(const Vector3&),Matrix4));
        Sys->RegisterFunction("Matrix4 Scale(Scalar s)",asFUNCTIONPR(Scale,(Scalar),Matrix4));
        Sys->RegisterFunction("Matrix4 Translation(const Vector3& in)",asFUNCTION(Translation));
        Sys->RegisterFunction("Matrix4 Rotation(const Vector3& in,Scalar a,const Vector3& in)",asFUNCTIONPR(Rotation,(const Vector3&,Scalar,const Vector3&),Matrix4));
        Sys->RegisterFunction("Matrix4 Rotation(Scalar y,Scalar p,Scalar r)",asFUNCTIONPR(Rotation,(Scalar,Scalar,Scalar),Matrix4));
        Sys->RegisterFunction("Matrix4 Rotation(const Vector3& in,Scalar a)",asFUNCTIONPR(Rotation,(const Vector3&,Scalar),Matrix4));
        Sys->RegisterFunction("Matrix4 Rotation(const Vector3& in,const Vector3& in,const Vector3& in)",asFUNCTIONPR(Rotation,(const Vector3&,const Vector3&,const Vector3&),Matrix4));
        Sys->RegisterFunction("Matrix4 RotationX(Scalar a)",asFUNCTION(RotationX));
        Sys->RegisterFunction("Matrix4 RotationY(Scalar a)",asFUNCTION(RotationY));
        Sys->RegisterFunction("Matrix4 RotationZ(Scalar a)",asFUNCTION(RotationZ));
        Sys->RegisterFunction("Matrix4 Camera(const Vector3& in,const Vector3& in,const Vector3& in,const Vector3& in)",asFUNCTION(Camera));
        Sys->RegisterFunction("Matrix4 Camera(const Vector3& in,const Vector3& in,const Vector3& in)",asFUNCTION(LookAt));
        Sys->RegisterFunction("Matrix4 Orthogonal(Scalar w,Scalar h,Scalar n,Scalar f)",asFUNCTIONPR(Orthogonal,(Scalar,Scalar,Scalar,Scalar),Matrix4));
        Sys->RegisterFunction("Matrix4 Orthogonal(Scalar l,Scalar r,Scalar t,Scalar b,Scalar n,Scalar f)",asFUNCTIONPR(Orthogonal,(Scalar,Scalar,Scalar,Scalar,Scalar,Scalar),Matrix4));
        Sys->RegisterFunction("Matrix4 Perspective(Scalar w,Scalar h,Scalar n,Scalar f)",asFUNCTION(Perspective));
        Sys->RegisterFunction("Matrix4 PerspectiveFov(Scalar f,Scalar w,Scalar h,Scalar n,Scalar f)",asFUNCTION(PerspectiveFov));
        Sys->RegisterFunction("Matrix4 PerspectiveMultiFov(Scalar fx,Scalar fy,Scalar w,Scalar h,Scalar n,Scalar f)",asFUNCTION(PerspectiveMultiFov));
        Sys->RegisterFunction("Matrix4 Viewport(Scalar w,Scalar h)",asFUNCTION(Viewport));
        
        Sys->RegisterClass("Quaternion",sizeof(Quaternion),asOBJ_POD | asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
        Sys->RegisterClassBehavior("Quaternion",asBEHAVE_CONSTRUCT,"void f(Scalar x,Scalar y,Scalar z,Scalar w)",asFUNCTIONPR(Quat,(Scalar,Scalar,Scalar,Scalar,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Quaternion",asBEHAVE_CONSTRUCT,"void f(const Vector3& in,Scalar w)",asFUNCTIONPR(Quat,(const Vector3&,Scalar,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Quaternion",asBEHAVE_CONSTRUCT,"void f(const Quaternion& in)",asFUNCTIONPR(Quat,(const Quaternion&,void*),void),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassProperty("Quaternion","Scalar x",asOFFSET(Quaternion,x));
        Sys->RegisterClassProperty("Quaternion","Scalar y",asOFFSET(Quaternion,y));
        Sys->RegisterClassProperty("Quaternion","Scalar z",asOFFSET(Quaternion,z));
        Sys->RegisterClassProperty("Quaternion","Scalar w",asOFFSET(Quaternion,w));
        Sys->RegisterClassMethod("Quaternion","Matrix4 ToMatrix() const",asMETHOD(Quaternion,ToMatrix));
        Sys->RegisterClassMethod("Quaternion","Quaternion opMul(const Quaternion& in) const",asMETHODPR(Quaternion,operator*,(const Quaternion&) const,Quaternion));
        Sys->RegisterClassMethod("Quaternion","Vector3 opMul(const Vector3& in) const",asMETHODPR(Quaternion,operator*,(const Vector3&) const,Vector3));
        Sys->RegisterClassMethod("Quaternion","Quaternion& opMulAssign(const Quaternion& in)",asMETHODPR(Quaternion,operator*=,(const Quaternion&),Quaternion&));
        
        Sys->RegisterFunction("Vector3 ColorFunc(Scalar Factor)",asFUNCTION(ColorFunc));
    }
};