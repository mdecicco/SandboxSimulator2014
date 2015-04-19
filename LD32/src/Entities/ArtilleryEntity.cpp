#include <Entities/ArtilleryEntity.h>
#include <Entities/EnemyEntity.h>
#include <GameState.h>

using namespace SandboxSimulator;
namespace LudumDare {
    ArtilleryEntity::ArtilleryEntity(Vec3 Position, GameState* State) : m_StartingPosition(Position), m_GameState(State)
    {

    }

    ArtilleryEntity::~ArtilleryEntity()
    {
    }

    void ArtilleryEntity::LoadMattress()
    {
        SceneGraph* Scene = m_Engine->GetSceneGraph();
        
        m_Mattress = Scene->CreateEntity();
        TransformComponent* t = new TransformComponent();
        Scene->AddComponent(m_Mattress,t);
        GetTransform()->SetPosition(m_StartingPosition);
        
        RenderComponent* r = new RenderComponent();
        Scene->AddComponent(m_Mattress,r);
        r->SetPrimType(GL_TRIANGLES);
        r->LoadMesh("Data/Models/DirtyMattress.obj");
        r->SetShader(new Shader());
        r->GetShader()->Load("Data/Shaders/MattressShader.glsl");
        r->GetMesh()->m_Textures[0] = new Texture();
        r->GetMesh()->m_Textures[0]->Load("Data/Textures/DirtyMattress.png");
        
        t->SetRelativeTo(m_Entity);
    }
    void ArtilleryEntity::LoadGun()
    {
        SceneGraph* Scene = m_Engine->GetSceneGraph();
        m_Gun = Scene->CreateEntity();
        
        TransformComponent* t = new TransformComponent();
        Scene->AddComponent(m_Gun,t);
        GetTransform()->SetPosition(m_StartingPosition);
        
        RenderComponent* r = new RenderComponent();
        Scene->AddComponent(m_Gun,r);
        r->SetPrimType(GL_TRIANGLES);
        r->LoadMesh("Data/Models/Gun.obj");
        r->SetShader(new Shader());
        r->GetShader()->Load("Data/Shaders/GunShader.glsl");
        r->GetMesh()->m_Textures[0] = new Texture();
        r->GetMesh()->m_Textures[0]->Load("Data/Textures/MachineGun.png");
        
        t->SetRelativeTo(m_Entity);
    }
    void ArtilleryEntity::LoadHomelessGuy()
    {
        SceneGraph* Scene = m_Engine->GetSceneGraph();
        m_HomelessGuy = Scene->CreateEntity();
        
        TransformComponent* t = new TransformComponent();
        Scene->AddComponent(m_HomelessGuy,t);
        GetTransform()->SetPosition(m_StartingPosition);
        GetTransform()->Scale(0.2f);
        
        RenderComponent* r = new RenderComponent();
        Scene->AddComponent(m_HomelessGuy,r);
        r->SetPrimType(GL_TRIANGLES);
        r->LoadMesh("Data/Models/HomelessGuy.obj");
        r->SetShader(new Shader());
        r->GetShader()->Load("Data/Shaders/HoboShader.glsl");
        r->GetMesh()->m_Textures[0] = new Texture();
        r->GetMesh()->m_Textures[0]->Load("Data/Textures/HomelessGuy.png");
        
        t->SetRelativeTo(m_Entity);
    }
    void ArtilleryEntity::Init(SSEngine* Eng)
    {
        m_Engine = Eng;
        
        //Setup the body
        SceneGraph* Scene = m_Engine->GetSceneGraph();
        m_Entity = Scene->CreateEntity();
        
        Scene->AddComponent(m_Entity, new TransformComponent());
        GetTransform()->SetPosition(m_StartingPosition);
        
        LoadMattress();
        LoadGun();
        LoadHomelessGuy();
        
        m_HoverSpeed = 3.0f;
        m_HoverDistance = 3;
        m_HoverDistanceVariation = 1.7f;
        m_HoverHeight = 2;
        m_HoverHeightVariation = 1.7f;
        m_MoveSpeed = 1500000.0f;
        m_t = 0.0f;
    }

    void ArtilleryEntity::Update(Scalar DeltaTime)
    {
        m_t += DeltaTime;
        
        //Find nearest enemy
        vector<Enemy*> Enemies = m_GameState->GetEnemyVector();
        TransformComponent* Transform = GetTransform();
        Vec3 Position = Transform->GetPosition();
        
        Scalar Dist = FLT_MAX;
        Vec3 EnemyPos;
        for(i32 i = 0;i < Enemies.size();i++)
        {
            TransformComponent* t = Enemies[i]->GetTransform();
            EnemyPos = t->GetPosition();
            Scalar d = (Position - EnemyPos).Magnitude();
            if(d < Dist)
            {
                Dist = d;
                m_NearestEnemy = Enemies[i];
            }
        }
        
        //Move towards it
        Vec3 EnemyToHoboXZ = ((Position - EnemyPos) / Dist) * Vec3(1,0,1);
        Vec3 TargetPos = EnemyPos + (EnemyToHoboXZ * (m_HoverDistance + (sin(m_t * m_HoverSpeed) * m_HoverDistanceVariation)))
                                  + Vec3(0,m_HoverHeight + (cos(m_t * m_HoverSpeed) * m_HoverHeightVariation),0);
        
        Vec3 HoboToTarget = (TargetPos - Position) / Dist;
        Transform->Translate(HoboToTarget);
        
        Vec3 NewPos = Transform->GetPosition();
        //Look at it
        Quat LookAt;
        LookAt.LookAt(NewPos,EnemyPos);
        //Transform->SetOrientation(LookAt);
    }
}