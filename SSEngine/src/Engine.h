//
//  Engine.h
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#ifndef SandboxSimulator_Engine_h
#define SandboxSimulator_Engine_h

#include <System/SSTypes.h>
#include <Core/SceneGraph.h>
#include <Core/EngineSystem.h>
#include <Core/Message.h>
#include <Core/Timer.h>
#include <vector>
#include <SFML/Network.hpp>
#include <System/PlatformIncludes.h>
#include <System/InputSystem.h>

#ifdef __APPLE__
#define AllocArray(Type,Size) (Type *)__alloca(Size * sizeof(Type))
#else
#define AllocArray(Type,Size) (Type *)malloc(Size * sizeof(Type))
#endif
#define AllocStr(Sz) AllocArray(char,Sz)

#define Profile(Stuff, i) { Scalar first = GetElapsedTime(); Stuff; Scalar second = GetElapsedTime(); printf("%d - %s Executed in %0.2f ms.\n",i, #Stuff,(second-first) * 1000.0f); }

namespace SandboxSimulator
{
	class RenderSystem;
    class PhysicsSystem;
    class SSEngine
    {
        public:
            SSEngine(u8 DesiredFps, bool ShowFps);
            ~SSEngine();
    
            /* Initialize engine */
            void Initialize(i32 ArgC,Literal ArgV[]);
            /* Run engine */
            void Run();
            /* Shutdown engine */
            void Shutdown();
        
            /* Register engine systems */
            void RegisterSystem(EngineSystem* Sys);
        
            /* Broadcast message to systems */
            void Broadcast(EngineMessage* Msg);
			void SendMessage(MESSAGE_TYPE Type);
        
            /* Save the state of the engine */
            void Serialize() const;
            /* Load a previously saved engine state */
            void Deserialize();
        
            /* Engine log */
            void Log(Literal Text,...) const;
        
            SceneGraph* GetSceneGraph() {return m_SceneGraph;}

            Scalar GetElapsedTime() {return m_RunTime.ElapsedTime();}

            InputSystem* GetInputSystem() { return m_InputSystem; }

            /* Time of day */
            Scalar GetGameTimeRate() const { return m_GameTimeRate; }
            void   SetGameTimeRate(Scalar Rate) { m_GameTimeRate = Rate; }//if(m_GameTimeRate < 0) m_GameTimeRate = 0; }
            void   SetTimeOfDay(Scalar T) { m_TimeOfDay = T; if(m_TimeOfDay < 0) m_TimeOfDay = 0; }
            Scalar GetTimeOfDay() const { return m_TimeOfDay; }
            Scalar NormalizeTime(i32 Hour,i32 Minute,i32 Second,bool AM) const;
            i32 GetHour() const;
            i32 GetMinute() const;
            i32 GetSecond() const;
            bool IsAM() const;
            string GetTimeString() const;

            //Misc
            void SetRenderSystem(RenderSystem* rSys) { m_RenderSystem = rSys; }
            RenderSystem* GetRenderSystem() { return m_RenderSystem; }
    
            void SetPhysicsSystem(PhysicsSystem* rSys) { m_PhysicsSystem = rSys; }
            PhysicsSystem* GetPhysicsSystem() { return m_PhysicsSystem; }

            void SetShutdown(bool sh) { m_DoShutdown = sh; }

        protected:
            /* Systems */
			std::vector<EngineSystem*> m_Systems;
            std::vector<EngineMessage*> m_AsynchronousMessages;
			//RenderSystem* m_RenderSystem;

			/* Managers */
            SceneGraph* m_SceneGraph;
            InputSystem* m_InputSystem;
            RenderSystem* m_RenderSystem;
            PhysicsSystem* m_PhysicsSystem;

			/* Misc */
			Timer m_RunTime;
            Scalar m_LastTime;
            bool m_DoShutdown;

            /* Time of day stuff */
            Scalar m_GameTimeRate;
            Scalar m_TimeOfDay;
            Scalar m_Timestep;

            int m_NumFrames;
            Scalar m_FrameCounter;
            bool m_ShowFps;
            Scalar m_MinFrameTime;
    };
}

#endif
