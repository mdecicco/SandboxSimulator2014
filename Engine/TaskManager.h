/*
 *  TaskManager.h
 *
 *  Created by Michael DeCicco on 9/15/14.
 *
 */

#pragma once
#include "EngineTypes.h"
#include "Threading.h"
#include <vector>
#include <queue>
using namespace std;


namespace Reality
{
    class Task
    {
        public:
            Task(Task* Parent = 0);
            virtual ~Task();
        
            bool IsReady() const { return m_DependenciesLeft == 0; }
            void _Run() { Run(); if(m_Parent) m_Parent->OnDependantCompleted(); }
        
            virtual void Run() = 0;
        
        protected:
            friend class TaskManager;
            void OnDependantCompleted() { m_DependenciesLeft--; }
            i32 m_ID;
            Task* m_Parent;
            i32 m_DependenciesLeft;
    };
    class TaskManager;
    class WorkerThread
    {
        public:
            WorkerThread(TaskManager* Manager);
            ~WorkerThread();
        
            bool Start();
            void Stop();
        
            void Idle();
            bool IsIdling() const { return m_IsIdling; }
            void Run();
        
        protected:
            friend class TaskManager;
            bool m_Shutdown;
            bool m_IsIdling;
            i32 m_ID;
            TaskManager* m_Manager;
            pthread_t m_Thread;
            ThreadCondition m_Wake;
            ThreadCondition m_Sleep;
    };
    
    class TaskManager
    {
        public:
            TaskManager();
            ~TaskManager();
        
            i32 GetCoreCount() const;
            void AddTask(Task* T);
        
            Task* GetSingleTask();
            void WorkUntilDone();
        
            void WakeThreads();
            void WaitForThreads() const;
        
            bool HasWork() const { return m_Tasks.size() != 0; }
            
        protected:
            friend class WorkerThread;
            vector<Task*> m_Tasks;
            vector<WorkerThread*> m_Threads;

            Mutex* m_TaskMutex;
    };
};