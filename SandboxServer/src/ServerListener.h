#ifndef SandboxSimulator_ServerListener_h
#define SandboxSimulator_ServerListener_h

#include <System/SSThread.h>
#include <Network/Socket.h>
#include <Client.h>

#include <map>

namespace SandboxSimulator {
class ServerListener : public Thread
{
    public:
        ServerListener(u16 Port, ClientManager* clientManager);
        ~ServerListener();
        void Stop();

    private:
        virtual void run();
        ClientManager* m_ClientManager;
        UdpSocket* m_Socket;
        bool m_KeepGoing;
        u16 m_Port;
};
}

#endif