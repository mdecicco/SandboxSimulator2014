/*
 *  Threading.h
 *
 *  Created by Michael DeCicco on 9/19/14.
 *
 */

#pragma once
#include "EngineTypes.h"

#include <pthread.h>
#include <semaphore.h>

#ifdef __APPLE__
#include <mach/task.h>
#include <mach/semaphore.h>
#endif

namespace Reality
{
    class Mutex
    {
        public:
            Mutex();
            ~Mutex();
        
            void Lock();
            bool TryLock();
            void Unlock();
        
        protected:
            friend class ThreadCondition;
            pthread_mutex_t m_Mtx;
    };
    
    class Semaphore
    {
        public:
            Semaphore(i32 Value);
            ~Semaphore();
        
            i32 GetValue();
            void Increment();
            void Wait();
        
        protected:
            #ifdef __APPLE__
            semaphore_t m_Sem;
            #else
            sem_t m_Sem;
            #endif
            i32 m_Value;
    };
    
    template <typename T>
    class LockingPtr
    {
        public:
            LockingPtr(volatile T& Obj,Mutex& Mtx) : m_Obj(const_cast<T*>(&Obj)), m_Mtx(&Mtx) { Mtx.Lock(); }
            ~LockingPtr() { m_Mtx->Unlock(); }
    
            T& operator* () { return *m_Obj; }
            T* operator->() { return  m_Obj; }
        
        protected:
           T* m_Obj;
           Mutex* m_Mtx;
    };
    
    class ThreadCondition
    {
        public:
            ThreadCondition();
            ~ThreadCondition();
        
            void Signal();
            void WaitSignal();
        
        protected:
            pthread_cond_t m_Cond;
            Mutex m_Mutex;
    };
};