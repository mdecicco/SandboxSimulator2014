#include <Rendering/CameraComponent.h>
#include <Engine.h>

namespace SandboxSimulator
{
    CameraComponent::CameraComponent() : Component(CT_CAMERA), m_FieldOfView(70.0f,70.0f), m_NearPlane(0.1f),
                                         m_FarPlane(1000.0f), m_Speed(0.0f), m_UpdateProjection(true), m_Exposure(1.0f)
    {
    }
    CameraComponent::~CameraComponent()
    {
    }

    const Mat4& CameraComponent::GetProjection()
    {
        if(m_UpdateProjection)
        {
            //m_Projection = PerspectiveMultiFov(m_FieldOfView.x,m_FieldOfView.y,m_NearPlane,m_FarPlane);
            m_Projection = PerspectiveProjection(m_FieldOfView.x, 800,600, m_NearPlane, m_FarPlane);
            m_UpdateProjection = false;
        }
        return m_Projection;
    }
}