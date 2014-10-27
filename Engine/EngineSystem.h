/*
 *  EngineSystem.h
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#pragma once
#include "EngineTypes.h"
#include "EntityManager.h"
#include <unordered_map>
#include <vector>
using namespace std;

namespace Reality
{
    class Engine;
    class EngineMessage;
    
    class EngineSystem
    {
        public:
            EngineSystem();
            virtual ~EngineSystem();
        
            void AddMessageType(i32 MsgType);
            bool AcceptsMessageType(i32 MsgType) const;
        
            void AddComponentType(COMPONENT_TYPE ComponentType);
            bool AcceptsComponentType(COMPONENT_TYPE ComponentType) const;
        
            void AddComponent(Component* Comp);
            void RemoveComponent(Component* Comp);
        
            virtual void HandleMessage(const EngineMessage* Msg) = 0;
        
            virtual void Initialize() = 0;
            virtual void Update(Scalar dt) = 0;
            virtual void Shutdown() = 0;
        
            virtual void Serialize() = 0;
            virtual void Deserialize() = 0;
            
        protected:
            vector<i32> m_MessageTypes;
            vector<COMPONENT_TYPE> m_ComponentTypes;
            vector<Component*> m_Components;
            unordered_map<Component*,i32> m_ComponentIndices;
    };
};