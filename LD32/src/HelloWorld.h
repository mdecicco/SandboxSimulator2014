#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

#include <Engine.h>
#include <Core/StateSystem.h>
#include <Rendering/Mesh.h>
#include <Rendering/Gui/Font.h>

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
        Mesh* _TimerMesh;
        int _TimeLeft;
        Font* F;
};
}

#endif