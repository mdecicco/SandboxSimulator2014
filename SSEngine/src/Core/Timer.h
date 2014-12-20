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
#include <chrono>

namespace SandboxSimulator
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
        //std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
        Flag m_Started;
    };
}

#endif
