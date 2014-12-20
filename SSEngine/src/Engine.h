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

#ifdef __APPLE__
#define AllocArray(Type,Size) (Type *)__alloca(Size * sizeof(Type))
#else
#define AllocArray(Type,Size) (Type *)malloc(Size * sizeof(Type))
#endif
#define AllocStr(Sz) AllocArray(char,Sz)

namespace SandboxSimulator
{
    class SSEngine
    {
        public:
            SSEngine(i32 ArgC, Literal Argv[]);
            ~SSEngine();
    
            /* Initialize engine */
            void Initialize();
            /* Run engine */
            void Run();
            /* Shutdown engine */
            void Shutdown();
        
            /* Register engine systems */
            void RegisterSystem(EngineSystem* Sys);
        
            /* Broadcast message to systems */
            void Broadcast(EngineMessage* Msg);
        
            /* Save the state of the engine */
            void Serialize() const;
            /* Load a previously saved engine state */
            void Deserialize();
        
            /* Engine log */
            void Log(Literal Text,...) const;
        
            SceneGraph* GetSceneGraph() {return m_SceneGraph;}
    
        protected:
            Timer m_RunTime;
            Scalar m_LastTime;
            std::vector<EngineSystem*> m_Systems;
            std::vector<EngineMessage*> m_AsynchronousMessages;
            SceneGraph* m_SceneGraph;
            bool m_DoShutdown;
    };
}

#endif
