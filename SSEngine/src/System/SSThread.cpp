#include <System/SSThread.h>

namespace SandboxSimulator
{
    Thread::Thread() : m_Thread(&Thread::run, this)
    {

    }

    void Thread::Start()
    {
        m_Thread.launch();
    }

    void Thread::Join()
    {
        m_Thread.wait();
    }
}