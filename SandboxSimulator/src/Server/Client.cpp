#include <Server/Client.h>
#include <iostream>
#include <assert.h>
#include <Engine.h>

#include <Core/TransformComponent.h>
#include <Server/ServerSystem.h>

namespace SandboxSimulator
{
    Client::Client(u16 clientID, std::string ip, u16 port, UdpSocket* Socket, SSEngine* engine, sf::Mutex* mutex, UID EntityID, ServerSystem* System) : m_ServerSystem(System), m_LastPacketID(0), m_ClientEntityID(EntityID), m_LastUpdateSequence(0), m_PendingPing(false), m_PendingPingID(0)
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
        (*packet) << m_Id << EntityID;
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
        i8 NumCommands = 0;
        switch(Type)
        {
            case PT_ACK:
                u32 AckPacketID;
                (*Packet) >> AckPacketID;
                //Remove packet from ack queue
                if(m_PendingPing && m_PendingPingID == AckPacketID)
                    m_PendingPing = false;
                break;
            case PT_COMMAND:
                //Parse and execute command
                (*Packet) >> NumCommands;
                for(i32 i = 0; i < NumCommands; i++) {
                    i8 CommandType = 0;
                    (*Packet) >> CommandType;
                    if(CommandType == GCOM_PLAYER_POSITION) {
                        if(PacketID > m_LastUpdateSequence || m_LastUpdateSequence == 0) {
                            PlayerPositionCommand* cmd = new PlayerPositionCommand(m_Engine);
                            cmd->Deserialize(Packet);
                            cmd->Execute();
                            delete cmd;
                        }
                    } else if (CommandType == GCOM_SET_TIME_RATE) {
                        SetTimeRateCommand* Cmd = new SetTimeRateCommand(m_Engine);
                        Cmd->Deserialize(Packet);
                        Cmd->Execute();
                        Broadcast(Cmd);
                        delete Cmd;
                    }
                }
                if(PacketID > m_LastUpdateSequence || m_LastUpdateSequence == 0)
                    m_LastUpdateSequence = PacketID;
                break;
            case PT_ESSENTIAL_COMMAND:
                Acknowledge(PacketID);
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
        sf::Packet* packet = CreatePacket(PT_COMMAND);
        std::vector<CreatePlayerCommand> Commands;

        EntityList* Entities = m_Engine->GetSceneGraph()->GetEntities();
        for(EntityList::iterator it = Entities->begin(); it != Entities->end(); it++) {
            Entity* E = it->second.get();
            if(E->GetID() != 998) {
                if(E->HasComponentType(CT_TRANSFORM)) {
                    TransformComponent* Trans = (TransformComponent*)E->GetComponentByType(CT_TRANSFORM);
                    Vec3 Pos = Trans->GetPosition();
                    CreatePlayerCommand cmd = CreatePlayerCommand(m_Engine, E->GetID(), Pos);
                    Commands.push_back(cmd);
                }
            }
        }

        (*packet) << (u8)(Commands.size()+2);
        SetTimeCommand* TimeCmd = new SetTimeCommand(m_Engine, m_Engine->GetTimeOfDay());
        SetTimeRateCommand* TimeRateCmd = new SetTimeRateCommand(m_Engine, m_Engine->GetGameTimeRate());
        TimeCmd->Serialize(packet);
        TimeRateCmd->Serialize(packet);
        for(i32 i = 0; i < Commands.size(); i++)
            Commands[i].Serialize(packet);

        Send(packet);
        delete packet;
        delete TimeCmd;
        delete TimeRateCmd;
    }

    void Client::SendPositionUpdate(SSEngine* Eng, bool InclClient)
    {
        sf::Packet* packet = CreatePacket(PT_COMMAND);
        std::vector<PlayerPositionCommand> Commands;

        EntityList* Entities = m_Engine->GetSceneGraph()->GetEntities();
        for(EntityList::iterator it = Entities->begin(); it != Entities->end(); it++) {
            Entity* E = it->second.get();
            if(E->HasComponentType(CT_TRANSFORM) && E->GetID() != m_ClientEntityID) {
                TransformComponent* Trans = (TransformComponent*)E->GetComponentByType(CT_TRANSFORM);
                Vec3 Pos = Trans->GetPosition();
                PlayerPositionCommand cmd = PlayerPositionCommand(m_Engine, E->GetID(), Pos);
                Commands.push_back(cmd);
            }
        }

        (*packet) << (u8)(Commands.size()+2);
        SetTimeCommand* TimeCmd = new SetTimeCommand(m_Engine, m_Engine->GetTimeOfDay());
        SetTimeRateCommand* TimeRateCmd = new SetTimeRateCommand(m_Engine, m_Engine->GetGameTimeRate());
        TimeCmd->Serialize(packet);
        TimeRateCmd->Serialize(packet);
        for(i32 i = 0; i < Commands.size(); i++)
            Commands[i].Serialize(packet);

        Send(packet);
        delete packet;
    }

    void Client::SendCommand(NetworkCommand* Cmd)
    {
        sf::Packet* packet = CreatePacket(PT_COMMAND);
        (*packet) << (u8)1;
        Cmd->Serialize(packet);
        Send(packet);
        delete packet;
    }

    void Client::Broadcast(NetworkCommand* Cmd)
    {
        m_ServerSystem->Broadcast(Cmd);
    }
}