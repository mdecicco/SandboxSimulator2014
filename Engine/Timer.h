/*
 *  Timer.h
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#pragma once
#include "EngineTypes.h"
#include <chrono>

namespace Reality
{
    class Timer
    {
        public:
            Timer();
            ~Timer();
        
            void Start();
            void Stop();
        
            Scalar ElapsedTime() const;
            operator Scalar() const;
        
        protected:
            std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
            Flag m_Started;
    };
};