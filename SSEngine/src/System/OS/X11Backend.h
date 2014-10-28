#ifndef GLFW_BACKEND_H
#define GLFW_BACKEND_H


#include <System/OS/Window.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glew.h>
#include <GL/glx.h>

namespace BearClaw {

class BackendWindow
{
public:
    Display*            m_Display;
    GLXContext          glxContext;
    GLXFBConfig*        glxConfig;
    WindowInitializer   m_Initializer;
    Window              m_Window        = 0;
    Colormap            m_Colormap      = 0;
    timeval             StartTime;
    bool                CloseRequested  = false;

public:
    BackendWindow(WindowInitializer &Initializer);
    ~BackendWindow() {Destroy();}
    void Create();
    void MakeCurrent();
    bool GetCloseRequested();
    Vec2 GetWindowSize();
    void SwapBuffers();
    void PollEvents();
    void Destroy();

    void ProcessEvent(XEvent* Event);
    void CreateInternal();
    void CreateNativeWindow();
    void CreateDisplay();
    void CreateConfig();
    void CreateContext();
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
    Cursor  EmptyCursor = 0;
    u16     KeyToBC[256];
};
}
#endif
