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
        Client(u16 clientID, std::string ip, u16 port, UdpSocket* Socket, SSEngine* engine, sf::Mutex* mutex);
        ~Client();

        void Disconnect(DISCONNECT_REASONS Reason);

        void ParsePacket(PACKET_TYPE Type, sf::Packet* Packet);

        void Acknowledge(i32 PacketID);
        void Send(sf::Packet* Packet);

        void Ping();

        //Getters
        u16 GetID() { return m_Id; }
        u16 GetPort() { return m_Port; }
        std::string GetAddress() { return m_IP; }
        Scalar GetLastMessageTime() { return m_LastMessageTime; }
        bool HasPendingPing() { return m_PendingPing; }

    private:
        u16 m_Id;
        u16 m_Port;
        std::string m_IP;
        SSEngine* m_Engine;
        UdpSocket* m_Socket;
        
        Scalar m_LastMessageTime;
        bool m_PendingPing;
        sf::Mutex* m_Mutex;
};
}

#endif