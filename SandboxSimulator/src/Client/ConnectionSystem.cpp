#include <Client/ConnectionSystem.h>
#include <Engine.h>

namespace SandboxSimulator
{
    ConnectionSystem::ConnectionSystem() :  m_LastPacketID(0), m_ClientID(0), m_LastMessageTime(0), m_Connected(false), m_ConnectionAttempted(false), m_LastStateUpdateSequence(0), m_NeedsUpdate(true), m_PendingPing(false), m_PendingPingID(0)
    {

    }

    ConnectionSystem::~ConnectionSystem()
    {
        delete m_Socket;
    }

    void ConnectionSystem::HandleMessage(const EngineMessage* Msg)
    {

    }

	void ConnectionSystem::Initialize(SSEngine* Eng)
    {
        m_Engine = Eng;
        m_Socket = new UdpSocket();
        m_Socket->Bind(sf::Socket::AnyPort);
        m_Socket->SetBlocking(false);
    }

    void ConnectionSystem::Connect(std::string ServerAddress, u16 ServerPort)
    {
        if(!m_ConnectionAttempted && !m_Connected) {
            m_ServerAddress = ServerAddress;
            m_ServerPort = ServerPort;
            m_LastMessageTime = m_Engine->GetElapsedTime();
            sf::Packet* packet = CreatePacket(PT_CONNECT);

            //have to send it without Send() that requires us to be connected already.
            m_Socket->Send(packet, m_ServerAddress, m_ServerPort);

            delete packet;
            m_ConnectionAttempted = true;
        } else {
            m_Engine->Log("Cannot connect to server, a connection is already in progress.\n");
        }
    }

    void ConnectionSystem::Disconnect()
    {
        if(m_Connected || m_ConnectionAttempted) {
            sf::Packet* Quit = CreatePacket(PT_DISCONNECT);
            (*Quit) << (i8) DR_QUIT;
            m_Socket->Send(Quit, m_ServerAddress, m_ServerPort);
            delete Quit;
            m_Connected = false;
            m_ConnectionAttempted = false;
            m_Engine->Log("Disconnected from server at %s:%d.\n", m_ServerAddress.c_str(), m_ServerPort);
        } else {
            m_Engine->Log("Could not disconnect: no connection established.\n");
        }
    }

