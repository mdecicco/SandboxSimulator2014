#include <System/OS/Window.h>
#include <System/OS/PlatformIncludes.h>

namespace BearClaw {
BcWindow::BcWindow(WindowInitializer &Initializer)
{
    m_Window = new BackendWindow(Initializer);
    m_CloseRequested = false;
}

BcWindow::~BcWindow()
{
	delete m_Window;
}

void BcWindow::Create()
{
    m_Window->Create();
}

void BcWindow::Destroy()
{
    m_Window->Destroy();
}

Vec2 BcWindow::GetWindowSize()
{
    return m_Window->GetWindowSize();
}

bool BcWindow::GetCloseRequested()
{
    if(!m_CloseRequested)
        return m_Window->GetCloseRequested();
    else
        return true;
}

void BcWindow::PollEvents()
{
    m_Window->PollEvents();
}

void BcWindow::SwapBuffers()
{
    m_Window->SwapBuffers();
}

void BcWindow::MakeCurrent()
{
    m_Window->MakeCurrent();
}
}
