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
    class SSEngine
    {
        public:
            SSEngine(bool ShowFps);
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
    
        protected:
            /* Systems */
			std::vector<EngineSystem*> m_Systems;
            std::vector<EngineMessage*> m_AsynchronousMessages;
			//RenderSystem* m_RenderSystem;

			/* Managers */
            SceneGraph* m_SceneGraph;
            InputSystem* m_InputSystem;

			/* Misc */
			Timer m_RunTime;
            Scalar m_LastTime;
            bool m_DoShutdown;
            int m_NumFrames;
            Scalar m_FrameCounter;
            bool m_ShowFps;
    };
}

#endif
