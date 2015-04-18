#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

#include <Engine.h>
#include <Core/StateSystem.h>

using namespace SandboxSimulator;
namespace LudumDare {
class HelloWorld : public State
{
    public:
        HelloWorld();
        ~HelloWorld();

    protected:
        virtual void Init(StateManagerSystem* Manager);
        virtual void Update(Scalar dt);

    private:
        StateManagerSystem* m_Manager;
};
}

#endif