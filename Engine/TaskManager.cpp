/*
 *  TaskManager.cpp
 *
 *  Created by Michael DeCicco on 9/15/14.
 *
 */

#include "TaskManager.h"

#ifdef __APPLE__
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
#include <mach/mach_init.h>
#endif

#include "Engine.h"

#include <thread>
using namespace std;

namespace Reality
{
    void SetThreadAffinity(i32 ThreadID)
    {
        #ifdef __APPLE__
        thread_extended_policy_data_t epolicy;
        epolicy.timeshare = FALSE;
        kern_return_t ret;
        ret = thread_policy_set(mach_thread_self(),THREAD_EXTENDED_POLICY,(thread_policy_t)&epolicy,THREAD_EXTENDED_POLICY_COUNT);
        if(ret != KERN_SUCCESS) Log("thread_policy_set returned %d", ret);

        thread_affinity_policy_data_t apolicy;
        apolicy.affinity_tag = ThreadID + 1;

        ret = thread_policy_set(mach_thread_self(),THREAD_EXTENDED_POLICY,(thread_policy_t)&apolicy,THREAD_EXTENDED_POLICY_COUNT);
        if(ret != KERN_SUCCESS) Log("thread_policy_set returned %d", ret);
        #else
        Log("Unsupported...\n");
        abort();
        #endif
    }
    
    
    void* ThreadFunc(void* Ptr)
    {
        ((WorkerThread*)Ptr)->Run();
        return 0;
    }
    Task::Task(Task* Parent) : m_Parent(Parent)
    {
        m_DependenciesLeft = 0;
        if(Parent) Parent->m_DependenciesLeft++;
    }
    Task::~Task()
    {
    }
    WorkerThread::WorkerThread(TaskManager* Manager) : m_Manager(Manager), m_Shutdown(false)
    {
    }
    WorkerThread::~WorkerThread()
    {
        pthread_join(m_Thread,0);
    }
    bool WorkerThread::Start()
    {
        return pthread_create(&m_Thread,0,ThreadFunc,this) != 0;
    }
    void WorkerThread::Stop()
    {
        m_Shutdown = true;
    }
    void WorkerThread::Idle()
    {
        m_IsIdling = true;
        m_Sleep.Signal();
        m_Wake .WaitSignal();
        m_IsIdling = false;
    }
    void WorkerThread::Run()
    {
        SetThreadAffinity(m_ID);
        
        while(!m_Shutdown)
        {
            /* Idle */
            Idle();
            
            /* Do work */
            while(m_Manager->HasWork())
            {
                /* Get one task */
                Task* T = m_Manager->GetSingleTask();
                if(!T) continue;
                
                /* Perform task */
                T->_Run();
                
                delete T;
            }
        }
        pthread_exit(0);
    }
    
    TaskManager::TaskManager()
    {
        i32 pCount = GetCoreCount() - 1;
        m_TaskMutex = new Mutex();
        
        for(i32 i = 0;i < pCount;i++)
        {
            WorkerThread* w = new WorkerThread(this);
            w->m_ID = (i32)m_Threads.size();
            m_Threads.push_back(w);
        }
        for(i32 i = 0;i < pCount;i++) m_Threads[i]->Start();
    }
    TaskManager::~TaskManager()
    {
        for(i32 i = 0;i < m_Threads.size();i++) m_Threads[i]->Stop();
        WakeThreads();
        for(i32 i = 0;i < m_Threads.size();i++) delete m_Threads[i];
    }

    i32 TaskManager::GetCoreCount() const
    {
        return std::thread::hardware_concurrency();
    }
    void TaskManager::AddTask(Task* T)
    {
        while(!m_TaskMutex->TryLock()) { }
        T->m_ID = (i32)m_Tasks.size();
        m_Tasks.push_back(T);
        m_TaskMutex->Unlock();
    }
    Task* TaskManager::GetSingleTask()
    {
        while(!m_TaskMutex->TryLock()) { if(m_Tasks.size() == 0) return 0; }
        
        Task* r = 0;
        for(i32 i = (i32)m_Tasks.size() - 1;i >= 0;i--)
        {
            if(m_Tasks[i]->IsReady())
            {
                r = m_Tasks[i];
                m_Tasks.erase(m_Tasks.begin() + i);
                break;
            }
        }
            
        
        m_TaskMutex->Unlock();
        return r;
    }
    void TaskManager::WorkUntilDone()
    {
        while(HasWork())
        {
            /* Get one task */
            Task* T = GetSingleTask();
            if(!T) continue;
            
            /* Perform task */
            T->_Run();
            
            delete T;
        }
    }

    void TaskManager::WakeThreads()
    {
        for(i32 i = 0;i < m_Threads.size();i++)
        {
            if(m_Threads[i]->IsIdling()) m_Threads[i]->m_Wake.Signal();
        }
    }
    void TaskManager::WaitForThreads() const
    {
        for(i32 i = 0;i < m_Threads.size();i++)
        {
            if(!m_Threads[i]->IsIdling()) m_Threads[i]->m_Sleep.WaitSignal();
        }
    }
};




