#ifndef MENU_STATE_H
#define MENU_STATE_H

#include <Engine.h>
#include <Core/StateSystem.h>
#include <Rendering/Mesh.h>
#include <Rendering/Gui/Font.h>
#include <Entities/Player.h>
#include <Entities/EnemyEntity.h>

#include <SFML/Audio.hpp>

using namespace SandboxSimulator;
namespace LudumDare {
class MenuState : public State
{
    public:
        MenuState();
        ~MenuState();

    protected:
        virtual void Init(StateManagerSystem* Manager);
        virtual void Update(Scalar dt);

    private:
        StateManagerSystem* m_Manager;
        SoundEffect* m_Music;
};
}

#endif