	void ConnectionSystem::Update(Scalar dt)
    {
        sf::Packet* packet;
        sf::IpAddress sender;
        u16 port;

        //Handle timing out
        Scalar DeltaMessageTime = m_Engine->GetElapsedTime() - m_LastMessageTime;
        if(DeltaMessageTime >= (TIMEOUT_LIMIT * 0.5) && m_Connected && !m_PendingPing) {
            Ping();
        }
        if(DeltaMessageTime >= TIMEOUT_LIMIT && m_Connected) {
            Disconnect();
            m_Engine->Log("Connection lost: timed out.\n");
        } else if (DeltaMessageTime >= TIMEOUT_LIMIT && m_ConnectionAttempted) {
            Disconnect();
            m_Engine->Log("Connection could not be established: timed out.\n");
        }

        if(m_Connected || m_ConnectionAttempted) {
            bool KeepChecking = true;
            while(KeepChecking) {
                packet = m_Socket->Receive(sender, port);
                if(packet && port == m_ServerPort) {
                    i8 PacketType;
                    u32 PacketID;
                    (*packet) >> PacketID >> PacketType;
                    m_LastMessageTime = m_Engine->GetElapsedTime();
                    i8 NumCommands = 0;

                    switch(PacketType)
                    {
                        case PT_CONNECT:
                            if(!m_Connected && m_ConnectionAttempted) {
                                (*packet) >> m_ClientID >> m_EntityID;
                                m_Connected = true;
                                m_Engine->Log("Connected to server at %s:%d.\n", sender.toString().c_str(), port);
                            } else {
                                m_Engine->Log("Could not connect to server, already connected to a server or a connection was not requested!");
                            }
                            break;
                        case PT_ACK:
                            //a packet was acknowledged, get the ack packet ID and remove it from pending ack queue (TODO)
                            u32 AckPacketID;
                            (*packet) >> AckPacketID;
                            //m_Engine->Log("Packet %d acknowledged by server!\n", AckPacketID);
                            if(m_PendingPing && AckPacketID == m_PendingPingID)
                                m_PendingPing = false;
                            break;
                        case PT_PING:
                            Acknowledge(PacketID);
                            m_PendingPing = false;
                            break;
                        case PT_DISCONNECT:
                            i8 Reason;
                            (*packet) >> Reason;
                            if(Reason == DR_TIMEOUT)
                                m_Engine->Log("Disconnected, reason: timed out.\n");
                            else if (Reason == DR_QUIT)
                                m_Engine->Log("Disconnected, reason: quit.\n");

                            m_Connected = false;
                            m_ConnectionAttempted = false;
                            break;
                        case PT_COMMAND:
                            //Parse and execute command
                            (*packet) >> NumCommands;
                            for(i32 i = 0; i < NumCommands; i++) {
                                i8 CommandType = 0;
                                (*packet) >> CommandType;
                                if(CommandType == GCOM_CREATE_PLAYER) {
                                    CreatePlayerCommand* cmd = new CreatePlayerCommand(m_Engine);
                                    cmd->Deserialize(packet);
                                    cmd->Execute();
                                    delete cmd;
                                } else if (CommandType == GCOM_PLAYER_POSITION) {
                                    if(PacketID > m_LastStateUpdateSequence || m_LastStateUpdateSequence == 0) {
                                        PlayerPositionCommand* cmd = new PlayerPositionCommand(m_Engine);
                                        cmd->Deserialize(packet);
                                        cmd->Execute();
                                        delete cmd;
                                    }
                                } else if (CommandType == GCOM_SET_TIME) {
                                    if(PacketID > m_LastStateUpdateSequence || m_LastStateUpdateSequence == 0) {
                                        SetTimeCommand* cmd = new SetTimeCommand(m_Engine);
                                        cmd->Deserialize(packet);
                                        cmd->Execute();
                                        delete cmd;
                                    }
                                } else if (CommandType == GCOM_SET_TIME_RATE) {
                                    if(PacketID > m_LastStateUpdateSequence || m_LastStateUpdateSequence == 0) {
                                        SetTimeRateCommand* cmd = new SetTimeRateCommand(m_Engine);
                                        cmd->Deserialize(packet);
                                        cmd->Execute();
                                        delete cmd;
                                    }
                                }
                            }
                            if(PacketID > m_LastStateUpdateSequence || m_LastStateUpdateSequence == 0)
                                m_LastStateUpdateSequence = PacketID;
                            break;
                        default:
                            //m_Engine->Log("Packet type %d not registered in the internal enum.\n", (i32)PacketType);
                            break;
                    }

                } else {
                    KeepChecking = false;
                }
                delete packet;
            }
        }
    }

	void ConnectionSystem::Shutdown()
    {
        Disconnect();
    }

    void ConnectionSystem::Acknowledge(i32 PacketID)
    {
        sf::Packet* Ack = CreatePacket(PT_ACK);
        (*Ack) << PacketID;
        Send(Ack);
        delete Ack;
    }

    void ConnectionSystem::Ping()
    {
        sf::Packet* Ping = CreatePacket(PT_PING);
        m_PendingPingID = m_LastPacketID;
        Send(Ping);
        m_PendingPing = true;
        delete Ping;
    }

    bool ConnectionSystem::Send(sf::Packet* Packet)
    {
        if(m_Connected) {
            m_Socket->Send(Packet, m_ServerAddress, m_ServerPort);
            return true;
        }
        m_Engine->Log("Could not send packet, not currently connected to a server!\n");
        return false;
    }

    sf::Packet* ConnectionSystem::CreatePacket(PACKET_TYPE type)
    {
        sf::Packet* packet = new sf::Packet();
        m_LastPacketID++;
        (*packet) << m_LastPacketID << (i8) type << m_ClientID;
        return packet;
    }

    void ConnectionSystem::SendPositionUpdate(Vec3 pos)
    {
        if(m_NeedsUpdate) {
            PlayerPositionCommand* cmd = new PlayerPositionCommand(m_Engine, m_EntityID, pos);
            sf::Packet* p = CreatePacket(PT_COMMAND);
            (*p) << (u8) 1;
            cmd->Serialize(p);
            Send(p);
            m_NeedsUpdate = false;
        }
    }

    void ConnectionSystem::SendCommand(NetworkCommand* Cmd)
    {
        sf::Packet* Packet = CreatePacket(PT_COMMAND);
        (*Packet) << (u8)1;
        Cmd->Serialize(Packet);
        Send(Packet);
    }

	void ConnectionSystem::Serialize()
    {}
	void ConnectionSystem::Deserialize()
    {}
}