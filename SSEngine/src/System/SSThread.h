#ifndef SandboxSimulator_Thread_h
#define SandboxSimulator_Thread_h

#include <iostream>
#include <memory>
#include <cassert>
#include <windows.h>
#include <process.h>
#include <SFML/Network.hpp>

namespace SandboxSimulator {
class Runnable {
    public:
	    virtual void* run() = 0;
	    virtual ~Runnable() = 0;
};

class Thread {
    public:
	    Thread(std::auto_ptr<Runnable> run);
	    Thread();
	    virtual ~Thread();
	    void start();
	    void* join();
    private:
	    HANDLE hThread;
	    unsigned wThreadID;
	    // runnable object will be deleted automatically
	    std::auto_ptr<Runnable> runnable;
	    Thread(const Thread&);
	    const Thread& operator=(const Thread&);
	    // called when run() completes
	    void setCompleted();
	    // stores return value from run()
	    void* result;
	    virtual void* run() {return 0;}
	    static unsigned WINAPI startThreadRunnable(LPVOID pVoid);
	    static unsigned WINAPI startThread(LPVOID pVoid);
	    void printError(LPTSTR lpszFunction, LPSTR fileName, int lineNumber);
};

class ClientThread : public Thread
{
    public:
        ClientThread(sf::TcpSocket* Socket) : m_Socket(Socket) {};
        virtual void* run() {
            assert(m_Socket);
            bool keepGoing = true;
            while(keepGoing)
            {
		        char data[100];
		        std::size_t received;

                if (m_Socket->receive(data, 100, received) != sf::Socket::Done)
                {
                    keepGoing = false;
                }
		        std::string output;
		        for(int i = 0; i < received && i < 100; i++)
			        output += data[i];

                std::cout << "Received " << received << " bytes" << std::endl;
                std::cout << output << std::endl;
            }
            printf("Closed connection...\n");
            return (void*)0;
        }
    private:
        sf::TcpSocket* m_Socket;
};

}
#endif