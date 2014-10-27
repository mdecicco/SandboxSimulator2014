/*
 *  Log.cpp
 *
 *  Created by Michael DeCicco on 6/27/14.
 *
 *  Reason:
 *   Some files need to be able to log but can't include Engine.h
 */

#include "Log.h"
#include "Engine.h"

namespace Reality
{
    void Log(Literal Text,...)
    {
        va_list Args;
        va_start(Args,Text);
        Engine::Log(Text,Args);
        va_end(Args);
    }
};