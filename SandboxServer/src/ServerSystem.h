#ifndef SandboxSimulator_ServerSystem_h
#define SandboxSimulator_ServerSystem_h

#include <Core/EngineSystem.h>
#include <Client.h>

#define TIMEOUT_LIMIT 10

namespace SandboxSimulator
{
class ServerSystem : public EngineSystem
{
    public:
        ServerSystem(sf::Mutex* mutex);
        ~ServerSystem();

        virtual void HandleMessage(const EngineMessage* Msg);

		virtual void Initialize(SSEngine* Eng);
		virtual void Update(Scalar dt);
		virtual void Shutdown();

		virtual void Serialize();
	    virtual void Deserialize();

        // Needs to be thread safe
        Client* NewClient(u16 ClientID, std::string Address, u16 Port, UdpSocket* Socket);
        bool HasClient(u16 ClientID);
        bool HasClient(std::string Address, u16 Port);
        bool HasClient(u16 ClientID, std::string Address, u16 Port);

        Client* GetClient(u16 ClientID);
        Client* GetClient(std::string Address, u16 Port);

        bool RemoveClient(Client* client);
        bool RemoveClient(u16 ClientID);
        bool RemoveClient(u16 ClientID, std::string Address, u16 Port);

        i32 NumClients() { return m_Clients.size(); }
        //

    private:
        SSEngine* m_Engine;
        sf::Mutex* m_Mutex;
        std::vector<Client*> m_Clients;
};
}

#endif