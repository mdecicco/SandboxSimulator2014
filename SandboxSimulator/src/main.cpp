#include <iostream>
#include <stdio.h>

#include <Engine.h>

using namespace SandboxSimulator;

#define TEST_COMPONENT_TYPE 99
class TestComponent : public Component
{
public:
    TestComponent(SSEngine* Eng) : Component(Eng,TEST_COMPONENT_TYPE), m_QuitTime(Scalar(5))
    {
        m_Time.Start();
    }
    ~TestComponent()
    {
    }
    
    Timer m_Time;
    Scalar m_QuitTime;
};

class TestSystem : public EngineSystem
{
public:
    TestSystem()
    {
    }
    virtual ~TestSystem()
    {
    }
    virtual void HandleMessage(EngineMessage* Msg)
    {
    }
    
    virtual void Initialize(SSEngine* ParentEngine)
    {
        m_Engine = ParentEngine;
        m_Engine->Log("Test system initialized.\n");
    }
    virtual void Update(Scalar dt)
    {
        for(i32 i = 0;i < m_Components.size();i++)
        {
            TestComponent* T = (TestComponent*)m_Components[i];
            
            m_Engine->Log("Update! %f\n", T->m_Time.ElapsedTime());
            
            if(T->m_Time >= T->m_QuitTime)
            {
                m_Engine->Log("Shutting down.\n");
                m_Engine->Broadcast(new ShutdownMessage());
            }
        }
    }
    virtual void Shutdown()
    {
        m_Engine->Log("Test system shutting down.\n");
    }
    
    virtual void Serialize()
    {
    }
    
    virtual void Deserialize()
    {
    }
    
    SSEngine* m_Engine;
};

int main(i32 ArgC,Literal ArgV[])
{
    SSEngine* Eng = new SSEngine(ArgC,ArgV);
    
    TestSystem* S = new TestSystem();
    S->AddComponentType(TEST_COMPONENT_TYPE);
    Eng->RegisterSystem(S);
    Eng->Initialize();
    
    Entity* E = Eng->GetSceneGraph()->CreateEntity();
    Eng->GetSceneGraph()->AddComponent(E,new TestComponent(Eng));
    
    Eng->Run();
    Eng->Shutdown();
    return 0;
}
