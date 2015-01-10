#ifndef SandboxSimulator_Socket_h
#define SandboxSimulator_Socket_h

#include <SFML/Network.hpp>
#include <System/SSTypes.h>

namespace SandboxSimulator{
enum PACKET_TYPE
{
    PT_CONNECT,
    PT_DISCONNECT,
    PT_EVENT,
    PT_UPDATE,
    PT_ACK,
    PT_COUNT
};

class TcpSocket
{
    public:

    private:
};

class UdpSocket
{
    public:
        UdpSocket() {}
        ~UdpSocket() {}

        void Bind(i16 port);
        void Send(sf::Packet* packet, sf::IpAddress address, i16 port);
        sf::Packet* Receive(sf::IpAddress &address, u16 &port);
        void SetBlocking(bool Block);

    private:
        sf::UdpSocket m_Socket;
};
}

#endif