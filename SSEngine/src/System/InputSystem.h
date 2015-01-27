#ifndef SandboxSimulator_InputSystem_h
#define SandboxSimulator_InputSystem_h

#include <Core/EngineSystem.h>
#include <GLFW/glfw3.h>
#include <Utils/Math.h>

namespace SandboxSimulator {
class InputSystem : public EngineSystem
{
    public:
        InputSystem();
        ~InputSystem();

        virtual void HandleMessage(const EngineMessage* Msg);

		virtual void Initialize(SSEngine* Eng);
		virtual void Update(Scalar dt);
		virtual void Shutdown();

		virtual void Serialize();
	    virtual void Deserialize();

        void SetWindow(GLFWwindow* window) { m_Window = window; m_WindowSet = true; }
        bool KeyDown(int key);
        bool ButtonDown(int button);

        Vec2 GetMousePosition();
        void SetMousePosition(Vec2 Pos);

        void DisableCursor(bool disabled);
        Vec2 GetMouseDelta();

    private:
        bool m_WindowSet;
        GLFWwindow* m_Window;
        Vec2 m_OldMousePosition;
};
}

#endif