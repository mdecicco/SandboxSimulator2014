#include <GameState.h>
#include <Engine.h>

#include <Core/TransformComponent.h>

namespace SandboxSimulator
{
    GameState::GameState(ConnectionSystem* Conn) : m_Connection(Conn), m_LastPacketTime(0)
    {}

    GameState::~GameState()
    {}

    void GameState::Init(StateManagerSystem *StateManager)
    {
        m_StateManager = StateManager;
        m_Connection->Connect("127.0.0.1", 3889);
    }

    void GameState::Update(Scalar dt)
    {
        //m_Engine->Log("Hello world?\n");
        if(m_Connection->IsConnected() && m_Engine->GetSceneGraph()->HasEntity(m_Connection->GetEntityID())) {
            Scalar pDelta = m_Engine->GetElapsedTime() - m_LastPacketTime;
            TransformComponent* trans = (TransformComponent*)m_Engine->GetSceneGraph()->GetEntityById(m_Connection->GetEntityID())->GetComponentByType(CT_TRANSFORM);
            Vec3 Pos = trans->GetPosition();

            if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_W)) {
                Pos += Vec3(0,0,-1*dt);
                m_Connection->NeedsUpdate(true);
            } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_S)) {
                Pos += Vec3(0,0,1*dt);
                m_Connection->NeedsUpdate(true);
            } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_A)) {
                Pos += Vec3(-1*dt,0,0);
                m_Connection->NeedsUpdate(true);
            } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_D)) {
                Pos += Vec3(1*dt,0,0);
                m_Connection->NeedsUpdate(true);
            }

            trans->SetPosition(Pos);
            if(pDelta > 0.05 && m_Connection->NeedsUpdate()) {
                sf::Packet* Packet = m_Connection->CreatePacket(PT_PLAYER_UPDATE);
                (*Packet) << Pos.x << Pos.y << Pos.z;
                m_Connection->Send(Packet);
                m_LastPacketTime = m_Engine->GetElapsedTime();
                m_Connection->NeedsUpdate(false);
            }
        }
    }
}