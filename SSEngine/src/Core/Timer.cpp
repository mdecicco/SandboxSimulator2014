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
        m_Clock = new sf::Clock();
    }

    Timer::~Timer()
    {
    }
    
    void Timer::Start()
    {
        if(m_Started) return;
        m_Clock->restart();
        m_StartTime = m_Clock->getElapsedTime().asSeconds();
    }
    void Timer::Stop()
    {
        m_Clock->restart();
        m_Started = false;
    }
    
    Scalar Timer::ElapsedTime() const
    {
        return m_Clock->getElapsedTime().asSeconds();
        return Scalar(0);
    }
}
