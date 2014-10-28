#include <System/StateManagement/StateManager.h>
#include <System/StateManagement/State.h>
#include <System/StateManagement/GameApp.h>
#include <System/Environment.h>

namespace BearClaw {
StateManager::StateManager(State* S)
{
    m_CurrentState = S;
}

StateManager::~StateManager()
{
    delete m_CurrentState;
}

void StateManager::Init(GameApp* G)
{
    m_Game = G;
    BC_LOG("State manager initializing...\n");
    m_CurrentState->Init(this);
    BC_LOG("Done initializing state manager.\n");
}

void StateManager::DeInit() {
	if (m_CurrentState)
		m_CurrentState->DeInit();
	//delete m_CurrentState;
}

void StateManager::ChangeState(State* S)
{
    State* OldState = m_CurrentState;
	OldState->DeInit();
    m_CurrentState = S;
	if (S)
		m_CurrentState->Init(this);
    delete OldState;
}

void StateManager::ChangeState(State* S, std::string Key)
{
    State* OldState = m_CurrentState;
    m_CurrentState = S;
    m_CurrentState->Init(this);

    OldState->OnDeactivate();
    InsertAliveState(Key, OldState);
}

void StateManager::SetActiveState(std::string Key)
{
    State* AliveState = FindAliveState(Key);

    if(AliveState != NULL)
    {
        State* OldState = m_CurrentState;
		OldState->DeInit();

        m_CurrentState = AliveState;
        m_CurrentState->OnActivate();
        
        delete OldState;
    }
    else
    {
        BC_LOG("ERROR: The State Could not be found.\n");
    }
}

void StateManager::SetActiveState(std::string Key, std::string SaveKey)
{
    State* AliveState = FindAliveState(Key);

    if(AliveState != NULL)
    {
        State* OldState = m_CurrentState;

        m_CurrentState = AliveState;
        m_CurrentState->OnActivate();
        
        OldState->OnDeactivate();
        InsertAliveState(SaveKey, OldState);
    }
    else
    {
        BC_LOG("ERROR: The State Could not be found.\n");
    }
}

State* StateManager::FindAliveState(std::string Key)
{
    for(AliveStatesMap::iterator it = m_AliveStates.begin(); it != m_AliveStates.end(); it++)
    {
        if(it->first == Key)
            return it->second;
    }

    return nullptr;
    BC_LOG("ERROR: The State Could not be found.\n");
}

void StateManager::InsertAliveState(std::string Key, State* S)
{
    m_AliveStates.insert(std::make_pair(Key, S));
}

void StateManager::UpdateState(f64 DeltaTime)
{
    m_CurrentState->Update(DeltaTime);
}
}