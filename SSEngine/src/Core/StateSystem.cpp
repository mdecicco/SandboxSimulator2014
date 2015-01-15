#include <Core/StateSystem.h>

namespace SandboxSimulator
{
    StateManagerSystem::StateManagerSystem() : m_ActiveState(std::move(StatePtr(nullptr))), m_Initialized(false)
    {}

    StateManagerSystem::~StateManagerSystem()
    {}

    void StateManagerSystem::SetState(State* DesiredState)
    {
        if(m_Initialized) {
            if(DesiredState) {
                if(m_ActiveState != nullptr) {
                    m_ActiveState.get()->DeInit();
                    m_ActiveState.get()->OnDeactivate();
                }

                m_ActiveState.reset(DesiredState);

                m_ActiveState.get()->m_Engine = m_Engine;
                m_ActiveState.get()->Init(this);
                m_ActiveState.get()->OnActivate();
            }
        }
    }

    void StateManagerSystem::HandleMessage(const EngineMessage* Msg)
    {}

    void StateManagerSystem::Initialize(SSEngine* Eng)
    {
        if(!m_Initialized) {
            m_Engine = Eng;
            m_Initialized = true;
        }
    }

    void StateManagerSystem::Update(Scalar dt)
    {
        if(m_ActiveState != nullptr)
            m_ActiveState.get()->Update(dt);
    }

    void StateManagerSystem::Shutdown()
    {
        if(m_Initialized) {
            if(m_ActiveState != nullptr) {
                m_ActiveState.get()->DeInit();
                m_ActiveState.get()->OnDeactivate();
            }
            m_Initialized = false;
        }
    }

    void StateManagerSystem::Serialize()
    {}

    void StateManagerSystem::Deserialize()
    {}
}