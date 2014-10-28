#ifndef GLFW_BACKEND_H
#define GLFW_BACKEND_H

#include <GLFW/glfw3.h>
#include <System/OS/Window.h>

namespace BearClaw {

class BackendWindow
{
private:
    GLFWwindow*         m_Window;
    WindowInitializer   m_Initializer;

public:
    BackendWindow(WindowInitializer &Initializer);
    ~BackendWindow();
    void Create();
    void MakeCurrent();
    bool GetCloseRequested();
    Vec2 GetWindowSize();
    void SwapBuffers();
    void PollEvents();
    void Destroy();

    GLFWwindow* GetWindow() {return m_Window;}
};

/*
 *Get current Time
 */
f64 GetTimeInMS();
f64 GetTimeInSeconds();

#define BCGetTimeInMS       GetTimeInMS
#define BCGetTimeInSeconds  GetTimeInSeconds

/*
 *Input
 */
struct InputImpl
{
    bool    HideCursor;
    u16     KeyToBC[256];
};
}
#endif
