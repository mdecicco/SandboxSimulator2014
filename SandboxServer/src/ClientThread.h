#ifndef SandboxSimulator_ClientThread_h
#define SandboxSimulotar_ClientThread_h

#include <System/SSThread.h>
#include <SFML/Network.hpp>

namespace SandboxSimulator {
class ClientThread : public Thread
{
    public:
        ClientThread(sf::TcpSocket* Socket) : m_Socket(Socket) {}
        
    private:
        virtual void run();
        sf::TcpSocket* m_Socket;
};
}

#endif