//
//  Engine.cpp
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#include <Core/Timer.h>
#include <GLFW/glfw3.h>

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
        m_StartTime = glfwGetTime();
    }
    void Timer::Stop()
    {
        m_Started = false;
    }
    
    Scalar Timer::ElapsedTime() const
    {
        return glfwGetTime() - m_StartTime;
        return Scalar(0);
    }
}
