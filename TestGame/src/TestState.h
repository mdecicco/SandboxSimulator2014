#ifndef SandboxSimulator_GameState_h
#define SandboxSimulator_GameState_h

#include <Core/StateSystem.h>
#include <Core/TransformComponent.h>
#include <Core/PhysicsSystem.h>

namespace SandboxSimulator {
class RenderComponent;

class TestState : public State
{
    public:
        TestState();
        ~TestState();

    protected:
        virtual void Init(StateManagerSystem* Manager);
        virtual void Update(Scalar dt);
        StateManagerSystem* m_StateManager;

    private:
        TransformComponent* CamTrans;
        TransformComponent* SphereTrans;
        RenderComponent* pr;
        RenderComponent* er;
        PhysicsComponent* m_Sphere;
};
}

#endif
