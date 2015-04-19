#include <Entities/EnemyEntity.h>

using namespace SandboxSimulator;
namespace LudumDare {
    Enemy::Enemy(Vec3 Position, Player* player) : m_StartingPosition(Position), m_Player(player)
    {

    }

    Enemy::~Enemy() 
    {

    }

    void Enemy::Init(SSEngine* Eng)
    {
        m_Engine = Eng;
        //Setup the body
        SceneGraph* Scene = m_Engine->GetSceneGraph();
        m_Entity = Scene->CreateEntity();
        Scene->AddComponent(m_Entity, new RenderComponent());
        Scene->AddComponent(m_Entity, new TransformComponent());
        GetRender()->SetShape(RC_POINT_SPRITE);
        GetTransform()->SetPosition(m_StartingPosition);
        m_Engine->GetPhysicsSystem()->MakePhysical(m_Entity, 1.0, true, 0.0, 1.0);
        PhysicsComponent* Col = GetPhysics();
        Col->SetSleepWhenActive(false);
        Col->SetAngularFactor(Vec3(0,0,0));
    }

    void Enemy::Update(Scalar DeltaTime)
    {
        Vec3 Direction = m_Player->GetTransform()->GetPosition() - GetTransform()->GetPosition();
        Direction.Normalize();
        Direction = Direction * 2;
        Vec3 vel = GetPhysics()->m_Body->getLinearVelocity();
        Direction.y = vel.y;
        GetTransform()->SetVelocity(Direction, true);
    }
}