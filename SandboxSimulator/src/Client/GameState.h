#ifndef SandboxSimulator_GameState_h
#define SandboxSimulator_GameState_h

#include <Core/StateSystem.h>
#include <Client/ConnectionSystem.h>

#include <string>

namespace SandboxSimulator {
class GameState : public State
{
    public:
        GameState(ConnectionSystem* Conn, std::string Address, u16 Port);
        ~GameState();

    protected:
        virtual void Init(StateManagerSystem* Manager);
        virtual void Update(Scalar dt);
        StateManagerSystem* m_StateManager;
        ConnectionSystem* m_Connection;

    private:
        Scalar m_LastPacketTime;
        u16 m_Port;
        std::string m_Address;

        bool MouseHeld;
        TransformComponent* CamTrans;
};
}

#endif
