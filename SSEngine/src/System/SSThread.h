#ifndef SandboxSimulator_Thread_h
#define SandboxSimulator_Thread_h

#include <SFML/System.hpp>

namespace SandboxSimulator {
class Thread
{
    public:
        Thread();
        virtual ~Thread() {}

        void Start();
        void Join();

    private:
        virtual void run() = 0;
        sf::Thread m_Thread;
};
}
#endif