#include <ClientThread.h>
#include <iostream>
#include <assert.h>

namespace SandboxSimulator
{
    void ClientThread::run()
    {
        assert(m_Socket);
        while(true)
        {
		    char data[100];
		    std::size_t received;
            int err = m_Socket->receive(data, 100, received);
            if (err != sf::Socket::Done)
            {
                switch(err)
                {
                    case 1:
                        printf("Socket not ready!\n");
                        break;
                    case 2:
                        printf("Client disconnected!\n");
                        break;
                    case 3:
                        printf("Socket error!\n");
                        break;
                    default:
                        break;
                }
                return;
            }
		    std::string output;
		    for(int i = 0; i < received && i < 100; i++)
		        output += data[i];

            std::cout << "Received " << received << " bytes" << std::endl;
            std::cout << output << std::endl;
        }
    }
}