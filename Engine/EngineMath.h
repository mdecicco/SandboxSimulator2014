/*
 *  EngineMath.h
 *
 *  Created by Michael DeCicco on 7/5/14.
 *
 */

#pragma once
#include "EngineTypes.h"
#include "btVector3.h"
#include "btQuaternion.h"
#define PI_OVER_180 0.01745329251
#define PI_UNDER_180 57.2957795131

#include <string>
using namespace std;

namespace Reality
{
    class Vector2
    {
        public:
            Vector2();
            Vector2(Scalar x,Scalar y);
        
            Vector2 operator-() const { return Vector2(-x,-y); }
            Vector2 operator+(Scalar rval) const;
            Vector2 operator-(Scalar rval) const;
            Vector2 operator*(Scalar rval) const;
            Vector2 operator/(Scalar rval) const;
            Vector2 operator+=(Scalar rval);
            Vector2 operator-=(Scalar rval);
            Vector2 operator*=(Scalar rval);
            Vector2 operator/=(Scalar rval);
        
            Vector2 operator+(const Vector2& rval) const;
            Vector2 operator-(const Vector2& rval) const;
            Vector2 operator*(const Vector2& rval) const;
            Vector2 operator/(const Vector2& rval) const;
            Vector2 operator+=(const Vector2& rval);
            Vector2 operator-=(const Vector2& rval);
            Vector2 operator*=(const Vector2& rval);
            Vector2 operator/=(const Vector2& rval);
        
            Scalar Magnitude() const;
            Scalar MagnitudeSq() const;
            void Normalize();
            Vector2 Normalized() const;
            Vector2 Perpendicular() const;
            Scalar Dot(const Vector2& rval) const;
            Scalar Angle() const;
            void ClampMagnitude(Scalar Magnitude);
            Vector2 ClampMagnitude(Scalar Magnitude) const;
        
            Scalar x;
            Scalar y;
    };
    
    class Vector3
    {
        public:
            Vector3();
            Vector3(Scalar x,Scalar y,Scalar z);
            Vector3(const Vector2& xy,Scalar z);
            Vector3(Scalar x,const Vector2& yz);
            Vector3(const Vector3& xyz);
            Vector3(const btVector3& xyz);
        
            operator btVector3() const { return btVector3(x,y,z); }
        
            Vector3& operator=(const Vector3& rval);
        
            Vector3 operator-() const { return Vector3(-x,-y,-z); }
            Vector3 operator+(Scalar rval) const;
            Vector3 operator-(Scalar rval) const;
            Vector3 operator*(Scalar rval) const;
            Vector3 operator/(Scalar rval) const;
            Vector3& operator+=(Scalar rval);
            Vector3& operator-=(Scalar rval);
            Vector3& operator*=(Scalar rval);
            Vector3& operator/=(Scalar rval);
        
            Vector3 operator+(const Vector3& rval) const;
            Vector3 operator-(const Vector3& rval) const;
            Vector3 operator*(const Vector3& rval) const;
            Vector3 operator/(const Vector3& rval) const;
            Vector3& operator+=(const Vector3& rval);
            Vector3& operator-=(const Vector3& rval);
            Vector3& operator*=(const Vector3& rval);
            Vector3& operator/=(const Vector3& rval);
        
            Scalar Magnitude() const;
            Scalar MagnitudeSq() const;
            void Normalize();
            Vector3 Normalized() const;
            Vector3 Cross(const Vector3& rval) const;
            Scalar Dot(const Vector3& rval) const;
            void ClampMagnitude(Scalar Magnitude);
            Vector3 ClampMagnitude(Scalar Magnitude) const;
            Scalar Angle(const Vector3& With) const;
        
            Vector2 xx() const;
            Vector2 xy() const;
            Vector2 xz() const;
            Vector2 yx() const;
            Vector2 yy() const;
            Vector2 yz() const;
            Vector2 zx() const;
            Vector2 zy() const;
            Vector2 zz() const;
        
            Scalar x;
            Scalar y;
            Scalar z;
    };
    
    class Vector4
    {
        public:
            Vector4();
            Vector4(Scalar x,Scalar y,Scalar z,Scalar w);
            Vector4(const Vector3& xyz,Scalar w);
            Vector4(Scalar x,const Vector3& yzw);
        
            operator btQuaternion() const { return btQuaternion(x,y,z,w); }
        
