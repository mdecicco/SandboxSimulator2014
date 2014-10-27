/*
 *  ScriptSystem.h
 *
 *  Created by Michael DeCicco on 6/27/14.
 *
 */

#pragma once
#include "EngineSystem.h"
#include "AssetManager.h"
#include "Event.h"
#include "Timer.h"

#include "angelscript.h"

namespace Reality
{
    class Entity;
    class ScriptComponent : public Component, public Asset
    {
        public:
            ScriptComponent();
            ~ScriptComponent();
        
            virtual bool Reload();
        
            asIScriptModule* m_Module;
            asIScriptFunction* m_InitFunc;
            asIScriptFunction* m_EventFunc;
            asIScriptFunction* m_UpdateFunc;
            asIScriptFunction* m_DestroyFunc;
            asIScriptFunction* m_ClickDownFunc;
            asIScriptFunction* m_ClickUpFunc;
            asIScriptFunction* m_LeftMouseDownFunc;
            asIScriptFunction* m_LeftMouseUpFunc;
            asIScriptFunction* m_RightMouseDownFunc;
            asIScriptFunction* m_RightMouseUpFunc;
            asIScriptFunction* m_CursorMoveFunc;
            asIScriptFunction* m_KeyDownFunc;
            asIScriptFunction* m_KeyUpFunc;
            asIScriptFunction* m_GamepadAttachedFunc;
            asIScriptFunction* m_GamepadDetachedFunc;
            asIScriptFunction* m_ButtonDownFunc;
            asIScriptFunction* m_ButtonUpFunc;
            asIScriptFunction* m_AxisMovedFunc;
        
            vector<Entity*> m_Entities;
            vector<asIScriptObject*> m_Objs;
            vector<Timer> m_Tms;
            vector<Scalar> m_InvUpdateRates;
        
            unordered_map<Entity*,i32> m_EntIndices;
    };
    
    class Entity;
    class ScriptSystem : public EngineSystem
    {
        public:
            ScriptSystem();
            ~ScriptSystem();
            
            virtual void HandleMessage(const EngineMessage* Msg);
        
            virtual void Initialize();
            virtual void Update(Scalar dt);
            virtual void Shutdown();
        
            virtual void Serialize();
            virtual void Deserialize();
        
            /* Execute script from file */
            i32 RunScriptFromFile(Literal Path);
        
            /* Execute script from memory */
            i32 RunScript(CString Code);
        
            ScriptComponent* InstantiateEntity(Entity* E,Literal Path,i32* idx);
        
            void InitializeEntity(ScriptComponent* EScript,i32 Idx = -1);
            void SendEventToEntity(ScriptComponent* EScript,const Event& Evt,Entity* E);
            void UpdateEntity(ScriptComponent* EScript,Scalar dt);
            void DestroyEntity(ScriptComponent* EScript,Entity* E);
        
            /* GUI Elements */
            void OnClickDown(ScriptComponent* EScript,Entity* E,const Vector2& Pos);
            void OnClickUp(ScriptComponent* EScript,Entity* E,const Vector2& Pos);
        
            /* Input */
            void OnLeftMouseDown(ScriptComponent* EScript,Entity* E,const Vector2& Pos);
            void OnLeftMouseUp(ScriptComponent* EScript,Entity* E,const Vector2& Pos);
            void OnRightMouseDown(ScriptComponent* EScript,Entity* E,const Vector2& Pos);
            void OnRightMouseUp(ScriptComponent* EScript,Entity* E,const Vector2& Pos);
            void OnCursorMove(ScriptComponent* EScript,Entity* E,const Vector2& Pos,const Vector2& Delta);
            void OnKeyDown(ScriptComponent* EScript,Entity* E,char Key);
            void OnKeyUp(ScriptComponent* EScript,Entity* E,char Key);
            void OnGamepadAttached(ScriptComponent* EScript,Entity* E,u32 DeviceID);
            void OnGamepadDetached(ScriptComponent* EScript,Entity* E,u32 DeviceID);
            void OnButtonDown(ScriptComponent* EScript,Entity* E,u32 DeviceID,u32 ButtonID);
            void OnButtonUp(ScriptComponent* EScript,Entity* E,u32 DeviceID,u32 ButtonID);
            void OnAxisMoved(ScriptComponent* EScript,Entity* E,u32 DeviceID,u32 AxisID,Scalar Position,Scalar Delta);
        
            /* Interface registration */
            void RegisterClass(Literal Classname,i32 ByteSize,asDWORD Flags);
            void RegisterClassMethod(Literal Classname,Literal Methoddecl,const asSFuncPtr& Func,asDWORD Conv = asCALL_THISCALL);
            void RegisterClassProperty(Literal Classname,Literal Propdecl,i32 ByteOffset);
            void RegisterClassBehavior(Literal Classname,asEBehaviours Behaviour,Literal Decl,const asSFuncPtr &Func,asDWORD Conv);
            void RegisterInterface(Literal Classname);
            void RegisterInterfaceMethod(Literal Classname,Literal Methoddecl);
            void RegisterEnum(Literal Enumname);
            void RegisterEnumValue(Literal Enumname,Literal Valuename,i32 Value);
            void RegisterVariable(Literal Vardecl,void* Pointer);
            void RegisterFunction(Literal Funcdecl,const asSFuncPtr& Func);
            void RegisterTypedef(Literal Type,Literal Decl);
        
            asIScriptEngine* GetEngine() const { return m_Engine; }
            asIScriptContext* GetContext();
            void ReleaseContext(asIScriptContext* Ctx);
        
        protected:
            asIScriptEngine* m_Engine;
        
            asIScriptContext** m_ContextPool;
            bool* m_ContextInUse;
            i32 m_ContextCount;
    };
    
};