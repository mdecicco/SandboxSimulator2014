#include <Server/ServerSystem.h>
#include <Rendering/RenderSystem.h>
#include <Engine.h>

#include <Network/Serialization.h>
#include <Common/GameCommands.h>

namespace SandboxSimulator
{
    ServerSystem::ServerSystem(sf::Mutex* mutex) : m_Clients(std::vector<Client*>()), m_Mutex(mutex), m_TimeSinceLastUpdate(0)
    {}

    ServerSystem::~ServerSystem()
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
        {
            delete m_Clients[i];
        }
        m_Clients.clear();
    }

    void ServerSystem::HandleMessage(const EngineMessage* Msg)
    {

    }

	void ServerSystem::Initialize(SSEngine* Eng)
    {
        m_Engine = Eng;
    }

	void ServerSystem::Update(Scalar dt)
    {
        m_Mutex->lock();
        m_TimeSinceLastUpdate += dt;
        if(m_Engine) {
            Scalar ElapsedTime = m_Engine->GetElapsedTime();
            //printf("%f: Updating server system\n", ElapsedTime);
            for(i32 i = 0; i < m_Clients.size(); i++)
            {
                if(ElapsedTime - m_Clients[i]->GetLastMessageTime() > TIMEOUT_LIMIT) {
                    Client* c = m_Clients[i];
                    if(RemoveClient(m_Clients[i]))
                        c->Disconnect(DR_TIMEOUT);
                } else if(ElapsedTime - m_Clients[i]->GetLastMessageTime() > TIMEOUT_LIMIT / 2 && !m_Clients[i]->HasPendingPing())
                    m_Clients[i]->Ping();
            }

            if(m_TimeSinceLastUpdate > 0.05) {
                for(i32 i = 0; i < m_Clients.size(); i++) {
                    m_Clients[i]->SendPositionUpdate(m_Engine, false);
                }
                //m_Engine->Log("Updating clients states\n");
                m_TimeSinceLastUpdate = 0.0;
            }
        }
        m_Mutex->unlock();
    }

	void ServerSystem::Shutdown()
    {

    }

	void ServerSystem::Serialize()
    {

    }

	void ServerSystem::Deserialize()
    {

    }

    Client* ServerSystem::NewClient(u16 ClientID, std::string Address, u16 Port, UdpSocket* Socket)
    {
        m_Mutex->lock();
        if(!HasClient(ClientID) && !HasClient(Address, Port)) {
            CreatePlayerCommand* IDCmd = new CreatePlayerCommand(m_Engine, (u32)-1, Vec3(0,0,-2));
            u32 EntityID = IDCmd->Execute();
            delete IDCmd;

            Client* c = new Client(ClientID, Address, Port, Socket, m_Engine, m_Mutex, EntityID, this);

            //Send serialized world state
            c->SendWorldState(m_Engine);
            //
            //Resend world state to other clients, so they know about this.
            CreatePlayerCommand* CPCmd = new CreatePlayerCommand(m_Engine, EntityID, Vec3(0,0,-2));
            for(i32 i = 0; i < m_Clients.size(); i++) {
                m_Clients[i]->SendCommand(CPCmd);
            }
            delete CPCmd;

            m_Clients.push_back(c);

            m_Mutex->unlock();
            return c;
        }
        m_Mutex->unlock();
        return NULL;
    }

    bool ServerSystem::HasClient(u16 ClientID)
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetID() == i)
                return true;

        return false;
    }

    bool ServerSystem::HasClient(std::string Address, u16 Port)
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetAddress() == Address && m_Clients[i]->GetPort() == Port)
                return true;

        return false;
    }

    bool ServerSystem::HasClient(u16 ClientID, std::string Address, u16 Port)
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetID() == ClientID && m_Clients[i]->GetAddress() == Address && m_Clients[i]->GetPort() == Port)
                return true;

        return false;
    }

    Client* ServerSystem::GetClient(u16 ClientID)
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetID() == ClientID) return m_Clients[i];

        return NULL;
    }

    Client* ServerSystem::GetClient(std::string Address, u16 Port)
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetAddress() == Address && m_Clients[i]->GetPort() == Port)
                return m_Clients[i];

        return NULL;
    }

    bool ServerSystem::RemoveClient(Client* client)
    {
        m_Mutex->lock();
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i] == client) {
                m_Clients.erase(m_Clients.begin()+i);
                m_Mutex->unlock();
                return true;
            }
        m_Mutex->unlock();
        return false;
    }

    bool ServerSystem::RemoveClient(u16 ClientID)
    {
        m_Mutex->lock();
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetID() == ClientID) {
                m_Clients.erase(m_Clients.begin()+i);
                m_Mutex->unlock();
                return true;
            }
        m_Mutex->unlock();
        return false;
    }

    bool ServerSystem::RemoveClient(u16 ClientID, std::string Address, u16 Port)
    {
        m_Mutex->lock();
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetID() == ClientID && m_Clients[i]->GetAddress() == Address && m_Clients[i]->GetPort() == Port) {
                m_Clients.erase(m_Clients.begin()+i);
                m_Mutex->unlock();
                return true;
            }
        m_Mutex->unlock();
        return false;
    }

    void ServerSystem::Broadcast(NetworkCommand* Cmd)
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
        {
            Client* c = m_Clients[i];
            sf::Packet* p = c->CreatePacket(PT_COMMAND);
            (*p) << (u8)1;
            Cmd->Serialize(p);
            c->Send(p);
        }
    }
};