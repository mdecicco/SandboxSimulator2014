#include <System/OS/Window.h>
#include <System/OS/PlatformIncludes.h>

namespace SSEngine {
SSWindow::SSWindow(WindowInitializer &Initializer)
{
    m_Window = new BackendWindow(Initializer);
    m_CloseRequested = false;
}

SSWindow::~SSWindow()
{
    delete m_Window;
}

void SSWindow::Create()
{
    m_Window->Create();
}

void SSWindow::Destroy()
{
    m_Window->Destroy();
}

Vec2 SSWindow::GetWindowSize()
{
    return m_Window->GetWindowSize();
}

bool SSWindow::GetCloseRequested()
{
    if(!m_CloseRequested)
        return m_Window->GetCloseRequested();
    else
        return true;
}

void SSWindow::PollEvents()
{
    m_Window->PollEvents();
}

void SSWindow::SwapBuffers()
{
    m_Window->SwapBuffers();
}

void SSWindow::MakeCurrent()
{
    m_Window->MakeCurrent();
}
}
