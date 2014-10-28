#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <string>
#include <System/BcTypes.h>
#include <map>

namespace BearClaw {
class GameApp;
class State;

class StateManager
{
public:
    typedef std::map<std::string, State*> AliveStatesMap;

private:
    GameApp* m_Game;
    State* m_CurrentState;
    AliveStatesMap m_AliveStates;

public:
    StateManager(State* S);
    ~StateManager();
    void Init(GameApp* G);
	void DeInit();
    void ChangeState(State* S);
    void ChangeState(State* S, std::string StoreStateAliveKey);
    void SetActiveState(std::string Key);
    void SetActiveState(std::string FindKey, std::string StoreStateAliveKey);
    void UpdateState(f64 DeltaTime);
    void InsertAliveState(std::string Key, State* S);
    State* FindAliveState(std::string Key);

    State* GetState()
    {
        return m_CurrentState;
    }

    GameApp* GetGame()
    {
        return m_Game;
    }
};
}

#endif