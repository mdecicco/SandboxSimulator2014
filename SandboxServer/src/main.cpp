 #include <iostream>
#include <stdio.h>

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Rendering/RenderSystem.h>
#include <Core/Message.h>
#include <Utils/Math.h>

#include <SFML/Network.hpp>
#include <System/SSThread.h>

#include <vector>

using namespace SandboxSimulator;

int main(i32 ArgC,Literal ArgV[])
{
    SSEngine* Eng = new SSEngine();

    Eng->Initialize(ArgC,ArgV);
	Eng->Run();

    sf::TcpListener listener;
    // bind the listener to a port
    if (listener.listen(56000) != sf::Socket::Done)
    {
        // error...
    }
    
    // accept a new connection
    std::vector<ClientThread*> Clients = std::vector<ClientThread*>();

    bool keepGoing = true;
    while(keepGoing) {
        sf::TcpSocket client;
        if (listener.accept(client) != sf::Socket::Done)
        {
            printf("Error connecting client!\n");
            keepGoing = false;
        }
        ClientThread* T = new ClientThread(&client);
        T->start();
        Clients.push_back(T);
    }

    for(int i = 0; i < Clients.size(); i++) {
        Clients[i]->join();
    }
    
    Eng->Shutdown();
    return 0;
}
