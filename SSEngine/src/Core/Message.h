//
//  Message.h
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#ifndef SandboxSimulator_Message_h
#define SandboxSimulator_Message_h

#include <System/SSTypes.h>
#include <String>
using namespace std;

namespace SandboxSimulator
{
    enum MESSAGE_TYPE
    {
        MT_COMPONENT_ADDED,
        MT_COMPONENT_REMOVED,
        MT_MAKE_RENDERABLE,
        MT_SET_SHADER,
        MT_SHUTDOWN,
        MT_INVALID
    };
    
    class EngineMessage
    {
        public:
            EngineMessage() : m_IsSynchronous(false), m_MessageType(MT_INVALID) {}
            virtual ~EngineMessage() {}
    
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
            ShutdownMessage() { m_IsSynchronous = false; m_MessageType = MT_SHUTDOWN; }
            ~ShutdownMessage() { }
    };

    class MakeRenderableMessage : public EngineMessage
    {
        public:
            MakeRenderableMessage(Entity* E) : m_Entity(E)
            {
                m_IsSynchronous = false;
                m_MessageType = MT_MAKE_RENDERABLE;
            }
            ~MakeRenderableMessage() {}

            Entity* m_Entity;
    };

    class SetShaderMessage : public EngineMessage
    {
        public:
            SetShaderMessage(Entity* E, const string& Path)
            {
                m_Entity = E;
                m_ShaderPath = Path;
                m_IsSynchronous = false;
                m_MessageType = MT_SET_SHADER;
            }
            ~SetShaderMessage() { }

            Entity* m_Entity;
            string m_ShaderPath;
    };
}

#endif
