#ifndef SandboxSimulator_ServerListener_h
#define SandboxSimulator_ServerListener_h

#include <System/SSThread.h>
#include <System/SSTypes.h>
#include <Network/Socket.h>
#include <Client.h>

#include <ServerSystem.h>

#include <map>

namespace SandboxSimulator {
class ServerListener : public Thread
{
    public:
        ServerListener(u16 Port, ServerSystem* system, sf::Mutex* mutex);
        ~ServerListener();
        void Stop();

    private:
        virtual void run();

        ServerSystem* m_ClientManager;
        u16 m_LastClientID;
        UdpSocket* m_Socket;
        bool m_KeepGoing;
        u16 m_Port;

        sf::Mutex* m_Mutex;
};
}

#endif