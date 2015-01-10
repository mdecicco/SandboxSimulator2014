 #include <iostream>
#include <stdio.h>

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Rendering/RenderSystem.h>
#include <Core/Message.h>
#include <Utils/Math.h>

#include <ServerListener.h>

#include <vector>

using namespace SandboxSimulator;

int main(i32 ArgC,Literal ArgV[])
{
    ClientManager* clientManager = new ClientManager();
    ServerListener* Listener = new ServerListener(3889, clientManager);
    Listener->Start();

    Listener->Join();
    return 0;
}
