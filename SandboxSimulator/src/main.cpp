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

    Material* Mat = new Material();
    ConstVec3Node* MatVec3 = new ConstVec3Node();
    MatVec3->m_Output->SetData(Vec3(0.0,1.0,0.0));
    Mat->m_Albedo->SetInput(MatVec3->m_Output);

    //Multiply material node... should come out to 0.5, that gets multiplied by the albedo in the shader, meaning the color should end up being 0r,0.5g,0b
    MultiplyNode* MultNode = new MultiplyNode();
    MultNode->m_AInput->SetInput(new MaterialOutput<f32>(0.25));
    MultNode->m_BInput->SetInput(new MaterialOutput<f32>(2.0));
    //Eng->Log("Output of mult node: %f\n", MultNode->m_Output->GetData());

    Mat->m_Opacity->SetInput(MultNode->m_Output);
    r->SetMaterial(Mat);

    r->SetShader(S);

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
