//
//  Message.h
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#ifndef SandboxSimulator_Message_h
#define SandboxSimulator_Message_h

namespace SandboxSimulator
{
    enum MESSAGE_TYPE
    {
        MT_COMPONENT_ADDED,
        MT_COMPONENT_REMOVED,
        MT_SHUTDOWN,
        MT_INVALID
    };
    
    class EngineMessage
    {
        public:
            EngineMessage() : m_IsSynchronous(false), m_MessageType(MT_INVALID) {}
        ~EngineMessage() {}
    
            bool m_IsSynchronous;
            MESSAGE_TYPE m_MessageType;
    };
    
    class Component;
    class Entity;
    
    class ComponentAddedMessage : public EngineMessage
    {
        public:
            ComponentAddedMessage(Entity* E,Component* C) : m_Entity(E), m_Component(C) { m_IsSynchronous = true; m_MessageType = MT_COMPONENT_ADDED; }
            ~ComponentAddedMessage() { }
        
            Entity* m_Entity;
            Component* m_Component;
    };
    
    class ComponentRemovedMessage : public EngineMessage
    {
        public:
            ComponentRemovedMessage(Entity* E,Component* C) : m_Entity(E), m_Component(C) { m_IsSynchronous = true; m_MessageType = MT_COMPONENT_REMOVED; }
        ~ComponentRemovedMessage() { }
        
            Entity* m_Entity;
            Component* m_Component;
    };
    
    class ShutdownMessage : public EngineMessage
    {
        public:
            ShutdownMessage() { m_IsSynchronous = true; m_MessageType = MT_SHUTDOWN; }
            ~ShutdownMessage() { }
    };
}

#endif
