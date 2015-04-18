#include <iostream>
#include <stdio.h>
#include <cstdlib>

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Rendering/RenderSystem.h>
#include <Core/PhysicsSystem.h>
#include <Core/Message.h>

#include <System/SSTypes.h>

#include <MenuState.h>

#ifdef WIN32
    //#include <vld.h>
    #include <Windows.h>
#endif

using namespace SandboxSimulator;
using namespace LudumDare;

int main(i32 ArgC,Literal ArgV[])
{
    #ifdef WIN32
        //FreeConsole();
    #endif
    SSEngine* Eng = new SSEngine(0,false);
    //Engine Systems
    StateManagerSystem* StateSystem = new StateManagerSystem();
    RenderSystem* RendSystem = new RenderSystem();
    PhysicsSystem* PhysSystem = new PhysicsSystem();

    Eng->RegisterSystem(StateSystem);
    Eng->RegisterSystem(RendSystem);
    Eng->SetRenderSystem(RendSystem);
    Eng->RegisterSystem(PhysSystem);
    Eng->SetPhysicsSystem(PhysSystem);
    Eng->Initialize(ArgC,ArgV);

    //Input won't process unless it has an active glfw window.
    Eng->GetInputSystem()->SetWindow(RendSystem->GetWindow());

    StateSystem->SetState(new MenuState());

    Eng->Run();
    Eng->Shutdown();
    delete Eng;
    return 0;
}
