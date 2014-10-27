/*
 *  InputManager.h
 *
 *  Created by Michael DeCicco on 9/7/14.
 *
 */

#pragma once
#include "EngineTypes.h"
#include "Gamepad.h"
#include "glfw3.h"

namespace Reality
{
    void KeyCallback(GLFWwindow* Window,i32 Key,i32 Scancode,i32 Action,i32 Mods);
    void MouseButtonCallback(GLFWwindow* Window,i32 ButtonID,i32 Action,i32 Mods);
    void CursorPosCallback(GLFWwindow* Window,f64 x,f64 y);
    class Entity;
    class InputManager
    {
        public:
            InputManager();
            ~InputManager();
        
            void Init();
        
            void SubscribeEntity(Entity* E) { m_ControlledEntities.push_back(E); }
            void UnsubscribeEntity(Entity* E) { for(i32 i = 0;i < m_ControlledEntities.size();i++) { if(m_ControlledEntities[i] == E) { m_ControlledEntities.erase(m_ControlledEntities.begin() + i); } } }
        
            void OnLeftMouseDown();
            void OnLeftMouseUp();
            void OnRightMouseDown();
            void OnRightMouseUp();
            void OnCursorMove(f32 x,f32 y);
            void OnKeyDown(char Key);
            void OnKeyUp(char Key);
            void OnGamepadAttached(Gamepad_device* Device);
            void OnGamepadDetached(Gamepad_device* Device);
            void OnButtonDown(u32 DeviceID,u32 ButtonID);
            void OnButtonUp  (u32 DeviceID,u32 ButtonID);
            void OnAxisMoved (u32 DeviceID,u32 AxisID,Scalar Position,Scalar Delta);
            void Update();
        
        protected:
            vector<Entity*> m_ControlledEntities;
            Vector2 m_CursorPos;
    };
};