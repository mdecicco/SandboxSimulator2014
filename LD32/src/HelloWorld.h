#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

#include <Engine.h>
#include <Core/StateSystem.h>
#include <Rendering/Mesh.h>
#include <Rendering/Gui/Font.h>
#include <Entities/Player.h>
#include <Entities/EnemyEntity.h>

using namespace SandboxSimulator;
namespace LudumDare {
class HelloWorld : public State
{
    public:
        HelloWorld();
        ~HelloWorld();

    protected:
        virtual void Init(StateManagerSystem* Manager);
        virtual void Update(Scalar dt);

    private:
        StateManagerSystem* m_Manager;
        Player* m_Player;
        std::vector<Enemy*> m_Enemies;
};
}

#endif