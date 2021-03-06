#include <System/InputSystem.h>

namespace SandboxSimulator
{
    InputSystem::InputSystem() : EngineSystem(), m_Window(nullptr), m_WindowSet(false), m_OldMousePosition(Vec2(0,0))
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

    Vec2 InputSystem::GetMousePosition() 
    {
        double x = 0;
        double y = 0;
        if(m_WindowSet)
            glfwGetCursorPos(m_Window, &x, &y);

        return Vec2(x,y);
    }

    void InputSystem::SetMousePosition(Vec2 Pos)
    {
        if(m_WindowSet)
            glfwSetCursorPos(m_Window, Pos.x, Pos.y);
    }

    //Input stuffs
    bool InputSystem::KeyDown(int key)
    {
        if(m_WindowSet && glfwGetKey(m_Window, key) == GLFW_PRESS) 
            return true;
        return false;
    }

    bool InputSystem::ButtonDown(int button)
    {
        if(m_WindowSet && glfwGetMouseButton(m_Window, button) == GLFW_PRESS) 
            return true;
        return false;
    }

    Vec2 InputSystem::GetMouseDelta()
    {
        Vec2 pos = GetMousePosition();
        Vec2 old = m_OldMousePosition;
        m_OldMousePosition = pos;
        return pos - old;
    }

    void InputSystem::DisableCursor(bool disable)
    {
        if(m_WindowSet) {
            if(disable) {
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
};