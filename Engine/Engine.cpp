/*
 *  Engine.cpp
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#include "Engine.h"
#include "FileManager.h"
#include "ScriptFunctionality.h"
#include "Lights.h"
#include "EngineTasks.h"

#include <unistd.h>
#include <iostream>
#include <stdarg.h>

namespace Reality
{
    Engine* Engine::m_Engine = 0;
    Engine::Engine()
    {
        m_Config = 0;
        m_LastTime = 0.0f;
        m_TimeOfDay = 0.0f;
        m_GameTimeRate = 1.0f / 16.0f;
        m_DoShutdown = false;
        m_RunTime.Start();
        m_FileManager = 0;
        m_EntityManager = 0;
        m_AssetManager = 0;
        m_ScriptSystem = 0;
    }
    Engine::~Engine()
    {
    }
    void Engine::CreateEngine()
    {
        if(m_Engine) return;
        m_Engine = new Engine();
    }
    Engine* Engine::GetEngine()
    {
        return m_Engine;
    }
    void Engine::Initialize(i32 ArgC,Literal *ArgV)
    {
        /*
         * Initialization process:
         *  1.) Load engine configuration file
         *  2.) Set engine variables
         *  3.) Load init script (loads necessary systems & assets for startup)
         *  4.) Initialize systems
         */
        
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
        FreeCString(Path);
        
        /* Initialize managers */
        m_FileManager   = new FileManager();
        m_EntityManager = new EntityManager();
        m_AssetManager  = new AssetManager();
        m_InputManager  = new InputManager();
        m_TaskManager   = new TaskManager();
        
        /* Load config file */
        m_Config = (PropertyList*)m_AssetManager->LoadAsset("Config.plst",AT_PROPERTYLIST);
        if(!m_Config)
        {
            m_LogToConsole = m_LogToFile = true;
            Log("Fatal error: Config.plst must exist in the executable directory.\n");
            exit(-1);
        }
        PNode* Sys = m_Config->Search("System");
        if(Sys)
        {
            PNode* DataDir = Sys->Search("Data Directory");
            if(DataDir)
            {
                Literal Dir = *DataDir;
                string NewConfigPath = "../";
                for(i32 i = 0;Dir[i] != 0;i++)
                {
                    if(Dir[i] == '/') NewConfigPath.append("../");
                }
                NewConfigPath.append("Config.plst");
                chdir(Dir);
                char d[256];
                memset(d,0,256);
                getcwd(d,256);
                
                m_Config->Destroy();
                m_Config = (PropertyList*)m_AssetManager->LoadAsset(NewConfigPath.c_str(),AT_PROPERTYLIST);
                Sys = m_Config->Search("System");
            }
        }
        
        /* remove log if exists */
        remove("EngineLog.txt");
        
        ReloadVars();
        ReloadConf();
        
        Log("Reality Engine v%d.%d\n",REALITY_VERSION_MAJOR,REALITY_VERSION_MINOR);
        Log("Initializing...\n");
        
        /* Create script system */
        m_ScriptSystem = new ScriptSystem();
        m_ScriptSystem->AddComponentType(CT_SCRIPT);
        m_ScriptSystem->AddMessageType(MT_DESTROY_ENTITY);
        m_ScriptSystem->AddMessageType(MT_USER_EVENT);
        m_ScriptSystem->AddMessageType(MT_TARGETED_USER_EVENT);
        m_ScriptSystem->AddMessageType(MT_ADD_SCRIPT);
        
        /* Create physics system */
        m_PhysicsSystem = new PhysicsSystem();
        m_PhysicsSystem->AddComponentType(CT_TRANSFORM);
        m_PhysicsSystem->AddMessageType(MT_MAKE_PHYSICAL);
        m_PhysicsSystem->AddMessageType(MT_DESTROY_ENTITY);
        
        /* Create render system */
        m_RenderSystem = new RenderSystem(m_Config->Search("Graphics"));
        m_RenderSystem->AddComponentType(CT_RENDER);
        m_RenderSystem->AddMessageType(MT_DESTROY_ENTITY);
        m_RenderSystem->AddMessageType(MT_MAKE_TRANSFORMABLE);
        m_RenderSystem->AddMessageType(MT_MAKE_CAMERA);
        m_RenderSystem->AddMessageType(MT_MAKE_RENDERABLE);
        m_RenderSystem->AddMessageType(MT_SET_SHADER);
        
        /* Create animation system */
        m_AnimationSystem = new AnimationSystem();
        m_AnimationSystem->AddComponentType(CT_ANIMATION);
        
        /* Register systems */
        RegisterSystem(m_ScriptSystem);
        RegisterSystem(m_PhysicsSystem);
        RegisterSystem(m_AnimationSystem);
        RegisterSystem(m_RenderSystem);
        
        /* Initialize engine systems */
        for(i32 i = 0;i < m_Systems.size();i++) m_Systems[i]->Initialize();
        m_InputManager->Init();
        
        /* Register script functionality */
        RegisterScriptMath();
        RegisterScriptFunctionality();
        
        /* Run program startup script */
        if(Sys)
        {
            PNode* StartupScript = Sys->Search("Startup Script");
            if(StartupScript)
            {
                i32 r = m_ScriptSystem->RunScriptFromFile(*StartupScript);
                Log("Startup script returned %d.\n",r);
                if(r != 0) SendMessage(MT_SHUTDOWN);
            }
            else
            {
                Log("<System><Startup Script> must be defined in Config.plst");
                SendMessage(MT_SHUTDOWN);
            }
        }
        else
        {
            Log("<System><Startup Script> must be defined in Config.plst");
            SendMessage(MT_SHUTDOWN);
        }
    }
    void Engine::Run()
    {
        Log("Starting engine.\n");
        
        if(m_Systems.size() == 0) m_DoShutdown = true;
        while(!m_DoShutdown)
        {
            Scalar dt = m_RunTime - m_LastTime;
            m_LastTime = m_RunTime;
            m_TimeOfDay += m_GameTimeRate * dt;
            if(m_TimeOfDay >= 1.0f) m_TimeOfDay = 0.0f;
            
            /* Create per-frame tasks */
            SystemUpdateTask* RenderTask  = new SystemUpdateTask(m_RenderSystem ,dt,0);
            SystemUpdateTask* PhysicsTask = new SystemUpdateTask(m_PhysicsSystem,dt,RenderTask);
            SystemUpdateTask* ScriptTask  = new SystemUpdateTask(m_ScriptSystem ,dt,PhysicsTask);
            
            m_TaskManager->AddTask(ScriptTask);
            m_TaskManager->AddTask(PhysicsTask);
            m_RenderSystem->CreateRenderTask();
            m_TaskManager->WakeThreads();
            m_TaskManager->WorkUntilDone();
            
            /* Update assets if necessary */
            m_AssetManager->ReloadChangedAssets();
            
            /* Render must be done on main thread */
            RenderTask->Run();
            
            m_TaskManager->WaitForThreads();
            
            /* Update config if necessary */
            if(m_Config->HasUpdated())
            {
                ReloadConf();
                ReloadVars();
                m_Config->ResetUpdateFlag();
            }
            
            /* Process asynchronous messages */
            if(m_AsynchronousMessages.size() > 0)
            {
                for(i32 m = 0;m < m_AsynchronousMessages.size();m++)
                {
                    m_AsynchronousMessages[m]->m_IsSynchronous = true;
                    Broadcast(m_AsynchronousMessages[m]);
                }
                m_AsynchronousMessages.clear();
            }
            
            while(m_RunTime - m_LastTime < m_Timestep) { usleep(500); }
        }
    }
    void Engine::Shutdown()
    {
        Log("Stopping engine.\n");
        for(i32 i = 0;i < m_EntityManager->GetEntityCount();i++)
        {
            if(m_EntityManager->GetEntity(i)) m_EntityManager->GetEntity(i)->Destroy();
        }
        for(i32 i = 0;i < m_Systems.size();i++)        m_Systems[i]->Shutdown();
        for(i32 i = 0;i < m_Systems.size();i++) delete m_Systems[i];
        m_Config->Destroy();
        delete m_AssetManager;
        delete m_EntityManager;
        delete m_FileManager;
        delete m_TaskManager;
    }
    Entity* Engine::CreateEntity()
    {
        return m_EntityManager->CreateEntity();
    }
    void Engine::AddComponent(Entity* E,Component* Comp)
    {
        m_EntityManager->AddComponent(E,Comp);
    }
    void Engine::RemoveComponent(Entity* E,Component* Comp)
    {
        m_EntityManager->RemoveComponent(E,Comp);
    }
    void Engine::RemoveComponentByType(Entity* E,COMPONENT_TYPE Type)
    {
        m_EntityManager->RemoveComponentByType(E,Type);
    }
    void Engine::RegisterSystem(EngineSystem* Sys)
    {
        m_Systems.push_back(Sys);
    }
    void Engine::Broadcast(EngineMessage* Msg)
    {
        if(Msg->m_MessageType == MT_MAKE_RENDERABLE && !m_RenderSystem->IsRendering()) Msg->m_IsSynchronous = true;
        if(Msg->m_IsSynchronous)
        {
            switch(Msg->m_MessageType)
            {
                case Reality::MT_COMPONENT_ADDED:
                {
                    Component* C = ((ComponentAddedMessage*)Msg)->m_Component;
                    for(i32 i = 0;i < m_Systems.size();i++)
                    {
                        if(m_Systems[i]->AcceptsComponentType(C->GetType())) m_Systems[i]->AddComponent(C);
                    }
                    break;
                }
                case Reality::MT_COMPONENT_REMOVED:
                {
                    Component* C = ((ComponentAddedMessage*)Msg)->m_Component;
                    for(i32 i = 0;i < m_Systems.size();i++)
                    {
                        if(m_Systems[i]->AcceptsComponentType(C->GetType())) m_Systems[i]->RemoveComponent(C);
                    }
                    break;
                }
                case Reality::MT_SHUTDOWN:
                {
                    m_DoShutdown = true;
                    break;
                };
                case Reality::MT_INVALID:
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
            if(Msg->m_MessageType == MT_DESTROY_ENTITY)
            {
                while(((DestroyEntityMessage*)Msg)->m_Entity->Destroy() > 0)
                {
                }
            }
            delete Msg;
            return;
        }
        //Save for the end of the frame
        m_AsynchronousMessages.push_back(Msg);
    }
    void Engine::SendMessage(MESSAGE_TYPE Type)
    {
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
    void Engine::SendEvent(const Event& E)
    {
        Broadcast(new UserEventMessage(E));
    }
    void Engine::SendEvent(Entity* Target,const Event& E)
    {
        Broadcast(new TargetedUserEventMessage(E,Target));
    }
    void Engine::MakeRenderable(Entity* Target)
    {
        Broadcast(new MakeRenderableMessage(Target));
    }
    void Engine::MakeTransformable(Entity* Target)
    {
        Broadcast(new MakeTransformableMessage(Target));
    }
    void Engine::MakeObserver(Entity* Target)
    {
        Broadcast(new MakeCameraMessage(Target));
    }
    void Engine::MakePhysical(Entity* Target,Scalar Mass,Scalar Restitution,Scalar Friction)
    {
        Broadcast(new MakePhysicalMessage(Target,Mass,Restitution,Friction));
    }
    void Engine::MakePhysicalWithShape(Entity* Target,Scalar Mass,Scalar Restitution,Scalar Friction,const string& Shape)
    {
        Broadcast(new MakePhysicalMessage(Target,Mass,Restitution,Friction,Shape));
    }
    void Engine::MakeLight(Entity* Target)
    {
        AddComponent(Target,new LightComponent());
    }
    void Engine::SetShader(Entity* Target,const string& Path)
    {
        Broadcast(new SetShaderMessage(Target,Path));
    }
    Entity* Engine::CreateScriptedEntity(const string& Path,Scalar Rate)
    {
        Entity* E = CreateEntity();
        E->AddScript(Path,Rate);
        return E;
    }
    Entity* Engine::SpawnEntity(const string& Path)
    {
        if(Path.length() == 0) return 0;
        Entity* E = CreateEntity();
        E->m_Definition = (PropertyList*)m_AssetManager->LoadAsset(Path.c_str(),AT_PROPERTYLIST);
        if(E->m_Definition)
        {
            PNode* Base = E->m_Definition->Search("Entity");
            if(!Base)
            {
                Log("Unable to load entity definition. (No base \"<Entity>\" node found)\n");
                E->Destroy();
                return 0;
            }
            
            PNode* Script       = Base->Search("Script");
            PNode* Render       = Base->Search("Render");
            PNode* XForm        = Base->Search("Transformable");
            PNode* Mass         = Base->Search("Mass");
            PNode* Restitution  = Base->Search("Restitution");
            PNode* Friction     = Base->Search("Friction");
            PNode* IsObserver   = Base->Search("Observer");
            PNode* IsStatic     = Base->Search("IsStatic");
            PNode* Position     = Base->Search("Position");
            PNode* Orientation  = Base->Search("Orientation");
            PNode* Dimensions   = Base->Search("Dimensions");
            PNode* Shape        = Base->Search("Shape");
            PNode* GUI          = Base->Search("GUI");
            
            if(XForm)
            {
                if(XForm->IsValue()      && (bool)*XForm)
                {
                    MakeTransformable(E);
                    TransformComponent* t = (TransformComponent*)E->GetComponentByType(CT_TRANSFORM);
                    if(Position && Position->IsValue()) t->Translate(*Position);
                    if(Orientation && Orientation->IsValue()) { Vector4 r = *Orientation; t->Rotate(r.xyz(),r.w); }
                    if(Dimensions && Dimensions->IsValue()) { t->SetDimensions(*Dimensions); }
                }
            }
            if(IsObserver && IsObserver->IsValue() && (bool)*IsObserver) MakeObserver(E);
            if(IsStatic   && IsStatic  ->IsValue() && (bool)*IsStatic  ) ((TransformComponent*)E->GetComponentByType(CT_TRANSFORM))->m_IsStatic = true;
            if(Render) MakeRenderable(E);
            if(Mass && Mass->IsValue())
            {
                Scalar r = 0.2f,f = 0.4f;
                if(Restitution && Restitution->IsValue()) r = *Restitution;
                if(Friction && Friction->IsValue()) r = *Friction;
                if(!Shape) MakePhysical(E,*Mass,r,f);
                else MakePhysicalWithShape(E,*Mass,r,f,(CString)*Shape);
            }
            if(GUI && GUI->IsValue()) E->SetGUIFlag(*GUI);
            
            if(Script)
            {
                if(Script->IsValue()) E->AddScript(string(*Script));
                else
                {
                    PNode* Filename = Script->Search("File");
                    PNode* Rate     = Script->Search("Rate");
                    
                    if(Filename) E->AddScript(string(*Filename),Rate ? *Rate : 0.0f);
                    else
                    {
                        Log("Invalid entity definition <%s>. (\"File\" not found under \"Script\")\n",Path.c_str());
                    }
                }
            }
        }
        E->AddRef();
        return E;
    }
    Entity* Engine::SpawnEntityPos(const string& Path,const Vector3& Pos)
    {
        if(Path.length() == 0) return 0;
        Entity* E = CreateEntity();
        E->m_Definition = (PropertyList*)m_AssetManager->LoadAsset(Path.c_str(),AT_PROPERTYLIST);
        if(E->m_Definition)
        {
            PNode* Base = E->m_Definition->Search("Entity");
            if(!Base)
            {
                Log("Unable to load entity definition. (No base \"<Entity>\" node found)\n");
                E->Destroy();
                return 0;
            }
            
            PNode* Script       = Base->Search("Script");
            PNode* Render       = Base->Search("Render");
            PNode* XForm        = Base->Search("Transformable");
            PNode* Mass         = Base->Search("Mass");
            PNode* Restitution  = Base->Search("Restitution");
            PNode* Friction     = Base->Search("Friction");
            PNode* IsObserver   = Base->Search("Observer");
            PNode* IsStatic     = Base->Search("IsStatic");
            PNode* Orientation  = Base->Search("Orientation");
            PNode* Dimensions   = Base->Search("Dimensions");
            PNode* Shape        = Base->Search("Shape");
            
            if(XForm)
            {
                if(XForm->IsValue()      && (bool)*XForm)
                {
                    MakeTransformable(E);
                    TransformComponent* t = (TransformComponent*)E->GetComponentByType(CT_TRANSFORM);
                    t->Translate(Pos);
                    if(Orientation && Orientation->IsValue()) { Vector4 r = *Orientation; t->Rotate(r.xyz(),r.w); }
                    if(Dimensions && Dimensions->IsValue()) { t->SetDimensions(*Dimensions); }
                }
            }
            if(IsObserver && IsObserver->IsValue() && (bool)*IsObserver) MakeObserver(E);
            if(IsStatic   && IsStatic  ->IsValue() && (bool)*IsStatic  ) ((TransformComponent*)E->GetComponentByType(CT_TRANSFORM))->m_IsStatic = true;
            if(Render) MakeRenderable(E);
            if(Mass && Mass->IsValue())
            {
                Scalar r = 0.2f,f = 0.4f;
                if(Restitution && Restitution->IsValue()) r = *Restitution;
                if(Friction && Friction->IsValue()) f = *Friction;
                if(!Shape) MakePhysical(E,*Mass,r,f);
                else MakePhysicalWithShape(E,*Mass,r,f,(CString)*Shape);
            }
            
            if(Script)
            {
                if(Script->IsValue()) E->AddScript(string(*Script));
                else
                {
                    PNode* Filename = Script->Search("File");
                    PNode* Rate     = Script->Search("Rate");
                    
                    if(Filename) E->AddScript(string(*Filename),Rate ? *Rate : 0.0f);
                    else
                    {
                        Log("Invalid entity definition <%s>. (\"File\" not found under \"Script\")\n",Path.c_str());
                    }
                }
            }
        }
        E->AddRef();
        return E;
    }

    void Engine::DestroyEntity(Entity *Target)
    {
        Broadcast(new DestroyEntityMessage(Target));
    }
    void Engine::Serialize() const
    {
        /*
         * Save the state of the engine
         */
    }
    void Engine::Deserialize()
    {
        /*
         * Load the state of the engine
         */
    }
    void Engine::Log(Literal Text,...)
    {
        if(m_Engine->m_LogToConsole)
        {
            va_list Args;
            va_start(Args,Text);
            printf("%f: ",m_Engine->m_RunTime.ElapsedTime());
            vprintf(Text,Args);
            va_end(Args);
        }
        if(m_Engine->m_LogToFile)
        {
            FILE* fp = fopen("EngineLog.txt","a");
            va_list Args;
            va_start(Args,Text);
            fprintf(fp,"%f: ",m_Engine->m_RunTime.ElapsedTime());
            vfprintf(fp,Text,Args);
            va_end(Args);
            fclose(fp);
        }
    }
    void Engine::Log(Literal Text,va_list& Args)
    {
        va_list tmp;
        if(m_Engine->m_LogToConsole)
        {
            //CString Str;
            //vasprintf(&Str,Text,Args);
            printf("%f: ",m_Engine->m_RunTime.ElapsedTime());
            if(m_Engine->m_LogToFile) va_copy(tmp,Args);
            vprintf(Text,Args);
            //free(Str);
        }
        if(m_Engine->m_LogToFile)
        {
            FILE* fp = fopen("EngineLog.txt","a");
            fprintf(fp,"%f: ",m_Engine->m_RunTime.ElapsedTime());
            vfprintf(fp,Text,tmp);
            va_end(tmp);
            fclose(fp);
        }
    }
    void Engine::ReloadConf()
    {
        PNode* SysSettings = m_Config->Search("System");
        if(SysSettings)
        {
            PNode* AutoReloadAssets = SysSettings->Search("Asset auto-reload");
            if(AutoReloadAssets)
            {
                PNode* Enabled = AutoReloadAssets->Search("Enabled");
                PNode* Interval = AutoReloadAssets->Search("Interval");
                if(Enabled) m_AssetManager->SetAutoAssetReloading(*Enabled);
                if(Interval) m_AssetManager->SetAssetCheckInterval(*Interval);
            }
            
            PNode* Framerate = SysSettings->Search("Framerate");
            if(Framerate)
            {
                m_Timestep = 1.0f / (Scalar)*Framerate;
            }
        }
        
        m_LogToConsole = GetGameVariableb("LogToConsole");
        m_LogToFile = GetGameVariableb("LogToFile");
    }
    void Engine::ReloadVars()
    {
        PNode* Vars = m_Config->Search("Variables");
        if(!Vars) return;
        m_Variables.Clear();
        i32 CNum = Vars->GetChildCount();
        for(i32 i = 0;i < CNum;i++)
        {
            PNode* Child = Vars->GetChild(i);
            if(!Child || !Child->IsValue()) continue;
            m_Variables.Insert(new PNode(*Child),Child->GetName());
        }
    }
};