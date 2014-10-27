/*
 *  ScriptSystem.cpp
 *
 *  Created by Michael DeCicco on 6/27/14.
 *
 */

#include "ScriptSystem.h"
#include "Engine.h"
#include "EntityManager.h"

#include "scriptstdstring.h"
#include "scriptmath.h"

#include <stdlib.h>

namespace Reality
{
    void MessageCallback(const asSMessageInfo *msg, void *param)
    {
      const char *type = "ERROR";
      if     (msg->type == asMSGTYPE_WARNING    ) type = "WARNING";
      else if(msg->type == asMSGTYPE_INFORMATION) type = "INFO"   ;
      if(*msg->section != 0) Log("AS: %s(%d,%d):%s: %s\n", msg->section, msg->row, msg->col, type, msg->message);
      else Log("AS: %s: %s\n",type, msg->message);
    }
    
    ScriptComponent::ScriptComponent() : Component(CT_SCRIPT), m_Module(0), m_InitFunc(0),
                                         m_EventFunc(0), m_UpdateFunc(0), m_DestroyFunc(0),
                                         m_LeftMouseDownFunc(0), m_LeftMouseUpFunc(0),
                                         m_RightMouseDownFunc(0), m_RightMouseUpFunc(0),
                                         m_CursorMoveFunc(0), m_KeyDownFunc(0),
                                         m_GamepadAttachedFunc(0), m_GamepadDetachedFunc(0),
                                         m_ButtonDownFunc(0), m_ButtonUpFunc(0),
                                         m_AxisMovedFunc(0)
    {
    }
    ScriptComponent::~ScriptComponent()
    {
        if(m_Objs.size() > 0)
        {
            for(i32 i = 0;i < m_Objs.size();i++)
            {
                while(m_Objs[i]->Release() != 0);
            }
        }
    }
    bool ScriptComponent::Reload()
    {
        asIScriptEngine* Eng = Engine::GetEngine()->GetScriptSystem()->GetEngine();
        static const char InitCode[] = "Entity@ self;void _Init(Entity@ E){@self = E;}";
    
        //Load the script before doing anything else, so that if there is an error we aren't left with no script at all
        //Load the script to a buffer large enough for the script and the init code
        CString Scpt = ReadAll();
        i32 cs = 0;
        for(i32 i = 0;Scpt[i] != 0;i++)
        {
            if(Scpt[i] == 'c' && !strncmp(&Scpt[i],"class",5) && cs == 0)
            {
                i += 5;
                cs = 1;
            }
            else if(Scpt[i] == '{' && cs == 1)
            {
                cs = 2;
                i++;
            }
            if(cs == 2)
            {
                string s;
                s.append(Scpt,i);
                s.append(InitCode,sizeof(InitCode) - 1);
                s.append(&Scpt[i],GetSize() - i);
                FreeCString(Scpt);
                Scpt = MakeCString(s.length() + 1);
                memcpy(Scpt,&s[0],s.length());
                Scpt[s.length()] = 0;
                break;
            }
        }
    
        if(!Scpt)
        {
            Engine::Log("Unable to instantiate entity, empty script <%s>.\n",m_Path.c_str());
            return false;
        }
        
        //Create module
        asIScriptModule* Mod = Eng->GetModule("_Temporary_",asGM_ALWAYS_CREATE);
        
        //Add the script sections to the module
        Mod->AddScriptSection(m_Path.c_str(),Scpt);
        
        //Build
        i32 r = Mod->Build();
        if(r < 0)
        {
            Mod->Discard();
            Engine::Log("Unable to build script.\n");
            FreeCString(Scpt);
            return false;
        }
        
        //Find class name
        i32 i = 0;
        i32 i0 = -1;
        i32 i1 = -1;
        while(Scpt[i] != 0)
        {
            if(Scpt[i + 0] == 'c'
            && Scpt[i + 1] == 'l'
            && Scpt[i + 2] == 'a'
            && Scpt[i + 3] == 's'
            && Scpt[i + 4] == 's')
            {
                i += 4;
                while(Scpt[i++] != 0)
                {
                    if(!isspace(Scpt[i]))
                    {
                        i0 = i;
                        while(Scpt[i] != 0)
                        {
                            if(isspace(Scpt[i])) { i1 = i; break; }
                            i++;
                        }
                        break;
                    }
                }
                break;
            }
            i++;
        }
        CString ClassName = MakeCString((i1 - i0) + 1);
        i32 c = 0;
        for(i32 i = 0;i < i1 - i0;i++,c++) ClassName[c] = Scpt[i0 + i];
        ClassName[c++] = 0;
        
        //Release memory used for script contents
        FreeCString(Scpt);
        
        //Get object type
        asIObjectType* Type = Eng->GetObjectTypeById(Mod->GetTypeIdByDecl(ClassName));
        FreeCString(ClassName);
        
        //Obtain the function objects that represent the class methods
        m_InitFunc              = Type->GetMethodByDecl("void Initialize(Entity@ E)");
        m_EventFunc             = Type->GetMethodByDecl("void OnEvent(const Event& in)");
        m_UpdateFunc            = Type->GetMethodByDecl("void Update(float dt)");
        m_DestroyFunc           = Type->GetMethodByDecl("void Destroy()");
        m_ClickDownFunc         = Type->GetMethodByDecl("void OnClickDown(const Vector2& in)");
        m_ClickUpFunc           = Type->GetMethodByDecl("void OnClickUp(const Vector2& in)");
        m_LeftMouseDownFunc     = Type->GetMethodByDecl("void OnLeftMouseDown(const Vector2& in)");
        m_LeftMouseUpFunc       = Type->GetMethodByDecl("void OnLeftMouseUp(const Vector2& in)");
        m_RightMouseDownFunc    = Type->GetMethodByDecl("void OnRightMouseDown(const Vector2& in)");
        m_RightMouseUpFunc      = Type->GetMethodByDecl("void OnRightMouseUp(const Vector2& in)");
        m_CursorMoveFunc        = Type->GetMethodByDecl("void OnCursorMove(const Vector2& in,const Vector2& in)");
        m_KeyDownFunc           = Type->GetMethodByDecl("void OnKeyDown(int Key)");
        m_KeyUpFunc             = Type->GetMethodByDecl("void OnKeyUp(int Key)");
        m_GamepadAttachedFunc   = Type->GetMethodByDecl("void OnGamepadAttached(uint DeviceID)");
        m_GamepadDetachedFunc   = Type->GetMethodByDecl("void OnGamepadDetached(uint DeviceID)");
        m_ButtonDownFunc        = Type->GetMethodByDecl("void OnButtonDown(uint DeviceID,uint ButtonID)");
        m_ButtonUpFunc          = Type->GetMethodByDecl("void OnButtonUp(uint DeviceID,uint ButtonID)");
        m_AxisMovedFunc         = Type->GetMethodByDecl("void OnAxisMoved(uint DeviceID,uint AxisID,Scalar Position,Scalar Delta)");
        
        //Get the factory function from the object type
        asIScriptFunction *Factory = Type->GetFactoryByIndex(0);
        
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        asIScriptContext* Ctx = Sys->GetContext();
        
        //Recreate all entity script objects
        for(i32 i = 0;i < m_Entities.size();i++)
        {
            //Prepare the context to call the factory function
            Ctx->Prepare(Factory);
            
            //Execute the call
            Ctx->Execute();
            
            //Get the object that was created
            asIScriptObject* Obj = *(asIScriptObject**)Ctx->GetAddressOfReturnValue();
            if(!Obj)
            {
                Log("Unable to execute script factory function <%s>.\n",m_Path.c_str());
                Mod->Discard();
                Ctx->Unprepare();
                return false;
            }
            Obj->AddRef();
            
            //Unprepare
            Ctx->Unprepare();
            
            asIScriptFunction* Init = Type->GetMethodByDecl("void _Init(Entity@ E)");
            Ctx->Prepare(Init);
            Ctx->SetObject(Obj);
            Ctx->SetArgObject(0,m_Entities[i]);
            Ctx->Execute();
            Ctx->Unprepare();
            Sys->ReleaseContext(Ctx);
            
            m_Objs[i]->Release();
            m_Objs[i] = Obj;
            
        }
        if(m_Module) m_Module->Discard();
        Mod->SetName(GetPath().c_str());
        m_Module = Mod;
        
        //Initialize all entities
        Sys->InitializeEntity(this);
    
        return true;
    }

