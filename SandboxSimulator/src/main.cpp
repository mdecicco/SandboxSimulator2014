#include <iostream>
#include <stdio.h>

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Rendering/RenderSystem.h>
#include <Core/Message.h>

#include <Network/Socket.h>
#include <System/SSTypes.h>

#include <ConnectionSystem.h>

using namespace SandboxSimulator;

int main(i32 ArgC,Literal ArgV[])
{
    SSEngine* Eng = new SSEngine();
    
    ConnectionSystem* ConnSystem = new ConnectionSystem();
    Eng->RegisterSystem(ConnSystem);
	Eng->RegisterSystem(new RenderSystem());
    Eng->Initialize(ArgC,ArgV);

    //Has to happen after the engine is initialized
    ConnSystem->Connect("127.0.0.1", 3889);

    Eng->Run();
    Eng->Shutdown();
    return 0;
}