            Vector4 operator-() const { return Vector4(-x,-y,-z,-w); }
            Vector4 operator+(Scalar rval) const;
            Vector4 operator-(Scalar rval) const;
            Vector4 operator*(Scalar rval) const;
            Vector4 operator/(Scalar rval) const;
            Vector4 operator+=(Scalar rval);
            Vector4 operator-=(Scalar rval);
            Vector4 operator*=(Scalar rval);
            Vector4 operator/=(Scalar rval);
        
            Vector4 operator+(const Vector4& rval) const;
            Vector4 operator-(const Vector4& rval) const;
            Vector4 operator*(const Vector4& rval) const;
            Vector4 operator/(const Vector4& rval) const;
            Vector4 operator+=(const Vector4& rval);
            Vector4 operator-=(const Vector4& rval);
            Vector4 operator*=(const Vector4& rval);
            Vector4 operator/=(const Vector4& rval);
        
            Scalar Magnitude() const;
            Scalar MagnitudeSq() const;
            void Normalize();
            Vector4 Normalized() const;
            Scalar Dot(const Vector4& rval) const;
            void ClampMagnitude(Scalar Magnitude);
            Vector4 ClampMagnitude(Scalar Magnitude) const;
        
            Scalar& operator[](i32 cIdx) { return (&x)[cIdx]; }
        
            Vector2 xx() const;
            Vector2 xy() const;
            Vector2 xz() const;
            Vector2 xw() const;
            Vector2 yx() const;
            Vector2 yy() const;
            Vector2 yz() const;
            Vector2 yw() const;
            Vector2 zx() const;
            Vector2 zy() const;
            Vector2 zz() const;
            Vector2 zw() const;
            Vector3 xxx() const;
            Vector3 xxy() const;
            Vector3 xxz() const;
            Vector3 xxw() const;
            Vector3 xyx() const;
            Vector3 xyy() const;
            Vector3 xyz() const;
            Vector3 xyw() const;
            Vector3 xzx() const;
            Vector3 xzy() const;
            Vector3 xzz() const;
            Vector3 xzw() const;
            Vector3 xwx() const;
            Vector3 xwy() const;
            Vector3 xwz() const;
            Vector3 xww() const;
            Vector3 yxx() const;
            Vector3 yxy() const;
            Vector3 yxz() const;
            Vector3 yxw() const;
            Vector3 yyx() const;
            Vector3 yyy() const;
            Vector3 yyz() const;
            Vector3 yyw() const;
            Vector3 yzx() const;
            Vector3 yzy() const;
            Vector3 yzz() const;
            Vector3 yzw() const;
            Vector3 ywx() const;
            Vector3 ywy() const;
            Vector3 ywz() const;
            Vector3 yww() const;
            Vector3 zxx() const;
            Vector3 zxy() const;
            Vector3 zxz() const;
            Vector3 zxw() const;
            Vector3 zyx() const;
            Vector3 zyy() const;
            Vector3 zyz() const;
            Vector3 zyw() const;
            Vector3 zzx() const;
            Vector3 zzy() const;
            Vector3 zzz() const;
            Vector3 zzw() const;
            Vector3 zwx() const;
            Vector3 zwy() const;
            Vector3 zwz() const;
            Vector3 zww() const;
            Vector3 wxx() const;
            Vector3 wxy() const;
            Vector3 wxz() const;
            Vector3 wxw() const;
            Vector3 wyx() const;
            Vector3 wyy() const;
            Vector3 wyz() const;
            Vector3 wyw() const;
            Vector3 wzx() const;
            Vector3 wzy() const;
            Vector3 wzz() const;
            Vector3 wzw() const;
            Vector3 wwx() const;
            Vector3 wwy() const;
            Vector3 wwz() const;
            Vector3 www() const;
        
            Scalar x;
            Scalar y;
            Scalar z;
            Scalar w;
    };
    
    class Matrix4
    {
        public:
            Matrix4();
            Matrix4(Scalar xx,Scalar xy,Scalar xz,Scalar xw,
                    Scalar yx,Scalar yy,Scalar yz,Scalar yw,
                    Scalar zx,Scalar zy,Scalar zz,Scalar zw,
                    Scalar wx,Scalar wy,Scalar wz,Scalar ww);
            Matrix4(const Matrix4& m);
            ~Matrix4();
        
            Scalar Determinant() const;
            Matrix4 Transpose() const;
            Matrix4 Inverse() const;
        
            Matrix4 operator*(const Matrix4& rVal) const;
            Matrix4& operator *=(const Matrix4& rVal);
            Vector4 operator*(const Vector4& rVal) const;
            Vector3 operator*(const Vector3& rVal) const;
        
