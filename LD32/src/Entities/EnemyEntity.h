#ifndef ENEMY_ENTITY_H
#define ENEMY_ENTITY_H

#include <Engine.h>
#include <Rendering/RenderSystem.h>
#include <Core/TransformComponent.h>
#include <Core/PhysicsSystem.h>
#include <Entities/Player.h>

using namespace SandboxSimulator;
namespace LudumDare {
class Enemy
{
    public:
        Enemy(Vec3 Position, Player* player);
        ~Enemy();

        void Init(SSEngine* Eng);
        void Update(Scalar DeltaTime);

        RenderComponent* GetRender() 
        {
            return (RenderComponent*)m_Entity->GetComponentByType(CT_RENDER);
        }

        PhysicsComponent* GetPhysics()
        {
            return (PhysicsComponent*)m_Entity->GetComponentByType(CT_PHYSICS);
        }

        TransformComponent* GetTransform()
        {
            return (TransformComponent*)m_Entity->GetComponentByType(CT_TRANSFORM);
        }

    protected:
        SSEngine* m_Engine;
        Entity* m_Entity;
        Vec3 m_StartingPosition;
        Player* m_Player;
};
}

#endif