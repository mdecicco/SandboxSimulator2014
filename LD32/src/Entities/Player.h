#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Core/PhysicsSystem.h>
#include <Rendering/RenderSystem.h>

using namespace SandboxSimulator;
namespace LudumDare {
class Player
{
    public:
        Player();
        ~Player();

        void Init(SSEngine* Engine);
        void Update(Scalar DeltaTime);

        RenderComponent* GetRender() 
        {
            return (RenderComponent*)m_Body->GetComponentByType(CT_RENDER);
        }

        PhysicsComponent* GetPhysics()
        {
            return (PhysicsComponent*)m_Body->GetComponentByType(CT_PHYSICS);
        }

        TransformComponent* GetTransform()
        {
            return (TransformComponent*)m_Body->GetComponentByType(CT_TRANSFORM);
        }

        CameraComponent* GetCameraComponent()
        {
            return (CameraComponent*)m_Camera->GetComponentByType(CT_CAMERA);
        }

        TransformComponent* GetCameraTransform()
        {
            return (TransformComponent*)m_Camera->GetComponentByType(CT_TRANSFORM);
        }

        Entity* GetCamera()
        {
            return m_Camera;
        }

        Entity* GetBody() 
        {
            return m_Body;
        }

    protected:
        SSEngine* m_Engine;
        Entity* m_Body;
        Entity* m_Camera;
};
}

#endif