//
//  Engine.cpp
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#include <Engine.h>
#include <unistd.h>
#include <iostream>
#include <stdarg.h>

namespace SandboxSimulator
{
    SSEngine::SSEngine(i32 ArgC,Literal ArgV[])
    {
        m_LastTime = 0.0f;
        m_DoShutdown = false;
        m_RunTime.Start();
        usleep(100);
        
        size_t plen = strlen(ArgV[0]);
        CString Path = MakeCString(plen);
        strcpy(Path,ArgV[0]);
        for(size_t i = plen - 1;i > 0;i--)
        {
            if(Path[i] == '/') { Path[i] = 0; break; }
            Path[i] = 0;
        }
        chdir(Path);
        Log("Path: %s\n",Path);
    }
    
    SSEngine::~SSEngine()
    {
    }
    
    void SSEngine::Initialize()
    {
        /*
         * Initialization process:
         *  1.) Load engine configuration file
         *  2.) Set engine variables
         *  3.) Load init script (loads necessary systems & assets for startup)
         *  4.) Initialize systems
         */
        /* Initialize entity manager */
        m_SceneGraph = new SceneGraph(this);
        
        /* Initialize engine systems */
        for(i32 i = 0;i < m_Systems.size();i++) m_Systems[i]->Initialize(this);
    }
    
    void SSEngine::Run()
    {
        Log("Starting engine.\n");
        /*
         * Single-threaded run loop:
         *  1.) Update input
         *  2.) Update systems
         *  3.) Process & send messages
         *  4.) Go to 1
         *
         * Multi-threaded run loop:
         *  1.) Start system threads
         *  2.) Profit ????
         */
        
        while(!m_DoShutdown)
        {
            Scalar dt = m_RunTime - m_LastTime;
            m_LastTime = m_RunTime;
            
            /* Update systems */
            for(i32 i = 0;i < m_Systems.size();i++) m_Systems[i]->Update(dt);
            
            /* Process asynchronous messages */
            for(i32 m = 0;m < m_AsynchronousMessages.size();m++)
            {
                for(i32 i = 0;i < m_Systems.size();i++)
                {
                    if(m_Systems[i]->AcceptsMessageType(m_AsynchronousMessages[m]->m_MessageType)) m_Systems[i]->HandleMessage(m_AsynchronousMessages[m]);
                }
                
                delete m_AsynchronousMessages[m];
            }
            m_AsynchronousMessages.clear();
        }
    }
    
    void SSEngine::Shutdown()
    {
        /*
         * Stop all systems
         * Unload all assets
         * Destroy all systems
         * Destroy self
         */
        for(i32 i = 0;i < m_Systems.size();i++) m_Systems[i]->Shutdown();
        for(i32 i = 0;i < m_Systems.size();i++) delete m_Systems[i];
    }

    void SSEngine::RegisterSystem(EngineSystem* Sys)
    {
        m_Systems.push_back(Sys);
    }
    
    void SSEngine::Broadcast(EngineMessage* Msg)
    {
        if(Msg->m_IsSynchronous)
        {
            switch(Msg->m_MessageType)
            {
                case SandboxSimulator::MT_COMPONENT_ADDED:
                {
                    Component* C = ((ComponentAddedMessage*)Msg)->m_Component;
                    for(i32 i = 0;i < m_Systems.size();i++)
                    {
                        if(m_Systems[i]->AcceptsComponentType(C->GetType())) m_Systems[i]->AddComponent(C);
                    }
                    break;
                    return;
                }
                case SandboxSimulator::MT_COMPONENT_REMOVED:
                {
                    Component* C = ((ComponentAddedMessage*)Msg)->m_Component;
                    for(i32 i = 0;i < m_Systems.size();i++)
                    {
                        if(m_Systems[i]->AcceptsComponentType(C->GetType())) m_Systems[i]->RemoveComponent(C);
                    }
                    break;
                    return;
                }
                case SandboxSimulator::MT_SHUTDOWN:
                {
                    m_DoShutdown = true;
                    break;
                };
                case SandboxSimulator::MT_INVALID:
                {
                    break;
                }
                default:
                {
                    break;
                }
            }
            
            for(i32 i = 0;i < m_Systems.size();i++)
            {
                if(m_Systems[i]->AcceptsMessageType(Msg->m_MessageType)) m_Systems[i]->HandleMessage(Msg);
            }
        }
        //Save for the end of the frame
        m_AsynchronousMessages.push_back(Msg);
    }
    
    void SSEngine::Serialize() const
    {
        /*
         * Save the state of the engine
         */
    }
    
    void SSEngine::Deserialize()
    {
        /*
         * Load the state of the engine
         */
    }
    
    void SSEngine::Log(Literal Text,...) const
    {
        va_list Args;
        va_start(Args,Text);
        CString Str = MakeCString(strlen(Text) + 256);
        vasprintf(&Str,Text,Args);
        va_end(Args);
        
        printf("%f: %s",m_RunTime.ElapsedTime(),Str);
    }
};