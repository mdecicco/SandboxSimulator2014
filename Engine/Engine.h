/*
 *  Engine.h
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#pragma once
/* Utility includes */
#include "EngineTypes.h"
#include "Timer.h"
#include "Message.h"
#include "PropertyList.h"
#include "EngineMath.h"
#include "Text.h"

/* Systems */
#include "ScriptSystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "AnimationSystem.h"

/* Managers */
#include "EntityManager.h"
#include "FileManager.h"
#include "InputManager.h"
#include "TaskManager.h"

/* Libraries */
#include <vector>

using namespace std;

namespace Reality
{
    class FileManager;
    
    class Engine
    {
        public:
            ~Engine();
        
            static void CreateEngine();
            static Engine* GetEngine();
        
            /* Initialize engine */
            void Initialize(i32 ArgC,Literal ArgV[]);
            /* Run engine */
            void Run();
            /* Shutdown engine */
            void Shutdown();
        
            Entity* CreateEntity();
            void AddComponent(Entity* E,Component* Comp);
            void RemoveComponent(Entity* E,Component* Comp);
            void RemoveComponentByType(Entity* E,COMPONENT_TYPE Type);
        
            /* Register engine systems */
            void RegisterSystem(EngineSystem* Sys);
        
            /* Broadcast message to systems */
            void Broadcast(EngineMessage* Msg);
        
            /* Script functions */
            Entity* GetEntity(UID EID) const { return m_EntityManager->GetEntity(EID); }
            void SendMessage(MESSAGE_TYPE Type);
            void SendEvent(const Event& E);
            void SendEvent(Entity* Target,const Event& E);
            void MakeRenderable(Entity* Target);
            void MakeTransformable(Entity* Target);
            void MakeObserver(Entity* Target);
            void MakePhysical(Entity* Target,Scalar Mass,Scalar Restitution = 0.2f,Scalar Friction = 0.4f);
            void MakePhysicalWithShape(Entity* Target,Scalar Mass,Scalar Restitution = 0.2f,Scalar Friction = 0.4f,const string& Shape = "");
            void MakeLight(Entity* Target);
            Entity* GetCurrentCamera() const { return m_RenderSystem->GetCurrentCamera(); }
            void SetShader(Entity* Target,const string& Path);
            Entity* CreateScriptedEntity(const string& Path,Scalar Rate = Scalar(0));
            Entity* SpawnEntity(const string& Path);
            Entity* SpawnEntityPos(const string& Path,const Vector3& Pos);
            void DestroyEntity(Entity* Target);
            i32 Execute(const string& Path) { return m_ScriptSystem->RunScriptFromFile(Path.c_str()); }
            i32 GetEntityCount() const { return m_EntityManager->GetEntityCount(); }
            i32 GetVisibleEntityCount() const { return m_RenderSystem->GetVisibleEntityCount(); }
            Scalar GetRunTime() const { return m_RunTime.ElapsedTime(); }
            Scalar GetGameTimeRate() const { return m_GameTimeRate; }
            void   SetGameTimeRate(Scalar Rate) { m_GameTimeRate = Rate; }
            void   SetTimeOfDay(Scalar T) { m_TimeOfDay = T; }
            Scalar GetTimeOfDay() const { return m_TimeOfDay; }
            Scalar GetFramerate() const { return m_RenderSystem->GetFPS(); }
            Scalar GetTimestep () const { return m_Timestep; }
            Vector3 GetResolution() const { return Vector3(m_RenderSystem->GetResolution(),0.0f); }
            void ToggleDebugCullingDisplay() { m_RenderSystem->ToggleDebugCullDisplay(); }
            void ToggleDebugPhysicsDisplay() { m_RenderSystem->ToggleDebugPhysDisplay(); }
            /* Please don't mind my compulsive behaviors. */
            bool VarExists(const string& Name) { if(Name.length() == 0) { return false; } return m_Variables.Contains(Name); }
            Scalar  GetGameVariablef  (const string& Name)                      { if(Name.length() == 0) { return 0; }         return *m_Variables[Name];          }
            i32     GetGameVariablei  (const string& Name)                      { if(Name.length() == 0) { return 0; }         return *m_Variables[Name];          }
            bool    GetGameVariableb  (const string& Name)                      { if(Name.length() == 0) { return false; }     return *m_Variables[Name];          }
            Vector2 GetGameVariablefv2(const string& Name)                      { if(Name.length() == 0) { return Vector2(); } return *m_Variables[Name];          }
            Vector3 GetGameVariablefv3(const string& Name)                      { if(Name.length() == 0) { return Vector3(); } return *m_Variables[Name];          }
            Vector4 GetGameVariablefv4(const string& Name)                      { if(Name.length() == 0) { return Vector4(); } return *m_Variables[Name];          }
            string  GetGameVariableStr(const string& Name)                      { if(Name.length() == 0) { return Name;      } return string(*m_Variables[Name] ); }
            void    SetGameVariablef  (const string& Name,Scalar         Value) { if(Name.length() == 0) { return;           } m_Variables[Name]->SetValue(Value); }
            void    SetGameVariablei  (const string& Name,i32            Value) { if(Name.length() == 0) { return;           } m_Variables[Name]->SetValue(Value); }
            void    SetGameVariableb  (const string& Name,bool           Value) { if(Name.length() == 0) { return;           } m_Variables[Name]->SetValue(Value); }
            void    SetGameVariablefv2(const string& Name,const Vector2& Value) { if(Name.length() == 0) { return;           } m_Variables[Name]->SetValue(Value); }
            void    SetGameVariablefv3(const string& Name,const Vector3& Value) { if(Name.length() == 0) { return;           } m_Variables[Name]->SetValue(Value); }
            void    SetGameVariablefv4(const string& Name,const Vector4& Value) { if(Name.length() == 0) { return;           } m_Variables[Name]->SetValue(Value); }
            void    SetGameVariableStr(const string& Name,const string&  Value) { if(Name.length() == 0) { return;           } m_Variables[Name]->SetValue(Value); }
            void    AddGameVariablef  (const string& Name,Scalar         Value) { if(Name.length() == 0) { return;           } m_Variables.Insert(new PNode(Name,Value),Name.c_str()); }
            void    AddGameVariablei  (const string& Name,i32            Value) { if(Name.length() == 0) { return;           } m_Variables.Insert(new PNode(Name,Value),Name.c_str()); }
            void    AddGameVariableb  (const string& Name,bool           Value) { if(Name.length() == 0) { return;           } m_Variables.Insert(new PNode(Name,Value),Name.c_str()); }
            void    AddGameVariablefv2(const string& Name,const Vector2& Value) { if(Name.length() == 0) { return;           } m_Variables.Insert(new PNode(Name,Value),Name.c_str()); }
            void    AddGameVariablefv3(const string& Name,const Vector3& Value) { if(Name.length() == 0) { return;           } m_Variables.Insert(new PNode(Name,Value),Name.c_str()); }
            void    AddGameVariablefv4(const string& Name,const Vector4& Value) { if(Name.length() == 0) { return;           } m_Variables.Insert(new PNode(Name,Value),Name.c_str()); }
            void    AddGameVariableStr(const string& Name,const string&  Value) { if(Name.length() == 0) { return;           } m_Variables.Insert(new PNode(Name,Value),Name.c_str()); }
            File* OpenFile(const string& Filename,const string& Mode) { return m_FileManager->Open(Filename.c_str(),Mode.c_str()); }
            void CloseFile(File* fp) { m_FileManager->Close(fp); }
        
