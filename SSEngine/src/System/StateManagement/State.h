#ifndef STATE_H
#define STATE_H

#include <string.h>
#include <System/BcTypes.h>

namespace BearClaw {
class StateManager;

class State
{
private:

public:
    ~State() {DeInit();}
    virtual void Init(StateManager* m)=0;
    virtual void DeInit(){};
    virtual void OnActivate(){};
    virtual void OnDeactivate(){};
    virtual void Update(f64 DeltaTime)=0;
};
}

#endif