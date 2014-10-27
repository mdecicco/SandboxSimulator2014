/*
 *  EngineTasks.h
 *
 *  Created by Michael DeCicco on 9/19/14.
 *
 */

#pragma once
#include "TaskManager.h"

namespace Reality
{
    class AssetManager;
    
    class EngineSystem;
    class SystemUpdateTask : public Task
    {
        public:
            SystemUpdateTask(EngineSystem* eSys,Scalar dt,Task* Parent);
            ~SystemUpdateTask();
        
            virtual void Run();
        
            EngineSystem* m_eSys;
            Scalar m_dt;
    };
};