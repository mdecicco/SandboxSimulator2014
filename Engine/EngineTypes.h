/*
 *  EngineTypes.h
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#pragma once
#include <stdint.h>
#include "EngineMacros.h"
#include "EngineConfig.h"

namespace Reality
{
    typedef int8_t      i8;
    typedef int16_t     i16;
    typedef int32_t     i32;
    typedef int64_t     i64;
    
    typedef uint8_t     u8;
    typedef uint16_t    u16;
    typedef uint32_t    u32;
    typedef uint64_t    u64;
    
    typedef float       f32;
    typedef double      f64;
    
    typedef int8_t      Byte;
    typedef uint8_t     uByte;
    
    typedef const char* Literal;
    typedef char*       CString;
    #ifdef USE_DOUBLE_SCALAR
    typedef f64         Scalar;
    #else
    typedef f32         Scalar;
    #endif
    typedef bool        Flag;
    typedef i32         UID;
    
    enum UNIT
    {
        GIGAPARSEC,
        MEGAPARSEC,
        KILOPARSEC,
        PARSEC,
        MILLIPARSEC,
        MICROPARSEC,
        NANOPARSEC,
        PICOPARSEC,
        FEMTOPARSEC,
        ATTOPARSEC,
        GIGAMETER,
        MEGAMETER,
        KILOMETER,
        METER,
        SECOND,
        MINUTE,
        HOUR,
        DAY,
        WEEK,
        MONTH,
        YEAR,
        DECADE,
        CENTURY,
        MILLENNIUM,
        SUPPORTED_UNIT_NUM
    };
};