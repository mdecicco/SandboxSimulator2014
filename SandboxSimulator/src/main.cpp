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
    
    //ConnectionSystem* ConnSystem = new ConnectionSystem();
    //Eng->RegisterSystem(ConnSystem);
	Eng->RegisterSystem(new RenderSystem());
    Eng->Initialize(ArgC,ArgV);

    //Has to happen after the engine is initialized
    //ConnSystem->Connect("127.0.0.1", 3889);

    Entity* E = Eng->GetSceneGraph()->CreateEntity();
    Eng->GetSceneGraph()->AddComponent(E, new RenderComponent());
    RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);

    Material* Mat = new Material();
    r->SetMaterial(Mat);
    MatVec3Node* Vec3Node = new MatVec3Node("ConstInputTest", Vec3(0,0,1));
    Mat->SetInput(MI_ALBEDO, Vec3Node->GetOutput());

    r->AddVertex(Vec3(0.5 , 0.5,0));
    r->AddVertex(Vec3(0.5 ,-0.5,0));
    r->AddVertex(Vec3(-0.5,-0.5,0));

    r->AddVertex(Vec3(-0.5,-0.5,0));
    r->AddVertex(Vec3(-0.5, 0.5,0));
    r->AddVertex(Vec3( 0.5, 0.5,0));

    Eng->Run();
    Eng->Shutdown();
    return 0;
}
