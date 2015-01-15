#ifndef SandboxSimulator_TransformComponent_h
#define SandboxSimulator_TransformComponent_h

#include <Core/SceneGraph.h>
#include <Utils/Math.h>

namespace SandboxSimulator {
class TransformComponent : public Component
{
    public:
        TransformComponent();
        ~TransformComponent();

        //Transformation controls
        void Translate(Vec3 Delta);

        //Getters
        Mat4 GetMat4();
        Vec3 GetPosition();

        virtual void BinarySerialize(sf::Packet* Packet);
        virtual void BinaryDeserialize(sf::Packet* Packet);

    protected:
        Mat4 m_Transform;
        Vec3 m_Position;

        bool m_NeedsUpdate;
};
}

#endif