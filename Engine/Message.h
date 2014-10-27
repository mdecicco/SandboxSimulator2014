/*
 *  Message.h
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#pragma once
#include "EngineTypes.h"
#include "Event.h"
#include <string>
using namespace std;

namespace Reality
{
    enum MESSAGE_TYPE
    {
        MT_COMPONENT_ADDED,
        MT_COMPONENT_REMOVED,
        MT_DESTROY_ENTITY,
        MT_SHUTDOWN,
        MT_USER_EVENT,
        MT_TARGETED_USER_EVENT,
        MT_ADD_SCRIPT,
        MT_MAKE_RENDERABLE,
        MT_MAKE_TRANSFORMABLE,
        MT_MAKE_CAMERA,
        MT_MAKE_PHYSICAL,
        MT_SET_SHADER,
        MT_SPAWN_ENTITY,
        MT_INVALID
    };
    
    class EngineMessage
        {
            public:
                EngineMessage() : m_IsSynchronous(false), m_MessageType(MT_INVALID) { }
                virtual ~EngineMessage() { }
            
                /* Whether the message should be sent immediately or at the end of the frame */
                bool m_IsSynchronous;
            
                MESSAGE_TYPE m_MessageType;
        };
    
    class Component;
    class Entity;
    
    /* Happens when a user event is sent to the engine */
    class UserEventMessage : public EngineMessage
    {
        public:
            UserEventMessage(const Event& E) : m_Event(E) { m_IsSynchronous = false; m_MessageType = MT_USER_EVENT; }
            ~UserEventMessage() { }
        
            Event m_Event;
    };
    
    /* Happens when a targeted user event is sent to the engine */
    class TargetedUserEventMessage : public EngineMessage
    {
        public:
            TargetedUserEventMessage(const Event& E,Entity* Target) : m_Event(E), m_TargetEntity(Target) { m_IsSynchronous = false; m_MessageType = MT_TARGETED_USER_EVENT; }
            ~TargetedUserEventMessage() { }
        
            Event m_Event;
            Entity* m_TargetEntity;
    };
    
    /* Happens when any component is added to any entity */
    class ComponentAddedMessage : public EngineMessage
    {
        public:
            ComponentAddedMessage(Entity* E,Component* C) : m_Entity(E), m_Component(C) { m_IsSynchronous = true; m_MessageType = MT_COMPONENT_ADDED; }
            ~ComponentAddedMessage() { }
        
            Entity* m_Entity;
            Component* m_Component;
    };
    
    /* Happens when any component is removed from an entity */
    class ComponentRemovedMessage : public EngineMessage
    {
        public:
            ComponentRemovedMessage(Entity* E,Component* C) : m_Entity(E), m_Component(C) { m_IsSynchronous = true; m_MessageType = MT_COMPONENT_REMOVED; }
            ~ComponentRemovedMessage() { }
        
            Entity* m_Entity;
            Component* m_Component;
    };
    
    /* Happens when an entity should be destroyed */
    class DestroyEntityMessage : public EngineMessage
    {
        public:
            DestroyEntityMessage(Entity* E) : m_Entity(E) { m_IsSynchronous = false; m_MessageType = MT_DESTROY_ENTITY; }
            ~DestroyEntityMessage() { }
        
            Entity* m_Entity;
    };
    
    /* Happens when the engine should shutdown */
    class ShutdownMessage : public EngineMessage
    {
        public:
            ShutdownMessage() { m_IsSynchronous = false; m_MessageType = MT_SHUTDOWN; }
            ~ShutdownMessage() { }
    };
    
    /* Happens when a script component should be added to an entity */
    class AddScriptMessage : public EngineMessage
    {
        public:
            AddScriptMessage(Entity* E,const string& ScriptPath,Scalar Rate) : m_Entity(E), m_UpdateRate(Rate)
            {
                m_ScriptPath = ScriptPath;
                m_IsSynchronous = false;
                m_MessageType = MT_ADD_SCRIPT;
            }
            ~AddScriptMessage() { }
        
            Entity* m_Entity;
            string m_ScriptPath;
            Scalar m_UpdateRate;
    };
    
    /* Happens when a render component should be added to an entity */
    class MakeRenderableMessage : public EngineMessage
    {
        public:
            MakeRenderableMessage(Entity* E) : m_Entity(E)
            {
                m_IsSynchronous = false;
                m_MessageType = MT_MAKE_RENDERABLE;
            }
            ~MakeRenderableMessage() { }
        
            Entity* m_Entity;
    };
    
    /* Happens when a transform component should be added to an entity */
    class MakeTransformableMessage : public EngineMessage
    {
        public:
            MakeTransformableMessage(Entity* E) : m_Entity(E)
            {
                m_IsSynchronous = true;
                m_MessageType = MT_MAKE_TRANSFORMABLE;
            }
            ~MakeTransformableMessage() { }
        
            Entity* m_Entity;
    };
    
    /* Happens when an entity should become a camera */
    class MakeCameraMessage : public EngineMessage
    {
        public:
            MakeCameraMessage(Entity* E) : m_Entity(E)
            {
                m_IsSynchronous = true;
                m_MessageType = MT_MAKE_CAMERA;
            }
            ~MakeCameraMessage() { }
        
            Entity* m_Entity;
    };
    
    /* Happens when an entity should become a part of the physics simulation */
    class MakePhysicalMessage : public EngineMessage
    {
        public:
            MakePhysicalMessage(Entity* E,Scalar _Mass,Scalar _Restitution = 0.2f,Scalar _Friction = 0.4f,const string& _Shape = "") : m_Entity(E),
                    Mass(_Mass), Restitution(_Restitution), Friction(_Friction), Shape(_Shape)
            {
                m_IsSynchronous = false;
                m_MessageType = MT_MAKE_PHYSICAL;
            }
            ~MakePhysicalMessage() { }
        
            Entity* m_Entity;
            Scalar Mass;
            Scalar Restitution;
            Scalar Friction;
            string Shape;
    };
    
    /* Happens when a shader is added to a render component */
    class SetShaderMessage : public EngineMessage
    {
        public:
            SetShaderMessage(Entity* E,const string& Path)
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
};