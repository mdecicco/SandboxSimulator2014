#ifndef SandboxSimulator_CameraComponent_h
#define SandboxSimulator_CameraComponent_h

#include <Utils/Math.h>
#include <Core/SceneGraph.h>
#include <Core/TransformComponent.h>

namespace SandboxSimulator {
class CameraComponent : public Component
{
    public:
        CameraComponent();
        ~CameraComponent();

        //Setters

        //Getters
        //Mat4 GetProjection();

        Vec2 m_FieldOfView;
        Scalar m_NearPlane;
        Scalar m_FarPlane;
        Scalar m_Exposure;
        Scalar m_Speed;
        bool m_UpdateProjection;
        
        const Mat4& GetProjection();

        virtual void BinarySerialize(sf::Packet* Packet) {}
        virtual void BinaryDeserialize(sf::Packet* Packet) {}

    private:
        Mat4 m_Transform;
        Mat4 m_Projection;

        bool m_NeedsUpdate;
        TransformComponent* m_TransformComponent;
};
}

#endif