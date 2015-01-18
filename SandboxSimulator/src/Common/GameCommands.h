#ifndef SandboxSimulator_GameCommands_h
#define SandboxSimulator_GameCommands_h

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Network/Socket.h>

#include <Core/TransformComponent.h>
#include <Rendering/RenderSystem.h>

#include <Common/NetworkDefs.h>

namespace SandboxSimulator {
enum GAME_COMMAND_TYPE
{
    GCOM_CREATE_PLAYER,
    GCOM_PLAYER_POSITION,
    GCOM_COUNT
};

class NetworkCommand
{
    public:
        NetworkCommand(SSEngine* Eng, GAME_COMMAND_TYPE Type) : m_Engine(Eng), m_Type(Type) {}
        virtual ~NetworkCommand() {}
        virtual i32 Execute() = 0;
        virtual i32 Execute(Entity* E) { return Execute(); }
        virtual i32 Execute(Component* C) { return Execute(); }

        virtual void Serialize(sf::Packet* p) = 0;
        virtual void Deserialize(sf::Packet* p) = 0;

    protected:
        SSEngine* m_Engine;
        GAME_COMMAND_TYPE m_Type;
};

class CreatePlayerCommand : public NetworkCommand
{
    public:
        CreatePlayerCommand(SSEngine* Eng) : NetworkCommand(Eng, GCOM_CREATE_PLAYER) {}
        CreatePlayerCommand(SSEngine* Eng, u32 EntityID, Vec3 Position) : NetworkCommand(Eng, GCOM_CREATE_PLAYER), EntityID_(EntityID), Position_(Position) {}
        ~CreatePlayerCommand() {}

        virtual i32 Execute()
        {
            Entity* P = m_Engine->GetSceneGraph()->CreateEntity(EntityID_);
            RenderComponent* pr = new RenderComponent();
            TransformComponent* pt = new TransformComponent();
            m_Engine->GetSceneGraph()->AddComponent(P, pr);
            m_Engine->GetSceneGraph()->AddComponent(P, pt);
            pr->SetShape(RC_SPHERE);
            pt->SetFirstPerson(true);
            pt->SetPosition(Position_);
            return P->GetID();
        }

        virtual void Serialize(sf::Packet* p)
        {
            (*p) << (i8)m_Type << EntityID_ << Position_.x << Position_.y << Position_.z;
        }

        virtual void Deserialize(sf::Packet* p)
        {
            (*p) >> EntityID_ >> Position_.x >> Position_.y >> Position_.z;
        }

        u32 EntityID_;
        Vec3 Position_;
};

class PlayerPositionCommand : public NetworkCommand
{
    public:
        PlayerPositionCommand(SSEngine* Eng) : NetworkCommand(Eng, GCOM_PLAYER_POSITION) {}
        PlayerPositionCommand(SSEngine* Eng, u32 EntityID, Vec3 Position) : NetworkCommand(Eng, GCOM_PLAYER_POSITION), EntityID_(EntityID), Position_(Position) {}
        ~PlayerPositionCommand() {}

        virtual i32 Execute()
        {
            Entity* P = m_Engine->GetSceneGraph()->GetEntityById(EntityID_);
            if(P && P->HasComponentType(CT_TRANSFORM)) {
                TransformComponent* Trans = (TransformComponent*)P->GetComponentByType(CT_TRANSFORM);
                Trans->SetPosition(Position_);
                return 0;
            }
            return 1;
        }

        virtual void Serialize(sf::Packet* p)
        {
            (*p) << (i8)m_Type << EntityID_ << Position_.x << Position_.y << Position_.z;
        }

        virtual void Deserialize(sf::Packet* p)
        {
            (*p) >> EntityID_ >> Position_.x >> Position_.y >> Position_.z;
        }

        u32 EntityID_;
        Vec3 Position_;
};

class SetTimeCommand : public NetworkCommand
{

};

class SetTimeRateCommand : public NetworkCommand
{

};
}

#endif