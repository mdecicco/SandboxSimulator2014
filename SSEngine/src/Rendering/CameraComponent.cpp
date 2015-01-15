#include <Rendering/CameraComponent.h>
#include <Engine.h>

namespace SandboxSimulator
{
    CameraComponent::CameraComponent() : Component(CT_CAMERA), m_Transform(Mat4(1.0f)), m_Projection(Mat4(1.0f)), m_NeedsUpdate(true)
    {
        m_Projection = PerspectiveProjection(90, 800, 600, 0.1f, 1000.0f);
    }

    CameraComponent::~CameraComponent()
    {

    }

    Mat4 CameraComponent::GetTransform()
    {
        if(m_NeedsUpdate) {
            m_TransformComponent = (TransformComponent*)m_Parent->GetComponentByType(CT_TRANSFORM);
            m_NeedsUpdate = false;
        }

        if(m_TransformComponent)
            return m_TransformComponent->GetMat4();

        return m_Transform;
    }

    Mat4 CameraComponent::GetProjection()
    {
        //if(m_NeedsUpdate)
        return m_Projection;
    }
}