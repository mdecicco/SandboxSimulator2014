#ifndef SandboxSimulator_Socket_h
#define SandboxSimulator_Socket_h

#include <SFML/Network.hpp>
#include <System/SSTypes.h>

namespace SandboxSimulator{
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