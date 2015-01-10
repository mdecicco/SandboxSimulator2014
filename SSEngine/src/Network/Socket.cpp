#include <Network/Socket.h>
#include <stdio.h>

namespace SandboxSimulator
{
    void UdpSocket::Bind(i16 port)
    {
        if (m_Socket.bind(port) != sf::Socket::Done)
        {
            printf("Error binding socket on port %d.\n", port);
        }
    }

    void UdpSocket::Send(sf::Packet* packet, sf::IpAddress address, i16 port)
    {
        if (m_Socket.send(*packet, address, port) != sf::Socket::Done)
        {
            printf("Error trying to send packet.\n");
        }
    }

    sf::Packet* UdpSocket::Receive(sf::IpAddress &address, u16 &port)
    {
        sf::Packet* packet = new sf::Packet();
        if (m_Socket.receive(*packet, address, port) != sf::Socket::Done)
        {
            printf("Error trying to receive packet.\n");
        }
        return packet;
    }

    void UdpSocket::SetBlocking(bool block)
    {
        m_Socket.setBlocking(block);
    }
}