#include <ServerSystem.h>
#include <Rendering/RenderSystem.h>
#include <Engine.h>

#include <Network/Serialization.h>

namespace SandboxSimulator
{
    ServerSystem::ServerSystem(sf::Mutex* mutex) : m_Clients(std::vector<Client*>()), m_Mutex(mutex)
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

    //Needs to be thread safe from here down
    Client* ServerSystem::NewClient(u16 ClientID, std::string Address, u16 Port, UdpSocket* Socket)
    {
        m_Mutex->lock();
        if(!HasClient(ClientID) && !HasClient(Address, Port)) {
            Entity* E = m_Engine->GetSceneGraph()->CreateEntity();
            m_Engine->GetSceneGraph()->AddComponent(E, new RenderComponent());
            RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
            r->SetShape(RC_SQUARE);

            Client* c = new Client(ClientID, Address, Port, Socket, m_Engine, m_Mutex, E->GetID());
            m_Clients.push_back(c);

            //Send serialized world state
            c->SendWorldState(m_Engine);
            //

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
};