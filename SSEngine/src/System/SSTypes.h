#ifndef BC_TYPES_H
#define BC_TYPES_H

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <SFML/Network.hpp>

#define SSENGINE_VERSION_MAJOR 0
#define SSENGINE_VERSION_MINOR 2

#define MakeCString(s) new char[s]
#define FreeCString(s) delete [] s

typedef sf::Uint8           u8;
typedef sf::Uint16          u16;
typedef sf::Uint32          u32;
typedef sf::Uint64          u64;
typedef uintptr_t           Ptr;

typedef sf::Int8            i8;
typedef sf::Int16           i16;
typedef sf::Int32           i32;
typedef sf::Int64           i64;

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
typedef u32                 UID;

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
