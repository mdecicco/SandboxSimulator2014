#ifndef SandboxSimulator_ConnectionManager_h
#define SandboxSimulator_ConnectionManager_h

#include <Core/EngineSystem.h>
#include <Network/Socket.h>
#include <string>

namespace SandboxSimulator {
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
        void Ping();

        sf::Packet* CreatePacket(PACKET_TYPE Type);

        void Connect(std::string ServerAddress, u16 ServerPort);
        void Disconnect();

        UID GetEntityID() { return m_EntityID; }
        bool IsConnected() { return m_Connected; }

        bool NeedsUpdate() {return m_NeedsUpdate;}
        void NeedsUpdate(bool t) {m_NeedsUpdate = t;}

    private:
        Scalar m_LastMessageTime;

        std::string m_ServerAddress;
        u16 m_ServerPort;
        SSEngine* m_Engine;
        UdpSocket* m_Socket;

        bool m_ConnectionAttempted;
        bool m_Connected;
        bool m_NeedsUpdate;

        bool m_PendingPing;
        u32 m_PendingPingID;

        u32 m_LastPacketID;
        u32 m_LastStateUpdateSequence;
    
        u16 m_ClientID;
        UID m_EntityID;
};
};

#endif