#ifndef SandboxSimulator_ClientThread_h
#define SandboxSimulator_ClientThread_h

#include <System/SSThread.h>
#include <Network/Socket.h>

#include <System/SSTypes.h>
#include <vector>

namespace SandboxSimulator {
class SSEngine;

class Client
{
    //All functions need to be thread safe
    public:
        Client(u16 clientID, std::string ip, u16 port, UdpSocket* Socket, SSEngine* engine, sf::Mutex* mutex, UID EntityID);
        ~Client();

        void Disconnect(DISCONNECT_REASON Reason);

        void ParsePacket(PACKET_TYPE Type, sf::Packet* Packet, u32 PacketID);

        void Acknowledge(u32 PacketID);
        void Send(sf::Packet* Packet);

        void Ping();

        //Getters
        u16 GetID() { return m_Id; }
        u16 GetPort() { return m_Port; }
        std::string GetAddress() { return m_IP; }
        Scalar GetLastMessageTime() { return m_LastMessageTime; }
        bool HasPendingPing() { return m_PendingPing; }

        //Misc
        sf::Packet* CreatePacket(PACKET_TYPE Type);
        void SendWorldState(SSEngine* Eng, bool IncludeClientEntity);
        void SendPositionUpdate(SSEngine* Eng, bool IncludeClientEntity);

    private:
        u16 m_Id;
        u16 m_Port;
        std::string m_IP;
        SSEngine* m_Engine;
        UdpSocket* m_Socket;
        u32 m_LastPacketID;
        u32 m_LastUpdateSequence;

        Scalar m_LastMessageTime;
        bool m_PendingPing;
        sf::Mutex* m_Mutex;

        UID m_ClientEntityID;
};
}

#endif