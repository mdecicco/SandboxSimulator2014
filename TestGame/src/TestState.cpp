#include <TestState.h>
#include <Engine.h>

#include <Rendering/RenderSystem.h>
#include <Rendering/Gui/Gui.h>
#include <Core/PhysicsSystem.h>

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
        TransformComponent* ET = new TransformComponent();
        er = new RenderComponent();
        m_Engine->GetSceneGraph()->AddComponent(E, er);
        m_Engine->GetSceneGraph()->AddComponent(E, ET);
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        r->SetShape(RC_SKY_SPHERE);

        Entity* G = m_Engine->GetSceneGraph()->CreateEntity();
        m_Engine->GetSceneGraph()->AddComponent(G, new RenderComponent());
        m_Engine->GetSceneGraph()->AddComponent(G, new TransformComponent());
        RenderComponent* gr = (RenderComponent*)G->GetComponentByType(CT_RENDER);
        gr->SetShape(RC_GROUND_PLANE);
        TransformComponent* gt = (TransformComponent*)G->GetComponentByType(CT_TRANSFORM);
        gt->SetPosition(Vec3(0,-2,0));
        gt->SetStatic(true);
        m_Engine->GetPhysicsSystem()->MakePhysical(G, 0.0, false, 0.5, 1.0);

        Entity* Po = m_Engine->GetSceneGraph()->CreateEntity();
        m_Engine->GetSceneGraph()->AddComponent(Po, new RenderComponent());
        m_Engine->GetSceneGraph()->AddComponent(Po, new TransformComponent());
        RenderComponent* por = (RenderComponent*)Po->GetComponentByType(CT_RENDER);
        por->SetShape(RC_BOX);
        TransformComponent* pot = (TransformComponent*)Po->GetComponentByType(CT_TRANSFORM);
        pot->SetPosition(Vec3(0,6,-8));
        m_Engine->GetPhysicsSystem()->MakePhysical(Po, 1, false, 0.1f, 5.0f);

        Entity* P = m_Engine->GetSceneGraph()->CreateEntity();
        m_Engine->GetSceneGraph()->AddComponent(P, new RenderComponent());
        m_Engine->GetSceneGraph()->AddComponent(P, new TransformComponent());
        pr = (RenderComponent*)P->GetComponentByType(CT_RENDER);
        pr->SetShape(RC_SPHERE);
        SphereTrans = (TransformComponent*)P->GetComponentByType(CT_TRANSFORM);
        SphereTrans->SetPosition(Vec3(0,6,0));
        m_Engine->GetPhysicsSystem()->MakePhysical(P, 1, true, 0.0f, 2.0f);
        m_Sphere = (PhysicsComponent*)P->GetComponentByType(CT_PHYSICS);
        m_Sphere->SetSleepWhenActive(false);
        m_Sphere->SetAngularFactor(Vec3(0,0,0));

        Font* f = new Font();
        f->Load("Data/Font/Ubuntu-better.dst");
        //f->AddToMesh(pr->GetMesh(), "Hello world!", Vec3(0,0,0), 56);


        Entity* Cam = m_Engine->GetSceneGraph()->CreateEntity();
        CamTrans = new TransformComponent();
        m_Engine->GetSceneGraph()->AddComponent(Cam, CamTrans);
        m_Engine->GetSceneGraph()->AddComponent(Cam, new CameraComponent());
        m_Engine->GetRenderSystem()->SetCamera(Cam);
        CamTrans->SetFirstPerson(true);
        m_Engine->GetInputSystem()->SetMousePosition(Vec2(1280/2,720/2));
        CamTrans->SetPosition(Vec3(0,1,3));
        CamTrans->Rotate(1,0,0,-20);
        CamTrans->SetRelativeTo(P);
        ET->SetRelativeTo(Cam);
    }

    void TestState::Update(Scalar dt)
    {
        //TransformComponent* trans = (TransformComponent*)m_Engine->GetSceneGraph()->GetEntityById(0)->GetComponentByType(CT_TRANSFORM);
        Vec3 Delta = Vec3();

        if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_W)) {
            Delta += Vec3(0,0,-0.025);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_S)) {
            Delta += Vec3(0,0,0.025);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_A)) {
            Delta += Vec3(-0.025,0,0);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_D)) {
            Delta += Vec3(0.025,0,0);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_SPACE)) {
            Delta += Vec3(0,0.025,0);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_Q)) {
            m_Engine->SetTimeOfDay(m_Engine->GetTimeOfDay() + ((3000*dt) * 0.00001157407f));
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_E)) {
            m_Engine->SetTimeOfDay(m_Engine->GetTimeOfDay() - ((3000*dt) * 0.00001157407f));
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_Z)) {
            pr->ReloadShader();
            er->ReloadShader();
        }

        Vec2 MousePos = m_Engine->GetInputSystem()->GetMousePosition();
        MousePos.x -= 1280/2;
        MousePos.y -= 720/2;
        if(MousePos.x + MousePos.y != 0) {
            Vec3 yAxis = Vec3(1,0,0);
            Vec3 xAxis = Vec3(0,1,0);

            SphereTrans->Rotate(xAxis,-MousePos.x*0.5f);
            //CamTrans->Rotate(yAxis,-MousePos.y*0.5f);
        }

        m_Engine->GetInputSystem()->SetMousePosition(Vec2(1280/2,720/2));
        SphereTrans->AddImpulse(Delta, true);
    }
}