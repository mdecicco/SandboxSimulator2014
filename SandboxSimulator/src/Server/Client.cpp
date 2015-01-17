#include <Server/Client.h>
#include <iostream>
#include <assert.h>
#include <Engine.h>

#include <Core/TransformComponent.h>

namespace SandboxSimulator
{
    Client::Client(u16 clientID, std::string ip, u16 port, UdpSocket* Socket, SSEngine* engine, sf::Mutex* mutex, UID EntityID) : m_LastPacketID(0), m_ClientEntityID(EntityID), m_LastUpdateSequence(0), m_PendingPing(false), m_PendingPingID(0)
    {
        m_Mutex = mutex;
        m_Engine = engine;
        m_LastMessageTime = m_Engine->GetElapsedTime();
        m_Port = port;
        m_IP = ip;
        m_Id = clientID;
        m_Socket = Socket;
        m_PendingPing = false;

        sf::Packet* packet = CreatePacket(PT_CONNECT);
        (*packet) << m_Id;
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
        sf::Packet* p = CreatePacket(PT_DISCONNECT);
        (*p) << (i8) Reason;
        Send(p);
    }

    void Client::ParsePacket(PACKET_TYPE Type, sf::Packet* Packet, u32 PacketID)
    {
        m_Mutex->lock();
        m_LastMessageTime = m_Engine->GetElapsedTime();
        switch(Type)
        {
            case PT_EVENT:

                break;
            case PT_ACK:
                u32 AckPacketID;
                (*Packet) >> AckPacketID;
                //Remove packet from ack queue
                if(m_PendingPing && m_PendingPingID == AckPacketID)
                    m_PendingPing = false;
                break;
            case PT_PLAYER_UPDATE:
                if(PacketID > m_LastUpdateSequence || m_LastUpdateSequence == 0) {
                    Vec3 Pos;
                    (*Packet) >> Pos.x >> Pos.y >> Pos.z;
                    TransformComponent* Trans = (TransformComponent*)m_Engine->GetSceneGraph()->GetEntityById(m_ClientEntityID)->GetComponentByType(CT_TRANSFORM);
                    Trans->SetPosition(Pos);
                    m_LastUpdateSequence = PacketID;
                    //Acknowledge(PacketID);
                }
                break;
            case PT_PING:
                Acknowledge(PacketID);
                break;
        }
        m_Mutex->unlock();
    }

    void Client::Acknowledge(u32 PacketID)
    {
        sf::Packet* packet = CreatePacket(PT_ACK);
        (*packet) << PacketID;
        Send(packet);
        delete packet;
    }

    void Client::Ping()
    {
        sf::Packet* packet = CreatePacket(PT_PING);
        m_PendingPingID = m_LastPacketID;
        Send(packet);
        m_PendingPing = true;
        delete packet;
    }

    void Client::Send(sf::Packet* Packet)
    {
        m_Socket->Send(Packet, m_IP, m_Port);
    }

    sf::Packet* Client::CreatePacket(PACKET_TYPE Type)
    {
        sf::Packet* packet = new sf::Packet();
        m_LastPacketID++;
        (*packet) << m_LastPacketID << (i8) Type;
        return packet;
    }

    void Client::SendWorldState(SSEngine* Eng)
    {
        sf::Packet* packet = CreatePacket(PT_STATE_UPDATE);
        (*packet) << m_ClientEntityID;
        Eng->GetSceneGraph()->BinarySerialize(packet);
        Send(packet);
    }

    void Client::SendPositionUpdate(SSEngine* Eng, bool InclClient)
    {
        sf::Packet* packet = CreatePacket(PT_POS_UPDATE);
        (*packet) << m_ClientEntityID;
        if(InclClient) {
            Eng->GetSceneGraph()->BinarySerializePositions(packet);
        } else
            Eng->GetSceneGraph()->BinarySerializePositions(packet, m_ClientEntityID);
        Send(packet);
    }
}