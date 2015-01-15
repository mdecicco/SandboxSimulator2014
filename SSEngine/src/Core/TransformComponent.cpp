#include <Core/TransformComponent.h>

namespace SandboxSimulator
{
    TransformComponent::TransformComponent() : Component(CT_TRANSFORM), m_Position(Vec3()), m_Transform(Mat4(1.0f)), m_NeedsUpdate(true)
    {}

    TransformComponent::~TransformComponent()
    {}

    Mat4 TransformComponent::GetMat4()
    {
        if(m_NeedsUpdate) {
            m_Transform.ClearToIdentity();
            m_Transform.Translate(m_Position);
        }
        return m_Transform;
    }

    Vec3 TransformComponent::GetPosition()
    {
        return m_Position;
    }

    void TransformComponent::Translate(Vec3 Delta)
    {
        m_Position += Delta;
        m_NeedsUpdate = true;
    }
    void TransformComponent::BinarySerialize(sf::Packet* Packet)
    {
        (*Packet) << (i8)CT_TRANSFORM << m_Position.x << m_Position.y << m_Position.z;
    }

    void TransformComponent::BinaryDeserialize(sf::Packet* Packet)
    {
        i8 x;
        i8 y;
        i8 z;
        (*Packet) >> m_Position.x >> m_Position.y >> m_Position.z;
    }
};