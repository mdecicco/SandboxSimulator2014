#ifndef MAINSTATE_H
#define MAINSTATE_H

#include <System/StateManagement/StateManager.h>
#include <System/StateManagement/State.h>

using namespace SSEngine;

namespace SandboxSimulator {
class MainState : public State
{
private:

public:
    MainState();
    ~MainState();

    void Init(StateManager *m);
    void DeInit();
    void OnActivate();
    void OnDeactivate();
    void Update(f64 DeltaTime);
};
}

#endif
