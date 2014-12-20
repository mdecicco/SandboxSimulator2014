#ifndef BC_TYPES_H
#define BC_TYPES_H

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define SSENGINE_VERSION_MAJOR 0
#define SSENGINE_VERSION_MINOR 1

#define MakeCString(s) new char[s]
#define FreeCString(s) delete [] s

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
typedef uintptr_t           Ptr;

typedef int8_t              i8;
typedef int16_t             i16;
typedef int32_t             i32;
typedef int64_t             i64;

typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;

typedef float               f32;
typedef double              f64;

typedef char                Byte;
typedef u8                  uByte;

typedef const char*         Literal;
typedef char*               CString;
typedef f32                 Scalar;
typedef bool                Flag;
typedef i32                 UID;

//Useful
typedef unsigned short      uid;

#ifdef __gnu_linux__
    #define INT32_MAX    __INT32_MAX__
#endif

#define _USE_32BIT_INDICES_
#ifdef _USE_32BIT_INDICES_
    typedef i32         Index;
    #define INDEX_NULL  INT32_MAX
#else
    typedef i16         Index;
    #define INDEX_NULL  INT16_MAX
#endif

#endif