    ScriptSystem::ScriptSystem()
    {
    }
    ScriptSystem::~ScriptSystem()
    {
    }

    void ScriptSystem::HandleMessage(const EngineMessage* Msg)
    {
        switch(Msg->m_MessageType)
        {
            case MT_DESTROY_ENTITY:
            {
                DestroyEntityMessage* dMsg = (DestroyEntityMessage*)Msg;
                ScriptComponent* Scpt = (ScriptComponent*)dMsg->m_Entity->GetComponentByType(CT_SCRIPT);
                if(!Scpt) break;
                DestroyEntity(Scpt,dMsg->m_Entity);
                Engine::GetEngine()->RemoveComponent(dMsg->m_Entity,Scpt);
                break;
            }
            case MT_USER_EVENT:
            {
                for(i32 i = 0;i < m_Components.size();i++)
                {
                    SendEventToEntity((ScriptComponent*)m_Components[i],((UserEventMessage*)Msg)->m_Event,0);
                }
                break;
            }
            case MT_TARGETED_USER_EVENT:
            {
                TargetedUserEventMessage* tMsg = (TargetedUserEventMessage*)Msg;
                SendEventToEntity((ScriptComponent*)tMsg->m_TargetEntity->GetComponentByType(CT_SCRIPT),tMsg->m_Event,tMsg->m_TargetEntity);
                break;
            }
            case MT_ADD_SCRIPT:
            {
                AddScriptMessage* sMsg = (AddScriptMessage*)Msg;
                i32 idx = -1;
                ScriptComponent* Scpt = InstantiateEntity(sMsg->m_Entity,sMsg->m_ScriptPath.c_str(),&idx);
                if(!Scpt)
                {
                    Log("Unable to load script <%s> for entity <%d>\n",sMsg->m_ScriptPath.c_str(),sMsg->m_Entity->GetID());
                    return;
                }
                Scpt->m_InvUpdateRates[idx] = Scalar(1) / sMsg->m_UpdateRate;
                Engine::GetEngine()->AddComponent(sMsg->m_Entity,Scpt);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    void ScriptSystem::Initialize()
    {
        m_Engine  = asCreateScriptEngine(ANGELSCRIPT_VERSION);
        m_Engine->SetMessageCallback(asFUNCTION(MessageCallback),0,asCALL_CDECL);
        m_Engine->SetEngineProperty(asEP_USE_CHARACTER_LITERALS,true);
        m_Engine->RegisterTypedef("char","uint8");
        
        RegisterScriptMath(m_Engine);
        RegisterStdString(m_Engine);
        
        if(!Engine::GetEngine()->VarExists("MaxScriptContexts"))
        {
            Log("Fatal error: \"MaxScriptContexts\" variable (integer) must exist in Config.plst. See engine documentation for more information.\n");
            exit(-1);
        }
        m_ContextCount = Engine::GetEngine()->GetGameVariablei("MaxScriptContexts");
        if(m_ContextCount <= 0)
        {
            Log("Fatal error: \"MaxScriptContexts\" variable in Config.plst must be greater than 0. See engine documentation for more information.\n");
            exit(-1);
        }
        
        m_ContextPool = new asIScriptContext*[m_ContextCount];
        m_ContextInUse = new bool[m_ContextCount];
        for(i32 i = 0;i < m_ContextCount;i++)
        {
            m_ContextPool[i] = m_Engine->CreateContext();
            m_ContextInUse[i] = false;
        }
    }
    void ScriptSystem::Update(Scalar dt)
    {
        /* Update input */
        Engine::GetEngine()->GetInputManager()->Update();
        
        /* Update scripts */
        for(i32 i = 0;i < m_Components.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_Components[i];
            
            for(i32 i = 0;i < Scpt->m_Entities.size();i++)
            {
                PropertyList* Def = Scpt->m_Entities[i]->GetDef();
                if(Def && Def->HasUpdated())
                {
                    PNode* Base = Def->Search("Entity");
                    if(Base)
                    {
                        PNode* ScptNode = Base->Search("Script");
                        if(ScptNode->IsValue())
                        {
                            Engine::GetEngine()->RemoveComponent(Scpt->m_Entities[i],Scpt);
                            Scpt->m_Entities[i]->AddScript(string(*ScptNode));
                        }
                        else
                        {
                            PNode* Filename = ScptNode->Search("File");
                            PNode* Rate     = ScptNode->Search("Rate");
                            
                            if(Filename)
                            {
                                if(Scpt->GetPath() != (CString)*Filename)
                                {
                                    Engine::GetEngine()->RemoveComponent(Scpt->m_Entities[i],Scpt);
                                    Scpt->m_Entities[i]->AddScript(string(*Filename),Rate ? (Scalar)*Rate : 0.0f);
                                }
                                else Scpt->m_InvUpdateRates[i] = Rate ? Scalar(1) / (Scalar)*Rate : 0.0f;
                            }
                            else
                            {
                                Log("Invalid entity definition <%s>. (\"File\" not found under \"Script\")\n",Def->GetPath().c_str());
                            }
                        }
                    }
                    Def->ResetUpdateFlag();
                }
            }
            UpdateEntity(Scpt,dt);
        }
    }
    void ScriptSystem::Shutdown()
    {
        //Destroy script objects and such
        for(i32 i = 0;i < m_Components.size();i++)
        {
            ScriptComponent* c = (ScriptComponent*)m_Components[i];
            DestroyEntity(c,0);
            
            //Destroy the script asset until it can't be destroyed anymore
            while(c->Asset::Destroy() != 0);
        }
        
        for(i32 i = 0;i < m_ContextCount;i++) m_ContextPool[i]->Release();
        delete [] m_ContextPool;
        delete [] m_ContextInUse;
        m_Engine->Release();
    }

    void ScriptSystem::Serialize()
    {
    }
    void ScriptSystem::Deserialize()
    {
    }

    /* Execute script from file */
    i32 ScriptSystem::RunScriptFromFile(Literal Path)
    {
        //Create a new script module
        asIScriptModule *tMod = m_Engine->GetModule(Path,asGM_ALWAYS_CREATE);
        
        //Load the script
        CString Scpt = Engine::GetEngine()->GetFileManager()->GetContents(Path);
        if(!Scpt)
        {
            tMod->Discard();
            Engine::Log("Unable to build script <%s>.\n",Path);
            return -1;
        }
        
        //Add the script sections to the module
        tMod->AddScriptSection(Path,Scpt);
        
        //Free the script
        FreeCString(Scpt);
        
        //Build
        i32 r = tMod->Build();
        if(r < 0)
        {
            tMod->Discard();
            Engine::Log("Unable to build script <%s>.\n",Path);
            return -1;
        }
        
        asIScriptFunction *Func = tMod->GetFunctionByDecl("int main()");
        if(!Func)
        {
            tMod->Discard();
            Engine::Log("Unable to locate function \"int main()\" in script <%s>.\n",Path);
            return -1;
        }
        
        //Prepare() must be called to allow the context to prepare the stack
        asIScriptContext* Ctx = GetContext();
        Ctx->Prepare(Func);
        
        r = Ctx->Execute();
        if(r == asEXECUTION_FINISHED)
        {
            //The return value is only valid if the execution finished successfully
            r = Ctx->GetReturnDWord();
        }
        
        //Prepare for next preparation
        Ctx->Unprepare();
        ReleaseContext(Ctx);
        
        //Discard the module
        tMod->Discard();
        
        return r;
    }

    /* Execute script from memory */
    i32 ScriptSystem::RunScript(CString Code)
    {
        //Create a new script module
        asIScriptModule *tMod = m_Engine->GetModule("mod",asGM_ALWAYS_CREATE);
        if(!Code)
        {
            tMod->Discard();
            Engine::Log("Unable to build script\n");
            return -1;
        }
        
        //Add the script sections to the module
        tMod->AddScriptSection("main",Code);
        
        //Build
        i32 r = tMod->Build();
        if(r < 0)
        {
            tMod->Discard();
            Engine::Log("Unable to build script.\n");
            return -1;
        }
        
        asIScriptFunction *Func = tMod->GetFunctionByDecl("int main()");
        if(!Func)
        {
            tMod->Discard();
            Engine::Log("Unable to locate function \"int main()\" in script.\n");
            return -1;
        }
        
        //Prepare() must be called to allow the context to prepare the stack
        asIScriptContext* Ctx = GetContext();
        Ctx->Prepare(Func);
        
        r = Ctx->Execute();
        if(r == asEXECUTION_FINISHED )
        {
            //The return value is only valid if the execution finished successfully
            r = Ctx->GetReturnDWord();
        }
        
        //Discard the module
        tMod->Discard();
        
        //Prepare for next preparation
        Ctx->Unprepare();
        ReleaseContext(Ctx);
        
        return r;
    }

    /* Instantiate entity */
    ScriptComponent* ScriptSystem::InstantiateEntity(Entity* E,Literal Path,i32* Idx)
    {
        ScriptComponent* Ret = (ScriptComponent*)Engine::GetEngine()->GetAssetManager()->LoadAsset(Path,AT_SCRIPT);
        
        if(!Ret) return 0;
        
        //Get the object type
        Ret->m_Module = m_Engine->GetModule(Path,asGM_ALWAYS_CREATE);
        
        static const char InitCode[] = "Entity@ self;void _Init(Entity@ E){@self = E;}";
        
        //Load the script to a buffer large enough for the script and the init code
        CString Scpt = Ret->ReadAll();
        i32 cs = 0;
        for(i32 i = 0;Scpt[i] != 0;i++)
        {
            if(Scpt[i] == 'c' && !strncmp(&Scpt[i],"class",5) && cs == 0)
            {
                i += 5;
                cs = 1;
            }
            else if(Scpt[i] == '{' && cs == 1)
            {
                cs = 2;
                i++;
            }
            if(cs == 2)
            {
                string s;
                s.append(Scpt,i);
                s.append(InitCode,sizeof(InitCode) - 1);
                s.append(&Scpt[i],Ret->GetSize() - i);
                FreeCString(Scpt);
                Scpt = MakeCString(s.length() + 1);
                memcpy(Scpt,&s[0],s.length());
                Scpt[s.length()] = 0;
                break;
            }
        }
        
        if(!Scpt)
        {
            Ret->m_Module->Discard();
            Engine::Log("Unable to instantiate entity, empty script <%s>.\n",Path);
            delete Ret;
            return 0;
        }
        
        
        
        //Add the script sections to the module
        Ret->m_Module->AddScriptSection(Path,Scpt);
        
        //Build
        i32 r = Ret->m_Module->Build();
        if(r < 0)
        {
            Ret->m_Module->Discard();
            delete Ret;
            Engine::Log("Unable to build script.\n");
            FreeCString(Scpt);
            return 0;
        }
        
        //Find class name
        i32 i = 0;
        i32 i0 = -1;
        i32 i1 = -1;
        while(Scpt[i] != 0)
        {
            if(Scpt[i + 0] == 'c'
            && Scpt[i + 1] == 'l'
            && Scpt[i + 2] == 'a'
            && Scpt[i + 3] == 's'
            && Scpt[i + 4] == 's')
            {
                i += 4;
                while(Scpt[i++] != 0)
                {
                    if(!isspace(Scpt[i]))
                    {
                        i0 = i;
                        while(Scpt[i] != 0)
                        {
                            if(isspace(Scpt[i])) { i1 = i; break; }
                            i++;
                        }
                        break;
                    }
                }
                break;
            }
            i++;
        }
        CString ClassName = MakeCString((i1 - i0) + 1);
        i32 c = 0;
        for(i32 i = 0;i < i1 - i0;i++,c++) ClassName[c] = Scpt[i0 + i];
        ClassName[c++] = 0;
        
        //Release memory used for script contents
        FreeCString(Scpt);
        
        //Get the class type
        asIObjectType* Type = m_Engine->GetObjectTypeById(Ret->m_Module->GetTypeIdByDecl(ClassName));
        FreeCString(ClassName);
        
        //Get the factory function from the object type
        asIScriptFunction *Factory = Type->GetFactoryByIndex(0);
        
        //Prepare the context to call the factory function
        asIScriptContext* Ctx = GetContext();
        Ctx->Prepare(Factory);
        
        //Execute the call
        Ctx->Execute();
        
        //Get the object that was created
        asIScriptObject* Obj = *(asIScriptObject**)Ctx->GetAddressOfReturnValue();
        Obj->AddRef();
        
        //Unprepare
        Ctx->Unprepare();
        
        //Obtain the function objects that represent the class methods
        Ret->m_InitFunc             = Type->GetMethodByDecl("void Initialize()");
        Ret->m_EventFunc            = Type->GetMethodByDecl("void OnEvent(const Event& in)");
        Ret->m_UpdateFunc           = Type->GetMethodByDecl("void Update(float dt)");
        Ret->m_DestroyFunc          = Type->GetMethodByDecl("void Destroy()");
        Ret->m_ClickDownFunc        = Type->GetMethodByDecl("void OnClickDown(const Vector2& in)");
        Ret->m_ClickUpFunc          = Type->GetMethodByDecl("void OnClickUp(const Vector2& in)");
        Ret->m_LeftMouseDownFunc    = Type->GetMethodByDecl("void OnLeftMouseDown(const Vector2& in)");
        Ret->m_LeftMouseUpFunc      = Type->GetMethodByDecl("void OnLeftMouseUp(const Vector2& in)");
        Ret->m_RightMouseDownFunc   = Type->GetMethodByDecl("void OnRightMouseDown(const Vector2& in)");
        Ret->m_RightMouseUpFunc     = Type->GetMethodByDecl("void OnRightMouseUp(const Vector2& in)");
        Ret->m_CursorMoveFunc       = Type->GetMethodByDecl("void OnCursorMove(const Vector2& in,const Vector2& in)");
        Ret->m_KeyDownFunc          = Type->GetMethodByDecl("void OnKeyDown(int Key)");
        Ret->m_KeyUpFunc            = Type->GetMethodByDecl("void OnKeyUp(int Key)");
        Ret->m_GamepadAttachedFunc  = Type->GetMethodByDecl("void OnGamepadAttached(uint DeviceID)");
        Ret->m_GamepadDetachedFunc  = Type->GetMethodByDecl("void OnGamepadDetached(uint DeviceID)");
        Ret->m_ButtonDownFunc       = Type->GetMethodByDecl("void OnButtonDown(uint DeviceID,uint ButtonID)");
        Ret->m_ButtonUpFunc         = Type->GetMethodByDecl("void OnButtonUp(uint DeviceID,uint ButtonID)");
        Ret->m_AxisMovedFunc        = Type->GetMethodByDecl("void OnAxisMoved(uint DeviceID,uint AxisID,Scalar Position,Scalar Delta)");
        
        //call _Init
        asIScriptFunction* Init = Type->GetMethodByDecl("void _Init(Entity@ E)");
        Ctx->Prepare(Init);
        Ctx->SetObject(Obj);
        Ctx->SetArgObject(0,E);
        Ctx->Execute();
        Ctx->Unprepare();
        ReleaseContext(Ctx);
        
        *Idx = (i32)Ret->m_Entities.size();
        Ret->m_EntIndices[E] = (i32)Ret->m_Entities.size();
        Ret->m_Entities.push_back(E);
        Ret->m_Objs.push_back(Obj);
        Ret->m_Tms.push_back(Timer());
        Ret->m_InvUpdateRates.push_back(1.0f);
        
        InitializeEntity(Ret,*Idx);
        return Ret;
    }
    void ScriptSystem::InitializeEntity(ScriptComponent* EScript,i32 Idx)
    {
        if(Idx != -1)
        {
            //Start the rate timer
            EScript->m_Tms[Idx].Start();
            
            if(!EScript->m_InitFunc) return;
            
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_InitFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[Idx]);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
        else
        {
            for(i32 i = 0;i < EScript->m_Objs.size();i++)
            {
                //Start the rate timer
                EScript->m_Tms[i].Start();
                
                if(!EScript->m_InitFunc) return;
                
                //Prepare the context for calling the method
                asIScriptContext* Ctx = GetContext();
                Ctx->Prepare(EScript->m_InitFunc);
                
                //Set the object pointer
                Ctx->SetObject(EScript->m_Objs[i]);
                
                //Execute the call
                Ctx->Execute();
                
                //Unprepare
                Ctx->Unprepare();
                ReleaseContext(Ctx);
            }
        }
    }
    void ScriptSystem::SendEventToEntity(ScriptComponent* EScript,const Event& Evt,Entity* E)
    {
        if(!EScript->m_EventFunc) return;
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_EventFunc);
            
            //Set the object pointer
            i32 Idx = EScript->m_EntIndices[E];
            Ctx->SetObject(EScript->m_Objs[Idx]);
            
            //Set argument
            Ctx->SetArgAddress(0,(void*)&Evt);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
        else
        {
            for(i32 i = 0;i < EScript->m_Objs.size();i++)
            {
                //Prepare the context for calling the method
                asIScriptContext* Ctx = GetContext();
                Ctx->Prepare(EScript->m_EventFunc);
                
                //Set the object pointer
                Ctx->SetObject(EScript->m_Objs[i]);
                
                //Set argument
                Ctx->SetArgObject(0,(void*)&Evt);
                
                //Execute the call
                Ctx->Execute();
                
                //Unprepare
                Ctx->Unprepare();
                ReleaseContext(Ctx);
            }
        }
    }
    void ScriptSystem::UpdateEntity(ScriptComponent* EScript,Scalar dt)
    {
        if(!EScript->m_UpdateFunc) return;
        for(i32 i = 0;i < EScript->m_Entities.size();i++)
        {
            Scalar _dt = dt;
            if(EScript->m_InvUpdateRates[i] != Scalar(0))
            {
                if(EScript->m_Tms[i] > EScript->m_InvUpdateRates[i])
                {
                    _dt = EScript->m_Tms[i];
                    EScript->m_Tms[i].Stop();
                    EScript->m_Tms[i].Start();
                }
                else continue;
            }
            
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_UpdateFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[i]);
            
            //Set argument
            #ifndef USE_DOUBLE_SCALAR
            Ctx->SetArgFloat(0,_dt);
            #else
            Ctx->SetArgDouble(0,_dt);
            #endif
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::DestroyEntity(ScriptComponent* EScript,Entity* E)
    {
        if(!EScript->m_DestroyFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_DestroyFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
        else
        {
            for(i32 i = 0;i < EScript->m_Entities.size();i++)
            {
                //Prepare the context for calling the method
                asIScriptContext* Ctx = GetContext();
                Ctx->Prepare(EScript->m_DestroyFunc);
                
                //Set the object pointer
                Ctx->SetObject(EScript->m_Objs[i]);
                
                //Execute the call
                Ctx->Execute();
                
                //Unprepare
                Ctx->Unprepare();
                ReleaseContext(Ctx);
            }
        }
    }
    void ScriptSystem::OnClickDown(ScriptComponent* EScript,Entity* E,const Vector2& Pos)
    {
        if(!EScript->m_ClickDownFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_ClickDownFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgObject(0,(void*)&Pos);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnClickUp(ScriptComponent* EScript,Entity* E,const Vector2& Pos)
    {
        if(!EScript->m_ClickUpFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_ClickUpFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgObject(0,(void*)&Pos);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnLeftMouseDown(ScriptComponent* EScript,Entity* E,const Vector2& Pos)
    {
        if(!EScript->m_LeftMouseDownFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_LeftMouseDownFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgObject(0,(void*)&Pos);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnLeftMouseUp(ScriptComponent* EScript,Entity* E,const Vector2& Pos)
    {
        if(!EScript->m_LeftMouseUpFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_LeftMouseUpFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgObject(0,(void*)&Pos);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnRightMouseDown(ScriptComponent* EScript,Entity* E,const Vector2& Pos)
    {
        if(!EScript->m_RightMouseUpFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_RightMouseDownFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgObject(0,(void*)&Pos);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnRightMouseUp(ScriptComponent* EScript,Entity* E,const Vector2& Pos)
    {
        if(!EScript->m_RightMouseUpFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_RightMouseUpFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgObject(0,(void*)&Pos);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnCursorMove(ScriptComponent* EScript,Entity* E,const Vector2& Pos,const Vector2& Delta)
    {
        if(!EScript->m_CursorMoveFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_CursorMoveFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgObject(0,(void*)&Pos);
            Ctx->SetArgObject(1,(void*)&Delta);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnKeyDown(ScriptComponent* EScript,Entity* E,char Key)
    {
        if(!EScript->m_KeyDownFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_KeyDownFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgDWord(0,(i32)Key);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnKeyUp(ScriptComponent* EScript,Entity* E,char Key)
    {
        if(!EScript->m_KeyUpFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_KeyUpFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgDWord(0,(i32)Key);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnGamepadAttached(ScriptComponent* EScript,Entity* E,u32 DeviceID)
    {
        if(!EScript->m_GamepadAttachedFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_GamepadAttachedFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgDWord(0,DeviceID);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnGamepadDetached(ScriptComponent* EScript,Entity* E,u32 DeviceID)
    {
        if(!EScript->m_GamepadDetachedFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_GamepadDetachedFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgDWord(0,DeviceID);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnButtonDown(ScriptComponent* EScript,Entity* E,u32 DeviceID,u32 ButtonID)
    {
        if(!EScript->m_ButtonDownFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_ButtonDownFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgDWord(0,DeviceID);
            Ctx->SetArgDWord(1,ButtonID);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnButtonUp(ScriptComponent* EScript,Entity* E,u32 DeviceID,u32 ButtonID)
    {
        if(!EScript->m_ButtonUpFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_ButtonUpFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgDWord(0,DeviceID);
            Ctx->SetArgDWord(1,ButtonID);
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::OnAxisMoved(ScriptComponent* EScript,Entity* E,u32 DeviceID,u32 AxisID,Scalar Position,Scalar Delta)
    {
        if(!EScript->m_AxisMovedFunc) return;
        
        if(E)
        {
            //Prepare the context for calling the method
            asIScriptContext* Ctx = GetContext();
            Ctx->Prepare(EScript->m_AxisMovedFunc);
            
            //Set the object pointer
            Ctx->SetObject(EScript->m_Objs[EScript->m_EntIndices[E]]);
            Ctx->SetArgDWord(0,DeviceID);
            Ctx->SetArgDWord(1,AxisID);
            #ifdef USE_DOUBLE_SCALAR
            Ctx->SetArgDouble(0,Position);
            Ctx->SetArgDouble(1,Delta);
            #else
            Ctx->SetArgFloat(2,Position);
            Ctx->SetArgFloat(3,Delta);
            #endif
            
            //Execute the call
            Ctx->Execute();
            
            //Unprepare
            Ctx->Unprepare();
            ReleaseContext(Ctx);
        }
    }
    void ScriptSystem::RegisterClass(Literal Classname,i32 ByteSize,asDWORD Flags)
    {
        m_Engine->RegisterObjectType(Classname,ByteSize,Flags);
    }
    void ScriptSystem::RegisterClassMethod(Literal Classname,Literal Methoddecl,const asSFuncPtr& Func,asDWORD Conv)
    {
        m_Engine->RegisterObjectMethod(Classname,Methoddecl,Func,Conv);
    }
    void ScriptSystem::RegisterClassProperty(Literal Classname,Literal Propdecl,i32 ByteOffset)
    {
        m_Engine->RegisterObjectProperty(Classname,Propdecl,ByteOffset);
    }
    void ScriptSystem::RegisterClassBehavior(Literal Classname,asEBehaviours Behaviour,Literal Decl,const asSFuncPtr &funcPointer,asDWORD Conv)
    {
        m_Engine->RegisterObjectBehaviour(Classname,Behaviour,Decl,funcPointer,Conv);
    }
    void ScriptSystem::RegisterInterface(Literal Classname)
    {
        m_Engine->RegisterInterface(Classname);
    }
    void ScriptSystem::RegisterInterfaceMethod(Literal Classname,Literal Methoddecl)
    {
        m_Engine->RegisterInterfaceMethod(Classname,Methoddecl);
    }
    void ScriptSystem::RegisterEnum(Literal Enumname)
    {
        m_Engine->RegisterEnum(Enumname);
    }
    void ScriptSystem::RegisterEnumValue(Literal Enumname,Literal Valuename,i32 Value)
    {
        m_Engine->RegisterEnumValue(Enumname,Valuename,Value);
    }
    void ScriptSystem::RegisterVariable(Literal Vardecl,void* Pointer)
    {
        m_Engine->RegisterGlobalProperty(Vardecl,Pointer);
    }
    void ScriptSystem::RegisterFunction(Literal Funcdecl,const asSFuncPtr& Func)
    {
        m_Engine->RegisterGlobalFunction(Funcdecl,Func,asCALL_CDECL);
    }
    void ScriptSystem::RegisterTypedef(Literal Type,Literal Decl)
    {
        m_Engine->RegisterTypedef(Type,Decl);
    }
    asIScriptContext* ScriptSystem::GetContext()
    {
        for(i32 i = 0;i < m_ContextCount;i++)
        {
            if(!m_ContextInUse[i])
            {
                m_ContextInUse[i] = true;
                return m_ContextPool[i];
            }
        }
        Log("No script contexts available to run script. Consider raising \"MaxScriptContexts\" in Config.plst to resolve this issue.\n");
        return 0;
    }
    void ScriptSystem::ReleaseContext(asIScriptContext *Ctx)
    {
        for(i32 i = 0;i < m_ContextCount;i++)
        {
            if(m_ContextPool[i] == Ctx)
            {
                m_ContextInUse[i] = false;
                return;
            }
        }
        Log("Couldn't find context in context pool. Something is very wrong in this picture.\n");
    }
};