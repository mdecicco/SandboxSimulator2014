#ifndef MATH_H
#define MATH_H

#include <Utils/Math/Functions.h>
#include <Utils/Math/Vec3.h>
#include <Utils/Math/Mat4.h>
#include <Utils/Math/Mat3.h>
#include <Utils/Math/Quat.h>
#include <Utils/Math/Vec2.h>
#include <Utils/Math/Vec4.h>

namespace SandboxSimulator {
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
}

#endif
