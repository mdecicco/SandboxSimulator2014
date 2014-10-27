/*
 *  EngineTasks.cpp
 *
 *  Created by Michael DeCicco on 9/19/14.
 *
 */

#include "EngineTasks.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "Engine.h"

namespace Reality
{
    SystemUpdateTask::SystemUpdateTask(EngineSystem* eSys,Scalar dt,Task* Parent) : Task(Parent), m_eSys(eSys), m_dt(dt)
    {
    }
    SystemUpdateTask::~SystemUpdateTask()
    {
    }
    void SystemUpdateTask::Run()
    {
        m_eSys->Update(m_dt);
    }
};