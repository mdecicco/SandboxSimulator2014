#include <TestState.h>
#include <Engine.h>

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
        r->SetShape(RC_SKY_SPHERE);

        Entity* G = m_Engine->GetSceneGraph()->CreateEntity();
        m_Engine->GetSceneGraph()->AddComponent(G, new RenderComponent());
        m_Engine->GetSceneGraph()->AddComponent(G, new TransformComponent());
        RenderComponent* gr = (RenderComponent*)G->GetComponentByType(CT_RENDER);
        gr->SetShape(RC_GROUND_PLANE);
        TransformComponent* gt = (TransformComponent*)G->GetComponentByType(CT_TRANSFORM);
        gt->SetPosition(Vec3(0,-2,0));

        Entity* Cam = m_Engine->GetSceneGraph()->CreateEntity();
        CamTrans = new TransformComponent();
        m_Engine->GetSceneGraph()->AddComponent(Cam, CamTrans);
        m_Engine->GetSceneGraph()->AddComponent(Cam, new CameraComponent());
        m_Engine->GetRenderSystem()->SetCamera(Cam);
        CamTrans->SetFirstPerson(true);
        m_Engine->GetInputSystem()->SetMousePosition(Vec2(400,300));
    }

    void TestState::Update(Scalar dt)
    {
        //TransformComponent* trans = (TransformComponent*)m_Engine->GetSceneGraph()->GetEntityById(0)->GetComponentByType(CT_TRANSFORM);
        Vec3 Pos = Vec3();

        if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_W)) {
            Pos += Vec3(0,0,-1*dt);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_S)) {
            Pos += Vec3(0,0,1*dt);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_A)) {
            Pos += Vec3(-1*dt,0,0);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_D)) {
            Pos += Vec3(1*dt,0,0);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_UP)) {
            m_Engine->SetGameTimeRate(m_Engine->GetGameTimeRate()+1);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_DOWN)) {
            m_Engine->SetGameTimeRate(m_Engine->GetGameTimeRate()-1);
        }

        Vec2 MousePos = m_Engine->GetInputSystem()->GetMousePosition();
        MousePos.x -= 400;
        MousePos.y -= 300;
        if(MousePos.x + MousePos.y != 0) {
            Vec3 yAxis = Vec3(1,0,0);
            Vec3 xAxis = Vec3(0,1,0);

            CamTrans->Rotate(xAxis,-MousePos.x*0.5f);
            CamTrans->Rotate(yAxis,-MousePos.y*0.5f);
        }

        m_Engine->GetInputSystem()->SetMousePosition(Vec2(400,300));
        CamTrans->Translate(Pos);
    }
}