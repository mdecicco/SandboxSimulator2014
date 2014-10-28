#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <System/OS/PlatformIncludes.h>
#include <System/InputManager.h>

namespace SSEngine {
class StateManager;
class State;

class GameApp
{
private:
    int m_DisplayWidth, m_DisplayHeight;
    bool m_bIsCloseRequested, m_bIsClosing, m_bIsRunning;

    char* m_Title;

    SSWindow* GameWindow;

    //void RegisterEngineEvents(void);

protected:
    StateManager* m_StateManager;
    
public:
    GameApp(State* S);
	~GameApp();

    void Start(WindowInitializer WinInit);
    
    void RequestClose() {m_bIsCloseRequested = true;}
    StateManager* GetStateManager() {return m_StateManager;}
    bool IsRunning() {return m_bIsRunning;}

	void TestKeyCallback(char button, Action_Type Type);
};
}

#endif
