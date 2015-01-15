#ifndef SandboxSimulator_GameState_h
#define SandboxSimulator_GameState_h

#include <Core/StateSystem.h>
#include <ConnectionSystem.h>

namespace SandboxSimulator {
class GameState : public State
{
    public:
        GameState(ConnectionSystem* Conn);
        ~GameState();

    protected:
        virtual void Init(StateManagerSystem* Manager);
        virtual void Update(Scalar dt);
        StateManagerSystem* m_StateManager;
        ConnectionSystem* m_Connection;

    private:
        Scalar m_LastPacketTime;
};
}

#endif
