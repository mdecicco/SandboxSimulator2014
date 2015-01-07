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
            if (listener.listen(53000) != sf::Socket::Done)
            {
                // error...
            }
        
            // accept a new connection
            sf::TcpSocket client;
            if (listener.accept(client) != sf::Socket::Done)
            {
                // error...
            }
            printf("Client connected...\n");
        }
    };
}

#endif
