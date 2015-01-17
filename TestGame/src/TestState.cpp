#include <TestState.h>
#include <Engine.h>

#include <Core/TransformComponent.h>
#include <Rendering/RenderSystem.h>

namespace SandboxSimulator
{
    TestState::TestState()
    {}

    TestState::~TestState()
    {}

    void TestState::Init(StateManagerSystem *StateManager)
    {
        m_StateManager = StateManager;
        Entity* E = m_Engine->GetSceneGraph()->CreateEntity();
        m_Engine->GetSceneGraph()->AddComponent(E, new RenderComponent());
        m_Engine->GetSceneGraph()->AddComponent(E, new TransformComponent());
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        r->SetShape(RC_SQUARE);
        TransformComponent* t = (TransformComponent*)E->GetComponentByType(CT_TRANSFORM);
        t->Translate(Vec3(0,0,-2));
    }

    void TestState::Update(Scalar dt)
    {
        TransformComponent* trans = (TransformComponent*)m_Engine->GetSceneGraph()->GetEntityById(0)->GetComponentByType(CT_TRANSFORM);
        Vec3 Pos = trans->GetPosition();

        if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_W)) {
            Pos += Vec3(0,0,-1*dt);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_S)) {
            Pos += Vec3(0,0,1*dt);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_A)) {
            Pos += Vec3(-1*dt,0,0);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_D)) {
            Pos += Vec3(1*dt,0,0);
        }

        trans->SetPosition(Pos);
    }
}