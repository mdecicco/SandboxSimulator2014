#include <iostream>
#include <stdio.h>
#include <cstdlib>

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Rendering/RenderSystem.h>
#include <Core/Message.h>

#include <Network/Socket.h>
#include <System/SSTypes.h>

//Client includes
#include <Client/ConnectionSystem.h>
#include <Client/GameState.h>
#include <Core/StateSystem.h>

//Server includes
#include <Server/ServerListener.h>
#include <Server/ServerSystem.h>

#ifdef WIN32
    #include <Windows.h>
#endif

using namespace SandboxSimulator;

int main(i32 ArgC,Literal ArgV[])
{
    bool ServerMode = false;
    u16 Port = 3889;
    std::string Address = "127.0.0.1";
    bool Okay = false;
    bool ShowFPS = false;
    bool ShowHelp = false;

    for(i32 i = 0; i < ArgC; i++) {
        if(strcmp(ArgV[i], "-s") == 0 || strcmp(ArgV[i], "--server") == 0) {
            ServerMode = true;
            if(ArgC > i+1) {
                Port = (u16)atoi(ArgV[i+1]);
                i++;
            }
            Okay = true;
        } else if(strcmp(ArgV[i], "-c") == 0 || strcmp(ArgV[i], "--client") == 0) {
            Okay = true;
            if(ArgC > i+1) {
                Address = ArgV[i+1];
                if(ArgC > i+2) {
                    Port = (u16)atoi(ArgV[i+2]);
                    i++;
                }
                i++;
            }
        } else if(strcmp(ArgV[i], "-f") == 0 || strcmp(ArgV[i], "--fps") == 0) {
            ShowFPS = true;
        } else if(strcmp(ArgV[i], "-h") == 0 || strcmp(ArgV[i], "--help") == 0) {
            ShowHelp = true;
        } else if(i != 0) {
            printf("Did not recognize argument '%s'\n", ArgV[i]);
        }
    }

    if(ArgC == 1) {
        Okay = true;
    }

    if(Okay)
    {
        if(ServerMode) {
            SSEngine* Eng = new SSEngine(20,ShowFPS);
            sf::Mutex* mutex = new sf::Mutex();

            ServerSystem* system = new ServerSystem(mutex);
            Eng->RegisterSystem(system);
            Eng->Initialize(ArgC,ArgV);

            ServerListener* Listener = new ServerListener(Port, system, mutex);
            Listener->Start();
            Eng->Run();
            Listener->Join();
            Eng->Shutdown();
        } else {
            #ifdef WIN32
                //FreeConsole();
            #endif
            SSEngine* Eng = new SSEngine(0,ShowFPS);
            //Engine Systems
            StateManagerSystem* StateSystem = new StateManagerSystem();
            ConnectionSystem* ConnSystem = new ConnectionSystem();
            RenderSystem* RendSystem = new RenderSystem();

            Eng->RegisterSystem(ConnSystem);
            Eng->RegisterSystem(StateSystem);
            Eng->RegisterSystem(RendSystem);
            Eng->SetRenderSystem(RendSystem);
            Eng->Initialize(ArgC,ArgV);

            //Input won't process unless it has an active glfw window.
            Eng->GetInputSystem()->SetWindow(RendSystem->GetWindow());

            StateSystem->SetState(new GameState(ConnSystem, Address, Port));

            Eng->Run();
            Eng->Shutdown();
        }
    }
    if(!Okay || ShowHelp) {
        printf("SandboxSimulator - Tuner, tuner tuner tuner, tuner.\n");
        printf("Copyright (c) 2015 -- Jordan Duty\n\n");
        printf("Usage:\n");
        printf("  SandboxSimulator [options]\n");
        printf("  SandboxSimulator -s               (Runs the server, listening on port 3889)\n");
        printf("  SandboxSimulator -s 12345         (Runs the server, listening on port 12345)\n");
        printf("  SandboxSimulator -c               (Runs the client, connects to localhost)\n");
        printf("  SandboxSimulator -c [addr]        (Runs the client, connects to [addr]:3889)\n");
        printf("  SandboxSimulator -c [addr] [port] (Runs the client, connects to [addr]:[port])\n");
        printf("-c | --client                       !Put as the last command! Launches client\n");
        printf("-s | --server                       !Put as the last command! Launches server\n");
        printf("-f | --fps                          Print out FPS in the console.\n");
        printf("-h | --help                         Shows this help\n");
    }
    return 0;
}
