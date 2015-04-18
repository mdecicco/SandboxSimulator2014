#include <HelloWorld.h>
#include <Core/TransformComponent.h>
#include <Rendering/RenderSystem.h>

namespace LudumDare {
    HelloWorld::HelloWorld() {

    }

    HelloWorld::~HelloWorld() {
        delete F;
    }

    void HelloWorld::Init(StateManagerSystem* Manager) {
        _TimeLeft = 360;
        m_Manager = Manager;

        Entity* TimerEnt = m_Engine->GetSceneGraph()->CreateEntity();
        TransformComponent* TimerTrans = new TransformComponent();
        RenderComponent* TimerRend = new RenderComponent();
        m_Engine->GetSceneGraph()->AddComponent(TimerEnt, TimerRend);
        m_Engine->GetSceneGraph()->AddComponent(TimerEnt, TimerTrans);
        TimerRend->SetShape(RC_UI);
        _TimerMesh = TimerRend->GetMesh();
        F = new Font();
        F->Load("Data/Font/Ubuntu-better.dst");
        F->AddToMesh(_TimerMesh, "Hello World", Vec3(0,0,0), 56);

        Entity* Camera = m_Engine->GetSceneGraph()->CreateEntity();
        m_Engine->GetSceneGraph()->AddComponent(Camera, new TransformComponent());
        m_Engine->GetSceneGraph()->AddComponent(Camera, new CameraComponent());
        m_Engine->GetRenderSystem()->SetCamera(Camera);
    }

    void HelloWorld::Update(Scalar DeltaTime) {

    }
}