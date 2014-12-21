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
#include <Core/SceneGraph.h>
#include <vector>

namespace SandboxSimulator
{
    class SSEngine;
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
        
            virtual void Initialize(SSEngine* ParentEngine) = 0;
            virtual void Update(Scalar dt) = 0;
            virtual void Shutdown() = 0;
        
            virtual void Serialize() = 0;
            virtual void Deserialize() = 0;
        
        protected:
            std::vector<i32> m_MessageTypes;
            std::vector<COMPONENT_TYPE> m_ComponentTypes;
            std::vector<Component*> m_Components;
    };
}

#endif
