#ifndef SandboxSimulator_StateSystem_h
#define SandboxSimulator_StateSystem_h

#include <Core/EngineSystem.h>
#include <memory>

namespace SandboxSimulator {
class StateManagerSystem;
    
class State
{
    public:
        ~State() { DeInit(); }

    protected:
        friend class StateManagerSystem;

        virtual void Init(StateManagerSystem* StateManager)=0;
        virtual void DeInit(){}
        virtual void OnActivate(){}
        virtual void OnDeactivate(){}
        virtual void Update(Scalar DeltaTime)=0;
        SSEngine* m_Engine;
};
    
#define StatePtr std::unique_ptr<State>
    
class StateManagerSystem : public EngineSystem
{
    public:
        StateManagerSystem();
        ~StateManagerSystem();

        void SetState(State* DesiredState);

        virtual void HandleMessage(const EngineMessage* Msg);
    
        virtual void Initialize(SSEngine* Eng);
        virtual void Update(Scalar dt);
        virtual void Shutdown();
    
        virtual void Serialize();
        virtual void Deserialize();
    
    private:
        SSEngine* m_Engine;
        StatePtr m_ActiveState;

        bool m_Initialized;
};
}

#endif