#include <MenuState.h>
#include <Core/TransformComponent.h>
#include <Rendering/RenderSystem.h>
#include <Entities/EnemyEntity.h>

#include <GameState.h>

namespace LudumDare {
    MenuState::MenuState()
    {

    }

    MenuState::~MenuState()
    {

    }

    void MenuState::Init(StateManagerSystem* Manager)
    {
        m_Music = m_Engine->CreateSound("Data/Sound/AmericaFuckYeah.wav");
        m_Music->Play();
        m_Music->SetLoop(true);
        m_Music->SetVolume(2);
        m_Manager = Manager;

        SceneGraph* Scene = m_Engine->GetSceneGraph();
    }

    void MenuState::Update(Scalar DeltaTime)
    {
        if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_SPACE)) {
            //m_Music.pause();
            m_Manager->SetState(new GameState(m_Music));
        }
    }
}