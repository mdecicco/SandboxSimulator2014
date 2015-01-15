#include <iostream>
#include <stdio.h>

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Rendering/RenderSystem.h>
#include <Core/Message.h>

#include <Network/Socket.h>
#include <System/SSTypes.h>

#include <ConnectionSystem.h>
#include <Core/StateSystem.h>

#include <GameState.h>

using namespace SandboxSimulator;

int main(i32 ArgC,Literal ArgV[])
{
    SSEngine* Eng = new SSEngine();

    //Engine Systems
    StateManagerSystem* StateSystem = new StateManagerSystem();
    ConnectionSystem* ConnSystem = new ConnectionSystem();
    RenderSystem* RendSystem = new RenderSystem();

    Eng->RegisterSystem(ConnSystem);
    Eng->RegisterSystem(StateSystem);
    Eng->RegisterSystem(RendSystem);
    Eng->Initialize(ArgC,ArgV);

    //Input won't process unless it has an active glfw window.
    Eng->GetInputSystem()->SetWindow(RendSystem->GetWindow());

    StateSystem->SetState(new GameState(ConnSystem));

    Eng->Run();
    Eng->Shutdown();
    return 0;
}