            /* Save the state of the engine */
            void Serialize() const;
            /* Load a previously saved engine state */
            void Deserialize();
        
            /* Engine log */
            static void Log(Literal Text,...);
            static void Log(Literal Text,va_list& Args);
        
            /* Systems */
            ScriptSystem*    GetScriptSystem   () const { return m_ScriptSystem   ; }
            RenderSystem*    GetRenderSystem   () const { return m_RenderSystem   ; }
            PhysicsSystem*   GetPhysicsSystem  () const { return m_PhysicsSystem  ; }
            AnimationSystem* GetAnimationSystem() const { return m_AnimationSystem; }
        
            /* Managers */
            EntityManager* GetEntityManager() const { return m_EntityManager; }
            FileManager*   GetFileManager  () const { return m_FileManager  ; }
            AssetManager*  GetAssetManager () const { return m_AssetManager ; }
            InputManager*  GetInputManager () const { return m_InputManager ; }
            TaskManager*   GetTaskManager  () const { return m_TaskManager  ; }
        
        protected:
            Engine();
            static Engine* m_Engine;
            
            /* Systems */
            vector<EngineSystem*> m_Systems;
            vector<EngineMessage*> m_AsynchronousMessages;
            ScriptSystem*    m_ScriptSystem;
            RenderSystem*    m_RenderSystem;
            PhysicsSystem*   m_PhysicsSystem;
            AnimationSystem* m_AnimationSystem;
        
            /* Managers */
            EntityManager* m_EntityManager;
            FileManager*   m_FileManager;
            AssetManager*  m_AssetManager;
            InputManager*  m_InputManager;
            TaskManager*   m_TaskManager;
        
            /* Runtime vars */
            PropertyList* m_Config;
            bool m_DoShutdown;
            Timer m_RunTime;
            Scalar m_LastTime;
            bool m_LogToConsole;
            bool m_LogToFile;
            Scalar m_GameTimeRate;
            Scalar m_TimeOfDay;
            Scalar m_Timestep;
        
            /* User variables */
            Hashmap<PNode*> m_Variables;
        
            /* Misc protected functions */
            void ReloadConf();
            void ReloadVars();
    };
};