#ifndef SandboxSimulator_ConnectionManager_h
#define SandboxSimulator_ConnectionManager_h

#include <Core/EngineSystem.h>
#include <Network/Socket.h>
#include <string>

namespace SandboxSimulator
{
class ConnectionSystem : public EngineSystem
{
    public:
        ConnectionSystem();
        ~ConnectionSystem();

        //System functions
        virtual void HandleMessage(const EngineMessage* Msg);

		virtual void Initialize(SSEngine* Eng);
		virtual void Update(Scalar dt);
		virtual void Shutdown();

		virtual void Serialize();
	    virtual void Deserialize();

        //Connection functions
        bool Send(sf::Packet* Packet);
        void Acknowledge(i32 PacketID);

        sf::Packet* CreatePacket(PACKET_TYPE Type);

        void Connect(std::string ServerAddress, u16 ServerPort);
        void Disconnect();

    private:
        Scalar m_LastMessageTime;

        std::string m_ServerAddress;
        u16 m_ServerPort;
        SSEngine* m_Engine;
        UdpSocket* m_Socket;

        bool m_ConnectionAttempted;
        bool m_Connected;

        u32 m_LastPacketID;
    
        u16 m_ClientID;
        u32 m_EntityID;
};
};

#endif