#pragma once

#include <Engine.h>
#include <Rendering/RenderSystem.h>
#include <Core/TransformComponent.h>
#include <Core/PhysicsSystem.h>

#include <Entities/Player.h>

using namespace SandboxSimulator;
namespace LudumDare
{
    class GameState;
    class Enemy;
    class ArtilleryEntity
    {
        public:
            ArtilleryEntity(Vec3 Position,GameState* State);
            ~ArtilleryEntity();
            
            void LoadMattress();
            void LoadGun();
            void LoadHomelessGuy();

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
            Entity* m_Mattress;
            Entity* m_Gun;
            Entity* m_HomelessGuy;
        
            Vec3 m_StartingPosition;
            Player* m_Player;
            GameState* m_GameState;
        
            Enemy* m_NearestEnemy;
        
            Scalar m_t;
            Scalar m_HoverSpeed;
            Scalar m_HoverHeight;
            Scalar m_HoverHeightVariation;
            Scalar m_HoverDistance;
            Scalar m_HoverDistanceVariation;
            Scalar m_MoveSpeed;
    };
}