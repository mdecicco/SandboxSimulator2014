#include <Client.h>
#include <iostream>
#include <assert.h>
#include <Engine.h>

namespace SandboxSimulator
{
    Client::Client(u16 clientID, std::string ip, u16 port, UdpSocket* Socket, SSEngine* engine, sf::Mutex* mutex) : m_LastPacketID(0)
    {
        m_Mutex = mutex;
        m_Engine = engine;
        m_LastMessageTime = m_Engine->GetElapsedTime();
        m_Port = port;
        m_IP = ip;
        m_Id = clientID;
        m_Socket = Socket;
        m_PendingPing = false;

        sf::Packet* packet = CreatePacket();
        (*packet) << (i8)PT_CONNECT << m_Id;
        m_Socket->Send(packet, m_IP, m_Port);
        delete packet;
    }

    Client::~Client()
    {}

    void Client::Disconnect(DISCONNECT_REASON Reason)
    {
        if(Reason == DR_TIMEOUT)
            m_Engine->Log("Disconnecting client, reason: timed out\n");
        else if (Reason == DR_QUIT)
            m_Engine->Log("Disconnecting client, reason: quit\n");
        sf::Packet* p = CreatePacket();
        (*p) << (i8) PT_DISCONNECT << (i8) Reason;
        Send(p);
    }

    void Client::ParsePacket(PACKET_TYPE Type, sf::Packet* Packet, u32 PacketID)
    {
        m_Mutex->lock();
        m_LastMessageTime = m_Engine->GetElapsedTime();
        switch(Type)
        {
            case PT_UPDATE:
                i8 rec;
                (*Packet) >> rec;
                m_Engine->Log("Received message from client %d: %d\n", m_Id, rec);
                break;
            case PT_EVENT:

                break;
            case PT_ACK:
                u32 AckPacketID;
                (*Packet) >> AckPacketID;//Remove packet from ack queue
                if(m_PendingPing) m_PendingPing = false;
                //Acknowledge(PacketID); <- Don't need to acknowledge acknowledgements...
                break;
            default:

                break;
        }
        m_Mutex->unlock();
    }

    void Client::Acknowledge(u32 PacketID)
    {
        sf::Packet* packet = CreatePacket();
        (*packet) << (i8)PT_ACK << PacketID;
        Send(packet);
        delete packet;
    }

    void Client::Ping()
    {
        sf::Packet* packet = CreatePacket();
        (*packet) << (i8)PT_PING;
        Send(packet);
        m_PendingPing = true;
        delete packet;
    }

    void Client::Send(sf::Packet* Packet)
    {
        m_Socket->Send(Packet, m_IP, m_Port);
    }

    sf::Packet* Client::CreatePacket()
    {
        sf::Packet* packet = new sf::Packet();
        m_LastPacketID++;
        (*packet) << m_LastPacketID;
        return packet;
    }
}