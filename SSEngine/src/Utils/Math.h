#ifndef MATH_H
#define MATH_H

#include <Utils/Math/Functions.h>
#include <Utils/Math/Vec3.h>
#include <Utils/Math/Mat4.h>
#include <Utils/Math/Mat3.h>
#include <Utils/Math/Quat.h>
#include <Utils/Math/Vec2.h>
#include <Utils/Math/Vec4.h>

#include <SFML/Network.hpp>

namespace SandboxSimulator
{
    class TCPServer
    {
        public:
        sf::TcpListener listener;
        TCPServer() {}
        ~TCPServer() {}
        
        void Start() {
            // bind the listener to a port
            if (listener.listen(56000) != sf::Socket::Done)
            {
                // error...
            }
        
            // accept a new connection
            sf::TcpSocket client;
            if (listener.accept(client) != sf::Socket::Done)
            {
                // error...
            }
            
            bool keepGoing = true;
            while(keepGoing)
            {
				char data[100];
				std::size_t received;

                if (client.receive(data, 100, received) != sf::Socket::Done)
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
        }
    };
}

#endif
