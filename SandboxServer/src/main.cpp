#include <iostream>
#include <stdio.h>

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Rendering/RenderSystem.h>
#include <Core/Message.h>
#include <Utils/Math.h>

using namespace SandboxSimulator;

int main(i32 ArgC,Literal ArgV[])
{
    SSEngine* Eng = new SSEngine();
    
    Eng->Initialize(ArgC,ArgV);

    Entity* E = Eng->GetSceneGraph()->CreateEntity();
    Eng->GetSceneGraph()->AddComponent(E, new RenderComponent());
    RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);

    Material* Mat = new Material();
    r->SetMaterial(Mat);
    MatVec3Node* Vec3Node = new MatVec3Node("ConstInputTest", Vec3(1,0,1));
    Mat->SetInput(MI_ALBEDO, Vec3Node->GetOutput());

    r->AddVertex(Vec3(0.5 , 0.5,0));
    r->AddVertex(Vec3(0.5 ,-0.5,0));
    r->AddVertex(Vec3(-0.5,-0.5,0));

    r->AddVertex(Vec3(-0.5,-0.5,0));
    r->AddVertex(Vec3(-0.5, 0.5,0));
    r->AddVertex(Vec3( 0.5, 0.5,0));

    TCPServer* Test = new TCPServer();
    Test->Start();
    
    Eng->Run();
    Eng->Shutdown();
    return 0;
}