            Vector4& operator[](i32 rIdx) { return (&x)[rIdx]; }
            Vector4 operator[](i32 rIdx) const { return (&x)[rIdx]; }
        
            Vector4 x;
            Vector4 y;
            Vector4 z;
            Vector4 w;
        
            static Matrix4 Identity;
    };
    
    Matrix4 Scale(const Vector3& ScaleFactors);
    Matrix4 Scale(Scalar ScaleFactor);
    Matrix4 Translation(const Vector3& Pos);
    Matrix4 Rotation(const Vector3& Axis,Scalar Angle,const Vector3& Center);
    Matrix4 Rotation(const Vector3& Axis,Scalar Angle);
    Matrix4 Rotation(Scalar Yaw,Scalar Pitch,Scalar Roll);
    Matrix4 Rotation(const Vector3& Basis1,const Vector3& Basis2,const Vector3& Basis3);
    Matrix4 RotationX(Scalar Theta);
    Matrix4 RotationY(Scalar Theta);
    Matrix4 RotationZ(Scalar Theta);
    Matrix4 Camera(const Vector3& Eye,const Vector3& Look,const Vector3& Up,const Vector3& Right);
    Matrix4 LookAt(const Vector3& Eye,const Vector3& At,const Vector3& Up);
    Matrix4 Orthogonal(Scalar Width,Scalar Height,Scalar Near,Scalar Far);
    Matrix4 Orthogonal(Scalar Left,Scalar Right,Scalar Top,Scalar Bottom, Scalar Near, Scalar Far);
    Matrix4 Perspective(Scalar Width,Scalar Height,Scalar Near,Scalar Far);
    Matrix4 PerspectiveFov(Scalar FOV,Scalar Aspect,Scalar Near,Scalar Far);
    Matrix4 PerspectiveMultiFov(Scalar FovX,Scalar FovY,Scalar Near,Scalar Far);
    Matrix4 Viewport(Scalar Width,Scalar Height);
    
    class Quaternion
    {
        public:
            Quaternion();
            Quaternion(Scalar Ax,Scalar Ay,Scalar Az,Scalar Angle);
            Quaternion(const Vector3& Axis,Scalar Angle);
            Quaternion(const Quaternion& q);
            ~Quaternion();
            
            operator btQuaternion() const { return btQuaternion(x,y,z,w); }
        
            void FromAxisAngle(Scalar Ax,Scalar Ay,Scalar Az,Scalar Angle);
            void FromAxisAngle(const Vector3& Axis,Scalar Angle);
        
            Matrix4 ToMatrix() const;
        
            Quaternion operator *(const Quaternion& rVal) const;
            Vector3 operator *(const Vector3& rVal) const;
            Quaternion& operator *=(const Quaternion& rVal);
        
            Scalar x;
            Scalar y;
            Scalar z;
            Scalar w;
    };
    
    class iv2
    {
        public:
            iv2() : x(0), y(0) { }
            iv2(i32 _x,i32 _y) : x(_x), y(_y) { }
            ~iv2() { }
            
            i32 x,y;
    };
    class iv3
    {
        public:
            iv3() : x(0), y(0), z(0) { }
            iv3(i32 _x,i32 _y,i32 _z) : x(_x), y(_y), z(_z) { }
            ~iv3() { }
            
            i32 x,y,z;
    };
    class iv4
    {
        public:
            iv4() : x(0), y(0), z(0), w(0) { }
            iv4(i32 _x,i32 _y,i32 _z,i32 _w) : x(_x), y(_y), z(_z), w(_w) { }
            ~iv4() { }
            
            i32 x,y,z,w;
    };
    
    class BoundingVolume
    {
        public:
            BoundingVolume() : TargetCenter(0), m_LastRejectedPlaneIndex(6) { }
            BoundingVolume(Scalar x,Scalar y,Scalar z) : Dimensions(x,y,z), TargetCenter(0) { }
            BoundingVolume(const Vector3& _Dimensions) : Dimensions(_Dimensions), TargetCenter(0) { }
            ~BoundingVolume() { }
        
            bool IntersectsVolume(const BoundingVolume& v) const;
            u8 IntersectsFrustum(Vector4* Planes);
            bool IsPointInside(const Vector3& p) const;
        
            Vector3* TargetCenter;
            Vector3  Dimensions;
        
        protected:
            friend class SceneOctree;
            u8 m_LastRejectedPlaneIndex;
    };
    
    void SeedRandom(i32 Seed);
    Scalar Random(Scalar Min,Scalar Max);
    Vector3 ColorFunc(Scalar Factor);
    
    void RegisterScriptMath();
};