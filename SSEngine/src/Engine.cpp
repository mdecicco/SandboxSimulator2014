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
    SSEngine::SSEngine(u8 DesiredFps, bool ShowFps) : m_FrameCounter(0), m_NumFrames(0), m_ShowFps(ShowFps)
    {
        if(DesiredFps > 0)
            m_MinFrameTime = (1.0f / DesiredFps) * 1000.0f;
        else
            m_MinFrameTime = 0;
        m_LastTime = 0.0f;
        m_DoShutdown = false;
        m_RunTime.Start();

        m_InputSystem = new InputSystem();
        RegisterSystem(m_InputSystem);

        m_TimeOfDay = 0.0f;
        m_GameTimeRate = 30;
        m_TimeOfDay = NormalizeTime(6,0,0,false);
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

		//size_t plen = strlen(ArgV[0]);
        //CString Path = MakeCString(plen);
        //strcpy(Path,ArgV[0]);
        //for(size_t i = plen - 1;i > 0;i--)
        //{
        //    if(Path[i] == '/') { Path[i] = 0; break; }
        //    Path[i] = 0;
        //}
        //FreeCString(Path);
		//Doesn't work on windows
		//chdir(Path);
        //Log("Path: %s\n",Path);

        /* Initialize entity manager */
        m_SceneGraph = new SceneGraph(this);
        
        /* Initialize engine systems */
        for(i32 i = 0;i < m_Systems.size();i++) m_Systems[i]->Initialize(this);
    }
    
    void SSEngine::Run()
    {
        Log("Starting engine.\n");
        
		if(m_Systems.size() == 0) m_DoShutdown = true;
        while(!m_DoShutdown)
        {
            Scalar dt = GetElapsedTime() - m_LastTime;
            m_LastTime = GetElapsedTime();

            //1 game second = 1/60th of a game minute
            //1 game minute = 1/60th of a game hour
            //1 game hour   = 1/24th of a game day
            //1 game day    = 1.0
            //1 game second = 1/60 * 1/60 * 1/24 = 86400^-1 = 0.00001157407
            m_TimeOfDay += (m_GameTimeRate * 0.00001157407f) * dt;
            if(m_TimeOfDay >= 1.0f) m_TimeOfDay = 0.0f;

            m_NumFrames++;
            m_FrameCounter += dt;
            
            /* Update systems */
            for(i32 i = 0;i < m_Systems.size();i++) {
                m_Systems[i]->Update(dt);
            }

            /* Process asynchronous messages */
            for(i32 m = 0;m < m_AsynchronousMessages.size();m++)
            {
				m_AsynchronousMessages[m]->m_IsSynchronous = true;
				Broadcast(m_AsynchronousMessages[m]);
            }
            m_AsynchronousMessages.clear();

            if(m_FrameCounter >= 1) {
                if(m_ShowFps)
                    printf("%d fps \n",m_NumFrames);
                m_FrameCounter = 0;
                m_NumFrames = 0;
            }
            Scalar EndTime = GetElapsedTime();
            Scalar FrameTime = (EndTime - m_LastTime)*1000.0f;
            if(FrameTime < m_MinFrameTime) {
                Scalar diff = m_MinFrameTime - FrameTime;
                sf::sleep(sf::milliseconds(diff));
            }
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
        free(Formatted);
    }

    Scalar SSEngine::NormalizeTime(i32 Hour,i32 Minute,i32 Second,bool AM) const
    {
        if(Hour   > 12) Hour   = 12;
        if(Hour   < 1 ) Hour   = 1;
        if(Minute > 59) Minute = 59;
        if(Minute < 0 ) Minute = 0;
        if(Second > 59) Second = 59;
        if(Second < 0 ) Second = 0;
        Scalar Hr = Hour - 1;
        if(!AM) Hr += 12.0f;
        Hr = Hr / 24.0f;
        Hr += Scalar(Minute) * 0.00069444446f;
        Hr += Scalar(Second) * 1.1574074e-05f;
        if(Hr > 1.0f) Hr = 1.0f - Hr;
        if(Hr < 0.0f) Hr += 1.0f;
        return Hr;
    }

    i32 SSEngine::GetHour() const
    {
        Scalar Hour = (24.0f * m_TimeOfDay);
        int h = int(floor(Hour)) + 1;
        if(h > 12) h -= 12;
        return h;
    }

    i32 SSEngine::GetMinute() const
    {
        Scalar Hour   = (24.0f * m_TimeOfDay);
        Scalar Minute = (Hour - floor(Hour)) * 60.0f;
        return floor(Minute);
    }

    i32 SSEngine::GetSecond() const
    {
        Scalar Hour   = (24.0f * m_TimeOfDay);
        Scalar Minute = (Hour - floor(Hour)) * 60.0f;
        Scalar Second = (Minute - floor(Minute)) * 60.0f;
        return floor(Second);
    }

    bool SSEngine::IsAM() const
    {
        return m_TimeOfDay < 0.5f - (1.0f / 24.0f) || m_TimeOfDay > 1.0f - (1.0f / 24.0f);
    }

    string SSEngine::GetTimeString() const
    {
        Scalar Hour   = (24.0f * m_TimeOfDay);
        Scalar Minute = (Hour   - floor(Hour)) * 60.0f;
        Scalar Second = (Minute - floor(Minute)) * 60.0f;
        int h = int(floor(Hour  )) + 1;
        int m = int(floor(Minute));
        int s = int(floor(Second));
        if(h > 12) { h -= 12; }
        bool ml10 = Minute < 10;
        bool sl10 = Second < 10;
        char str[32];
        memset(str,0,32);
        bool IsMorning = m_TimeOfDay < 0.5f - (1.0f / 24.0f) || m_TimeOfDay > 1.0f - (1.0f / 24.0f);
        //snprintf(str,32,"%d:%s%d:%s%d %s",h,(ml10 ? "0" : ""),m,(sl10 ? "0" : ""),s,IsMorning ? "AM" : "PM");
        return string(str);
    }
}
