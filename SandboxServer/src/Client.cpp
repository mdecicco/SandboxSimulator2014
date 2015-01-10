#include <Client.h>
#include <iostream>
#include <assert.h>

namespace SandboxSimulator
{
    Client::Client(i8 clientID, std::string ip, u16 port)
    {
        m_Port = port;
        m_IP = ip;
        m_Id = clientID;
        m_Socket = new UdpSocket();

        sf::Packet* packet = new sf::Packet();
        (*packet) << (i8)PT_CONNECT << m_Id;
        m_Socket->Send(packet, m_IP, m_Port);
        delete packet;
    }

    Client::~Client() 
    {}

    void Client::Disconnect()
    {
    
    }

    void Client::ParsePacket(PACKET_TYPE Type, sf::Packet* Packet)
    {
        switch(Type)
        {
            case PT_UPDATE:
                i8 rec;
                (*Packet) >> rec;
                printf("%d: %d\n", m_Id, rec);
                break;
            case PT_EVENT:

                break;
            default:

                break;
        }
    }

    void Client::Acknowledge(i32 PacketID)
    {
        sf::Packet* packet = new sf::Packet();
        (*packet) << (i8)PT_ACK << PacketID;
        Send(packet);
        delete packet;
    }

    void Client::Send(sf::Packet* Packet)
    {
        m_Socket->Send(Packet, m_IP, m_Port);
    }

    /*--------------------------- Client Manager ---------------------------------*/

    ClientManager::~ClientManager() 
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
        {
            delete m_Clients[i];
        }
        m_Clients.clear();
    }

    Client* ClientManager::NewClient(i8 ClientID, std::string Address, u16 Port)
    {
        if(!HasClient(ClientID) && !HasClient(Address, Port)) {
            Client* c = new Client(ClientID, Address, Port);
            m_Clients.push_back(c);
            return c;
        }

        return NULL;
    }

    bool ClientManager::HasClient(i8 ClientID)
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetID() == i)
                return true;

        return false;
    }

    bool ClientManager::HasClient(std::string Address, u16 Port)
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetAddress() == Address && m_Clients[i]->GetPort() == Port)
                return true;

        return false;
    }

    bool ClientManager::HasClient(i8 ClientID, std::string Address, u16 Port)
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetID() == ClientID && m_Clients[i]->GetAddress() == Address && m_Clients[i]->GetPort() == Port)
                return true;

        return false;
    }

    Client* ClientManager::GetClient(i32 Index)
    {
        return m_Clients[Index];
    }

    Client* ClientManager::GetClient(i8 ClientID)
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetID() == ClientID) return m_Clients[i];

        return NULL;
    }

    Client* ClientManager::GetClient(std::string Address, u16 Port)
    {
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetAddress() == Address && m_Clients[i]->GetPort() == Port)
                return m_Clients[i];

        return NULL;
    }

    bool ClientManager::RemoveClient(i8 ClientID)
    {
        if(HasClient(ClientID))
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetID() == ClientID) {
                m_Clients.erase(m_Clients.begin()+i);
                return true;
            }
        return false;
    }

    bool ClientManager::RemoveClient(i8 ClientID, std::string Address, u16 Port)
    {
        if(HasClient(ClientID, Address, Port))
        for(i32 i = 0; i < m_Clients.size(); i++)
            if(m_Clients[i]->GetID() == ClientID) {
                m_Clients.erase(m_Clients.begin()+i);
                return true;
            }
        return false;
    }
}