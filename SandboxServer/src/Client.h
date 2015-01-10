#ifndef SandboxSimulator_ClientThread_h
#define SandboxSimulotar_ClientThread_h

#include <System/SSThread.h>
#include <Network/Socket.h>

#include <System/SSTypes.h>
#include <vector>

namespace SandboxSimulator {
class Client
{
    public:
        Client(i8 clientID, std::string ip, u16 port);
        ~Client();

        void Disconnect();

        void ParsePacket(PACKET_TYPE Type, sf::Packet* Packet);

        void Acknowledge(i32 PacketID);
        void Send(sf::Packet* Packet);

        //Getters
        i8 GetID() { return m_Id; }
        u16 GetPort() { return m_Port; }
        std::string GetAddress() { return m_IP; }

    private:
        i8 m_Id;
        UdpSocket* m_Socket;
        std::string m_IP;
        u16 m_Port;
};

class ClientManager
{
    public:
        ClientManager() : m_Clients(std::vector<Client*>()) {}
        ~ClientManager();

        Client* NewClient(i8 ClientID, std::string Address, u16 Port);
        bool HasClient(i8 ClientID);
        bool HasClient(std::string Address, u16 Port);
        bool HasClient(i8 ClientID, std::string Address, u16 Port);

        Client* GetClient(i32 Index);
        Client* GetClient(i8 ClientID);
        Client* GetClient(std::string Address, u16 Port);

        bool RemoveClient(i8 ClientID);
        bool RemoveClient(i8 ClientID, std::string Address, u16 Port);

        i32 NumClients() { return m_Clients.size(); }

    private:
        std::vector<Client*> m_Clients;
};
}

#endif