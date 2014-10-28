#ifndef WINDOW_H
#define WINDOW_H

#include <System/BcTypes.h>
#include <Utils/Math.h>

namespace BearClaw {
class BackendWindow;

struct WindowInitializer
{
    //stupid non initializer lists support.
    u32         Width;//           =   800;
    u32         Height;//          =   600;
    u32         rgbaBits[4];//     =   {8,8,8,0};
    u32         DepthBits;//       =   0;
    u32         StencilBits;//     =   0;
    u32         SamplesCount;//    =   0;
    bool        FullScreen;//      =   false;
    bool        Resizable;//       =   false;
    u32         MajorVersion;//    =   3;
    u32         MinorVersion;//    =   3;
    const char* Title;//           =   "BearClaw Engine";
};

class BcWindow
{
private:
    BackendWindow*      m_Window;
    bool                m_CloseRequested;

public:
    BcWindow(WindowInitializer &Initializer);
    ~BcWindow();
    void Create();
    void MakeCurrent();
    bool GetCloseRequested();
    Vec2 GetWindowSize();
    void SwapBuffers();
    void PollEvents();
    void Destroy();

    BackendWindow*  GetBackendWindow()  { return m_Window; }

    int GetWidth()
    {
        return GetWindowSize().x;
    }

    int GetHeight()
    {
        return GetWindowSize().y;
    }

    void SetCloseRequested(bool close)
    {
        m_CloseRequested = close;
    }
};
}
#endif
