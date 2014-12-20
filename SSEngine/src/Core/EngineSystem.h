//
//  EngineSystem.h
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#ifndef SandboxSimulator_EngineSystem_h
#define SandboxSimulator_EngineSystem_h

#include <System/SSTypes.h>
#include <vector>

namespace SandboxSimulator
{
    class SSEngine;
    class EngineMessage;

    class Component
    {
        public:
            Component(SSEngine* eng, i32 Type) : m_Engine(eng), m_RefCount(0), m_Type(Type) {}
        virtual ~Component() {}
        
            void AddRef();
            void Destroy();
        
            i32 GetType() const {return m_Type;}
        protected:
            SSEngine* m_Engine;
            i32 m_RefCount;
            i32 m_Type;
    };
    
    class EngineSystem
    {
        public:
            EngineSystem();
            virtual ~EngineSystem();
        
            void AddMessageType(i32 MsgType);
            bool AcceptsMessageType(i32 MsgType) const;
        
            void AddComponentType(i32 ComponentType);
            bool AcceptsComponentType(i32 ComponentType) const;
        
            void AddComponent(Component* Comp);
            void RemoveComponent(Component* Comp);
        
            virtual void HandleMessage(const EngineMessage* Msg) = 0;
        
            virtual void Initialize(SSEngine* ParentEngine) = 0;
            virtual void Update(Scalar dt) = 0;
            virtual void Shutdown() = 0;
        
            virtual void Serialize() = 0;
            virtual void Deserialize() = 0;
        
        protected:
            std::vector<i32> m_MessageTypes;
            std::vector<i32> m_ComponentTypes;
            std::vector<Component*> m_Components;
    };
}

#endif
