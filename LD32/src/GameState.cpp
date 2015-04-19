#include <GameState.h>
#include <Core/TransformComponent.h>
#include <Rendering/RenderSystem.h>

#include <Entities/ArtilleryEntity.h>
#include <Entities/Player.h>
#include <Entities/EnemyEntity.h>

namespace LudumDare {
    GameState::GameState(SoundEffect* BackgroundMusic) : m_BackgroundMusic(BackgroundMusic)
    {

    }

    GameState::~GameState()
    {
        delete m_Player;
        delete m_Explosion;
        delete m_HoboGunnerTest;
    }

    void GameState::Init(StateManagerSystem* Manager)
    {
        m_Explosion = m_Engine->CreateSound("Data/Sound/Explosion.wav");
        //m_BackgroundMusic->play();

        m_Manager = Manager;

        SceneGraph* Scene = m_Engine->GetSceneGraph();

        //Sky mesh
        Entity* Sky = Scene->CreateEntity();
        TransformComponent* SkyTransform = new TransformComponent();
        RenderComponent* SkyRender = new RenderComponent();
        Scene->AddComponent(Sky, SkyRender);
        Scene->AddComponent(Sky, SkyTransform);
        SkyRender->SetShape(RC_SKY_SPHERE);

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
        
        //Gunner test
        m_HoboGunnerTest = new ArtilleryEntity(Vec3(0,0.5,0),this);
        m_HoboGunnerTest->Init(m_Engine);
        
        
        
        m_Player = new Player();
        m_Player->Init(m_Engine);
        SkyTransform->SetRelativeTo(m_Player->GetCamera());

        m_Engine->GetRenderSystem()->SetCamera(m_Player->GetCamera());

        for(i32 i = 0; i < 2; i++) {
            Enemy* Enmy = new Enemy(Vec3(0,0,2), m_Player);
            Enmy->Init(m_Engine);
            m_Enemies.push_back(Enmy);
        }
    }

    void GameState::Update(Scalar DeltaTime)
    {
        if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_Q)) {
            m_Engine->SetTimeOfDay(m_Engine->GetTimeOfDay() + ((3000*DeltaTime) * 0.00001157407f));
            m_Engine->SetGlobalVolume(m_Engine->GetGlobalVolume()-1);
            m_Explosion->Play();
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_E)) {
            m_Engine->SetTimeOfDay(m_Engine->GetTimeOfDay() - ((3000*DeltaTime) * 0.00001157407f));
            m_Engine->SetGlobalVolume(m_Engine->GetGlobalVolume()+1);
            m_Explosion->Play();
        }

        m_Player->Update(DeltaTime);
        m_HoboGunnerTest->Update(DeltaTime);
        
        for(i32 i = 0; i < m_Enemies.size(); i++) {
            m_Enemies[i]->Update(DeltaTime);
        }

        if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_ESCAPE))
            m_Engine->SetShutdown(true);
    }
}