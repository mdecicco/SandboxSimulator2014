#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <Engine.h>
#include <Core/StateSystem.h>
#include <Rendering/Mesh.h>
#include <Rendering/Gui/Font.h>

#include <Sound/SoundEffect.h>

using namespace SandboxSimulator;
namespace LudumDare {

class ArtilleryEntity;
class Enemy;
class Player;

class GameState : public State
{
    public:
        GameState(SoundEffect* BackgroundMusic);
        ~GameState();
        
        std::vector<Enemy*>& GetEnemyVector() { return m_Enemies; }

    protected:
        virtual void Init(StateManagerSystem* Manager);
        virtual void Update(Scalar dt);

    private:
        StateManagerSystem* m_Manager;
        Player* m_Player;
        ArtilleryEntity* m_HoboGunnerTest;
        std::vector<Enemy*> m_Enemies;
        SoundEffect* m_BackgroundMusic;
        SoundEffect* m_Explosion;
};
}

#endif