 #include <iostream>
#include <stdio.h>

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Rendering/RenderSystem.h>
#include <Core/Message.h>
#include <Utils/Math.h>

#include <ServerListener.h>
#include <ServerSystem.h>

#include <vector>

using namespace SandboxSimulator;

int main(i32 ArgC,Literal ArgV[])
{
    sf::Mutex* mutex = new sf::Mutex();

    SSEngine* Eng = new SSEngine();
    
    ServerSystem* system = new ServerSystem(mutex);
    Eng->RegisterSystem(system);

    Eng->Initialize(ArgC,ArgV);

    ServerListener* Listener = new ServerListener(3889, system, mutex);
    Listener->Start();

    Eng->Run();
    Eng->Shutdown();

    Listener->Join();
    return 0;
}
