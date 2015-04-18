#include <HelloWorld.h>
#include <Core/TransformComponent.h>
#include <Rendering/RenderSystem.h>

namespace LudumDare {
    HelloWorld::HelloWorld() 
    {

    }

    HelloWorld::~HelloWorld() 
    {
        delete m_Player;
    }

    void HelloWorld::Init(StateManagerSystem* Manager) 
    {
        m_Manager = Manager;

        SceneGraph* Scene = m_Engine->GetSceneGraph();

        //Sky mesh
        Entity* Sky = Scene->CreateEntity();
        TransformComponent* SkyTransform = new TransformComponent();
        RenderComponent* SkyRender = new RenderComponent();
        Scene->AddComponent(Sky, SkyRender);
        Scene->AddComponent(Sky, SkyTransform);
        SkyRender->SetShape(RC_SKY_SPHERE);

        Entity* Test = Scene->CreateEntity();
        TransformComponent* TestTransform = new TransformComponent();
        RenderComponent* TestRender = new RenderComponent();
        Scene->AddComponent(Test, TestRender);
        Scene->AddComponent(Test, TestTransform);
        TestTransform->SetPosition(Vec3(0,0.5,-1));
        TestRender->SetShape(RC_POINT_SPRITE);

        Entity* Test2 = Scene->CreateEntity();
        TransformComponent* Test2Transform = new TransformComponent();
        RenderComponent* Test2Render = new RenderComponent();
        Scene->AddComponent(Test2, Test2Render);
        Scene->AddComponent(Test2, Test2Transform);
        Test2Transform->SetPosition(Vec3(0,0.5,-1));
        Test2Render->SetShape(RC_SQUARE);

        //Ground mesh
        Entity* Ground = Scene->CreateEntity();
        TransformComponent* GroundTransform = new TransformComponent();
        RenderComponent* GroundRender = new RenderComponent();
        Scene->AddComponent(Ground, GroundRender);
        Scene->AddComponent(Ground, GroundTransform);
        GroundRender->SetShape(RC_GROUND_PLANE);
        GroundTransform->SetPosition(Vec3(0, -2, 0));
        GroundTransform->SetStatic(true);
        m_Engine->GetPhysicsSystem()->MakePhysical(Ground, 0.0, false, 0.5, 1.0);

        m_Player = new Player();
        m_Player->Init(m_Engine);
        SkyTransform->SetRelativeTo(m_Player->GetCamera());

        m_Engine->GetRenderSystem()->SetCamera(m_Player->GetCamera());
    }

    void HelloWorld::Update(Scalar DeltaTime) 
    {
        m_Player->Update(DeltaTime);

        if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_ESCAPE))
            m_Engine->SetShutdown(true);
    }
}