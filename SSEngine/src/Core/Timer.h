//
//  Timer.h
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#ifndef SandboxSimulator_Timer_h
#define SandboxSimulator_Timer_h

#include <System/SSTypes.h>
#include <SFML/System.hpp>
#include <chrono>

namespace SandboxSimulator
{
    class RenderSystem;
    class Timer
    {
        public:
            Timer();
            ~Timer();
        
            void Start();
            void Stop();
        
            Scalar ElapsedTime() const;
        
        protected:
            Scalar m_StartTime;
            sf::Clock* m_Clock;
            Flag m_Started;
    };
}

#endif
