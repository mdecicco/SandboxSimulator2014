#include <iostream>
#include <stdio.h>

#include <Engine.h>
#include <Core/SceneGraph.h>
#include <Rendering/RenderSystem.h>
#include <Core/Message.h>

using namespace SandboxSimulator;

int main(i32 ArgC,Literal ArgV[])
{
    SSEngine* Eng = new SSEngine();
    
    Eng->Initialize(ArgC,ArgV);

	Entity* E = Eng->GetSceneGraph()->CreateEntity();
	Eng->GetSceneGraph()->AddComponent(E, new RenderComponent());
    RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
    Shader* S = new Shader();
    S->Load("");
    r->SetShader(S);

    r->AddVertex(Vec3(0.5 , 0.5,0));
    r->AddVertex(Vec3(0.5 ,-0.5,0));
    r->AddVertex(Vec3(-0.5,-0.5,0));
    r->AddVertex(Vec3(-0.5, 0.5,0));
    r->AddVertex(Vec3( 0.5, 0.5,0));
    r->AddVertex(Vec3(-0.5,-0.5,0));

    Eng->Run();
    Eng->Shutdown();
    return 0;
}
