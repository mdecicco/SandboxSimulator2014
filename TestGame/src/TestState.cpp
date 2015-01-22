#include <TestState.h>
#include <Engine.h>

#include <Rendering/RenderSystem.h>
#include <Rendering/Gui/Gui.h>

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

        Entity* P = m_Engine->GetSceneGraph()->CreateEntity();
        m_Engine->GetSceneGraph()->AddComponent(P, new RenderComponent());
        m_Engine->GetSceneGraph()->AddComponent(P, new TransformComponent());
        pr = (RenderComponent*)P->GetComponentByType(CT_RENDER);
        pr->SetShape(RC_UI);
        //pr->SetPrimType(GL_LINES);
        TransformComponent* pt = (TransformComponent*)P->GetComponentByType(CT_TRANSFORM);
        //pt->Rotate(Quat(0,0,1,10));

        Font* f = new Font();
        f->Load("Data/Font/Ubuntu-better.dst");
        //f->AddTextToEntity(P, "Hello world!", Vec3(0,0,0), 1);
        GUIManager* Manager = new GUIManager(f);
        GuiElement* TestPanel = Manager->MakeElement(pr);
        TestPanel->SetSize(Vec2(200,200));
        pt->SetPosition(Vec3(100,100,0));
        TestPanel->GenerateMesh();



        Entity* Cam = m_Engine->GetSceneGraph()->CreateEntity();
        CamTrans = new TransformComponent();
        m_Engine->GetSceneGraph()->AddComponent(Cam, CamTrans);
        m_Engine->GetSceneGraph()->AddComponent(Cam, new CameraComponent());
        m_Engine->GetRenderSystem()->SetCamera(Cam);
        CamTrans->SetFirstPerson(true);
        //m_Engine->GetInputSystem()->SetMousePosition(Vec2(400,300));
        //CamTrans->SetRelativeTo(P);
        CamTrans->SetPosition(Vec3(0,0,2));
        ET->SetRelativeTo(Cam);
    }

    void TestState::Update(Scalar dt)
    {
        //TransformComponent* trans = (TransformComponent*)m_Engine->GetSceneGraph()->GetEntityById(0)->GetComponentByType(CT_TRANSFORM);
        Vec3 Pos = Vec3();

        if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_W)) {
            Pos += Vec3(0,0,-30*dt);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_S)) {
            Pos += Vec3(0,0,30*dt);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_A)) {
            Pos += Vec3(-30*dt,0,0);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_D)) {
            Pos += Vec3(30*dt,0,0);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_UP)) {
            m_Engine->SetTimeOfDay(m_Engine->GetTimeOfDay() + ((1800*dt) * 0.00001157407f));
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_DOWN)) {
            m_Engine->SetTimeOfDay(m_Engine->GetTimeOfDay() - ((1800*dt) * 0.00001157407f));
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_Q)) {
            pr->ReloadShader();
            er->ReloadShader();
        }

        Vec2 MousePos = m_Engine->GetInputSystem()->GetMousePosition();
        MousePos.x -= 400;
        MousePos.y -= 300;
        if(MousePos.x + MousePos.y != 0) {
            Vec3 yAxis = Vec3(1,0,0);
            Vec3 xAxis = Vec3(0,1,0);

            //CamTrans->Rotate(xAxis,-MousePos.x*0.5f);
            //CamTrans->Rotate(yAxis,-MousePos.y*0.5f);
        }

        //m_Engine->GetInputSystem()->SetMousePosition(Vec2(400,300));
        CamTrans->Translate(Pos);
    }
}