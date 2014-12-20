//
//  Engine.cpp
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#include <Core/Timer.h>

namespace SandboxSimulator
{
    Timer::Timer() : m_Started(false)
    {
    }
    Timer::~Timer()
    {
    }
    
    void Timer::Start()
    {
        if(m_Started) return;
        m_Started = true;
        //m_StartTime = std::chrono::high_resolution_clock::now();
    }
    void Timer::Stop()
    {
        m_Started = false;
    }
    
    Scalar Timer::ElapsedTime() const
    {
        //if(m_Started) return std::chrono::duration<Scalar>(std::chrono::high_resolution_clock::now() - m_StartTime).count();
        return Scalar(0);
    }
    Timer::operator Scalar() const
    {
        //if(m_Started) return std::chrono::duration<Scalar>(std::chrono::high_resolution_clock::now() - m_StartTime).count();
        return Scalar(0);
    }
}