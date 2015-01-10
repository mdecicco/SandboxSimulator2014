#include <ServerListener.h>
#include <System/SSTypes.h>

namespace SandboxSimulator
{
    ServerListener::ServerListener(u16 Port, ClientManager* clientManager) : m_Port(Port), m_Socket(new UdpSocket()), m_KeepGoing(true), m_ClientManager(clientManager)
    {}

    ServerListener::~ServerListener()
    {}

    void ServerListener::run()
    {
        m_Socket->Bind(m_Port);
        while(m_KeepGoing)
        {
            sf::Packet *packet;
            sf::IpAddress sender;
            u16 port;
            packet = m_Socket->Receive(sender, port);
            i32 PacketID;
            i8 PacketType;
            (*packet) >> PacketID >> PacketType;
            i8 ClientID;
            i8 NumClients = m_ClientManager->NumClients();

            switch(PacketType)
            {
                case PT_CONNECT:
                    if(NumClients < 256) {
                        Byte NewClientID = NumClients+1;
                        if(!m_ClientManager->HasClient(sender.toString(), port)) {
                            m_ClientManager->NewClient(NewClientID, sender.toString(), port)->Acknowledge(PacketID);
                        } else
                            printf("New client connected with id %d\n", (i32)NewClientID);
                    } else {
                        printf("Reached maximum client ID limit: 255\n");
                    }
                break;
                case PT_DISCONNECT:
                    (*packet) >> ClientID;
                    if(m_ClientManager->RemoveClient(ClientID, sender.toString(), port)) {
                        Client* c = m_ClientManager->GetClient(ClientID);
                        c->Disconnect();
                    } else {
                        printf("The provided client ID could not be desconnected as it was not found.");
                    }
                        
                break;
                default:
                    (*packet) >> ClientID;
                    if(m_ClientManager->HasClient(ClientID, sender.toString(), port)) {
                        Client* c = m_ClientManager->GetClient(ClientID);
                        c->ParsePacket((PACKET_TYPE)PacketType, packet);
                        c->Acknowledge(PacketID);
                    } else
                        printf("Client %d is not connected.\n", ClientID);
                break;
            }

            delete packet;
        }
    }

    void ServerListener::Stop()
    {
        m_KeepGoing = false;
    }
}