#include <ServerListener.h>
#include <System/SSTypes.h>

namespace SandboxSimulator
{
    ServerListener::ServerListener(u16 Port, ServerSystem* clientManager, sf::Mutex* mutex) : m_Mutex(mutex), m_Port(Port), m_Socket(new UdpSocket()), m_KeepGoing(true), m_ClientManager(clientManager), m_LastClientID(0)
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
            if(packet) {
                u32 PacketID;
                i8 PacketType;
                (*packet) >> PacketID >> PacketType;
                u16 ClientID;
                u16 NumClients = m_ClientManager->NumClients();

                switch(PacketType)
                {
                    case PT_CONNECT:
                        if(NumClients < CLIENT_LIMIT) {
                            u16 NewClientID = m_LastClientID+1;
                            if(!m_ClientManager->HasClient(sender.toString(), port)) {
                                m_ClientManager->NewClient(NewClientID, sender.toString(), port, m_Socket);
                                printf("New client connected with id %d\n", (i32)NewClientID);
                                m_LastClientID++;
                            } else
                                printf("Client already connected!\n");
                        } else {
                            printf("Reached maximum client ID limit\n");
                        }
                        break;
                    case PT_DISCONNECT:
                        i8 Reason;
                        (*packet) >> ClientID >> Reason;
                        if(m_ClientManager->HasClient(ClientID, sender.toString(), port)) {
                            Client* c = m_ClientManager->GetClient(ClientID);
                            m_ClientManager->RemoveClient(c);
                            c->Disconnect((DISCONNECT_REASON)Reason);
                        } else {
                            printf("The provided client ID could not be disconnected as it was not found.\n");
                        }
                        
                    break;
                    default:
                        (*packet) >> ClientID;
                        if(m_ClientManager->HasClient(ClientID, sender.toString(), port)) {
                            Client* c = m_ClientManager->GetClient(ClientID);
                            c->ParsePacket((PACKET_TYPE)PacketType, packet, PacketID);
                            if(PacketType != PT_ACK)
                                c->Acknowledge(PacketID);
                        } else
                            printf("Client %d is not connected.\n", ClientID);
                    break;
                }

                delete packet;
            }
        }
    }

    void ServerListener::Stop()
    {
        m_KeepGoing = false;
    }
}