/*
 *  InputManager.cpp
 *
 *  Created by Michael DeCicco on 9/7/14.
 *
 */

#include "Engine.h"
#include "RenderSystem.h"

namespace Reality
{
    void KeyCallback(GLFWwindow* Window,i32 Key,i32 Scancode,i32 Action,i32 Mods)
    {
        if(Action == GLFW_PRESS) Engine::GetEngine()->GetInputManager()->OnKeyDown(Key);
        else if(Action == GLFW_RELEASE) Engine::GetEngine()->GetInputManager()->OnKeyUp(Key);
    }
    void MouseButtonCallback(GLFWwindow* Window,i32 ButtonID,i32 Action,i32 Mods)
    {
        if(ButtonID == 0)
        {
            if(Action == GLFW_PRESS) Engine::GetEngine()->GetInputManager()->OnLeftMouseDown();
            else if(Action == GLFW_RELEASE) Engine::GetEngine()->GetInputManager()->OnLeftMouseUp();
        }
        else if(ButtonID == 1)
        {
            if(Action == GLFW_PRESS) Engine::GetEngine()->GetInputManager()->OnRightMouseDown();
            else if(Action == GLFW_RELEASE) Engine::GetEngine()->GetInputManager()->OnRightMouseUp();
        }
    }
    void CursorPosCallback(GLFWwindow* Window,f64 x,f64 y)
    {
        Engine::GetEngine()->GetInputManager()->OnCursorMove(x,y);
    }
    
    void onButtonDown    (struct Gamepad_device* Device,u32 ButtonID,f64 Timestamp,void* Context)
    {
        ((InputManager*)Context)->OnButtonDown(Device->deviceID,ButtonID);
    }
    void onButtonUp      (struct Gamepad_device* Device,u32 ButtonID,f64 Timestamp,void* Context)
    {
        ((InputManager*)Context)->OnButtonUp(Device->deviceID,ButtonID);
    }
    void onAxisMoved     (struct Gamepad_device* Device,u32 AxisID,Scalar Value,Scalar LastValue,f64 Timestamp,void* Context)
    {
        ((InputManager*)Context)->OnAxisMoved(Device->deviceID,AxisID,Value,Value - LastValue);
    }
    void onDeviceAttached(struct Gamepad_device* Device,void* Context)
    {
        ((InputManager*)Context)->OnGamepadAttached(Device);
    }
    void onDeviceRemoved (struct Gamepad_device* Device,void* Context)
    {
        ((InputManager*)Context)->OnGamepadDetached(Device);
    }
    
    InputManager::InputManager()
    {
        Gamepad_deviceAttachFunc(onDeviceAttached,this);
        Gamepad_deviceRemoveFunc(onDeviceRemoved ,this);
        Gamepad_buttonDownFunc  (onButtonDown    ,this);
        Gamepad_buttonUpFunc    (onButtonUp      ,this);
        Gamepad_axisMoveFunc    (onAxisMoved     ,this);
        Gamepad_init();
    }
    InputManager::~InputManager()
    {
        Gamepad_shutdown();
    }
    void InputManager::Init()
    {
        f64 x,y;
        glfwGetCursorPos(Engine::GetEngine()->GetRenderSystem()->GetWindow(),&x,&y);
        m_CursorPos = Vector2(x,y);
    }
    void InputManager::OnLeftMouseDown()
    {
        Engine::GetEngine()->GetRenderSystem()->GetGUIManager()->OnMouseLeftDown(m_CursorPos);
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnLeftMouseDown(Scpt,m_ControlledEntities[i],m_CursorPos);
        }
    }
    void InputManager::OnLeftMouseUp()
    {
        Engine::GetEngine()->GetRenderSystem()->GetGUIManager()->OnMouseLeftUp(m_CursorPos);
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnLeftMouseUp(Scpt,m_ControlledEntities[i],m_CursorPos);
        }
    }
    void InputManager::OnRightMouseDown()
    {
        Engine::GetEngine()->GetRenderSystem()->GetGUIManager()->OnMouseRightDown(m_CursorPos);
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnRightMouseDown(Scpt,m_ControlledEntities[i],m_CursorPos);
        }
    }
    void InputManager::OnRightMouseUp()
    {
        Engine::GetEngine()->GetRenderSystem()->GetGUIManager()->OnMouseRightUp(m_CursorPos);
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnRightMouseUp(Scpt,m_ControlledEntities[i],m_CursorPos);
        }
    }
    void InputManager::OnCursorMove(f32 x,f32 y)
    {
        Vector2 Delta = Vector2(x,y) - m_CursorPos;
        m_CursorPos = Vector2(x,y);
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnCursorMove(Scpt,m_ControlledEntities[i],m_CursorPos,Delta);
        }
    }
    void InputManager::OnKeyDown(char Key)
    {
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnKeyDown(Scpt,m_ControlledEntities[i],Key);
        }
    }
    void InputManager::OnKeyUp(char Key)
    {
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnKeyUp(Scpt,m_ControlledEntities[i],Key);
        }
    }
    void InputManager::OnGamepadAttached(Gamepad_device* Device)
    {
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnGamepadAttached(Scpt,m_ControlledEntities[i],Device->deviceID);
        }
    }
    void InputManager::OnGamepadDetached(Gamepad_device* Device)
    {
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnGamepadDetached(Scpt,m_ControlledEntities[i],Device->deviceID);
        }
    }
    void InputManager::OnButtonDown(u32 DeviceID,u32 ButtonID)
    {
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnButtonDown(Scpt,m_ControlledEntities[i],DeviceID,ButtonID);
        }
    }
    void InputManager::OnButtonUp  (u32 DeviceID,u32 ButtonID)
    {
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnButtonUp(Scpt,m_ControlledEntities[i],DeviceID,ButtonID);
        }
    }
    void InputManager::OnAxisMoved (u32 DeviceID,u32 AxisID,Scalar Position,Scalar Delta)
    {
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_ControlledEntities.size();i++)
        {
            ScriptComponent* Scpt = (ScriptComponent*)m_ControlledEntities[i]->GetComponentByType(CT_SCRIPT);
            if(!Scpt) continue;
            Sys->OnAxisMoved(Scpt,m_ControlledEntities[i],DeviceID,AxisID,Position,Delta);
        }
    }
    void InputManager::Update()
    {
        Gamepad_detectDevices();
        Gamepad_processEvents();
    }
};