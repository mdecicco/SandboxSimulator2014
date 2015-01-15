//
//  Engine.cpp
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#include <Engine.h>
#include <iostream>
#include <stdarg.h>

/* Systems */
#include <Rendering/RenderSystem.h>

namespace SandboxSimulator
{
    SSEngine::SSEngine()
    {
        m_LastTime = 0.0f;
        m_DoShutdown = false;
        m_RunTime.Start();

        m_InputSystem = new InputSystem();
        RegisterSystem(m_InputSystem);
    }
    
    SSEngine::~SSEngine()
    {
    }
    
    void SSEngine::Initialize(i32 ArgC,Literal ArgV[])
    {
		Log("Initializing Engine...\n");
        /*
         * Initialization process:
         *  1.) Load engine configuration file
         *  2.) Set engine variables
         *  3.) Load init script (loads necessary systems & assets for startup)
         *  4.) Initialize systems
         */
        Log("SSEngine v%d.%d\n", SSENGINE_VERSION_MAJOR, SSENGINE_VERSION_MINOR);

		size_t plen = strlen(ArgV[0]);
        CString Path = MakeCString(plen);
        strcpy(Path,ArgV[0]);
        for(size_t i = plen - 1;i > 0;i--)
        {
            if(Path[i] == '/') { Path[i] = 0; break; }
            Path[i] = 0;
        }
        //FreeCString(Path);
		//Doesn't work on windows
		//chdir(Path);
        //Log("Path: %s\n",Path);

        /* Initialize entity manager */
        m_SceneGraph = new SceneGraph(this);

		/* Create Render System */
		//m_RenderSystem = new RenderSystem();
		//RegisterSystem(m_RenderSystem);
        
        /* Initialize engine systems */
        for(i32 i = 0;i < m_Systems.size();i++) m_Systems[i]->Initialize(this);
    }
    
    void SSEngine::Run()
    {
        Log("Starting engine.\n");
        
		if(m_Systems.size() == 0) m_DoShutdown = true;
        while(!m_DoShutdown)
        {
            Scalar dt = m_RunTime.ElapsedTime() - m_LastTime;
            m_LastTime = m_RunTime.ElapsedTime();
            
            /* Update systems */
            for(i32 i = 0;i < m_Systems.size();i++) m_Systems[i]->Update(dt);
            
            /* Process asynchronous messages */
            for(i32 m = 0;m < m_AsynchronousMessages.size();m++)
            {
				m_AsynchronousMessages[m]->m_IsSynchronous = true;
				Broadcast(m_AsynchronousMessages[m]);
            }
            m_AsynchronousMessages.clear();
        }
    }
    
    void SSEngine::Shutdown()
    {
		Log("Stopping Engine.\n");
        for(i32 i = 0;i < m_Systems.size();i++) m_Systems[i]->Shutdown();
        for(i32 i = 0;i < m_Systems.size();i++) delete m_Systems[i];
		delete m_SceneGraph;
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
			delete Msg;
			return;
        }
        //Save for the end of the frame
        m_AsynchronousMessages.push_back(Msg);
    }

	void SSEngine::SendMessage(MESSAGE_TYPE Type) {
		switch(Type)
		{
			case MT_SHUTDOWN:
			{
				Broadcast(new ShutdownMessage());
				return;
			}

			default:
			{
				return;
			}
		}
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
        va_list List;
		va_start(List, Text);
		int Size = strlen(Text) + 512;
		char *Formatted = AllocStr(Size);

		vsnprintf(Formatted, Size, Text, List);
		va_end(List);

		// save to log file i32 Ret = fprintf(m_Log, "%s", Formatted);
        printf("%f: %s",m_RunTime.ElapsedTime(),Formatted);
        //free(Formatted);
    }
}
