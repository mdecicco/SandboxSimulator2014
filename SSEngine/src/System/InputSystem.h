#ifndef SandboxSimulator_InputSystem_h
#define SandboxSimulator_InputSystem_h

#include <Core/EngineSystem.h>
#include <GLFW/glfw3.h>

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

    private:
        bool m_WindowSet;
        GLFWwindow* m_Window;
};
}

#endif