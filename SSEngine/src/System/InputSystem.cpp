#include <System/InputSystem.h>

namespace SandboxSimulator
{
    InputSystem::InputSystem() : EngineSystem(), m_Window(nullptr), m_WindowSet(false)
    {}
    InputSystem::~InputSystem() {}

    void InputSystem::HandleMessage(const EngineMessage* Msg)
    {}

    void InputSystem::Initialize(SSEngine* Eng)
    {
    
    }

    void InputSystem::Update(Scalar dt)
    {

    }

    void InputSystem::Shutdown()
    {

    }

    void InputSystem::Serialize(){}
    void InputSystem::Deserialize(){}

    //Input stuffs
    bool InputSystem::KeyDown(int key)
    {
        if(m_WindowSet && glfwGetKey(m_Window, key) == GLFW_PRESS) 
            return true;
        return false;
    }
};