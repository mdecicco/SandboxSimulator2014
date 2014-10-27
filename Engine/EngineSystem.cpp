/*
 *  EngineSystem.cpp
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#include "EngineSystem.h"
#include "Engine.h"

namespace Reality
{
    EngineSystem::EngineSystem()
    {
    }
    EngineSystem::~EngineSystem()
    {
    }
    void EngineSystem::AddMessageType(i32 MsgType)
    {
        m_MessageTypes.push_back(MsgType);
    }
    bool EngineSystem::AcceptsMessageType(i32 MsgType) const
    {
        for(i32 i = 0;i < m_MessageTypes.size();i++) if(m_MessageTypes[i] == MsgType) return true;
        return false;
    }
    void EngineSystem::AddComponentType(COMPONENT_TYPE ComponentType)
    {
        m_ComponentTypes.push_back(ComponentType);
    }
    bool EngineSystem::AcceptsComponentType(COMPONENT_TYPE ComponentType) const
    {
        for(i32 i = 0;i < m_ComponentTypes.size();i++) if(m_ComponentTypes[i] == ComponentType) return true;
        return false;
    }
    void EngineSystem::AddComponent(Component* Comp)
    {
        if(m_ComponentIndices.find(Comp) != m_ComponentIndices.end()) return;
        m_ComponentIndices[Comp] = (i32)m_Components.size();
        m_Components.push_back(Comp);
    }
    void EngineSystem::RemoveComponent(Component *Comp)
    {
        if(m_ComponentIndices.find(Comp) != m_ComponentIndices.end())
        {
            Log("Unable to remove component because does not exist in system.\n");
            return;
        }
        m_Components.erase(m_Components.begin() + m_ComponentIndices[Comp]);
        m_ComponentIndices.erase(Comp);
    }
    
    void EngineSystem::Initialize()
    {
    }
    void EngineSystem::Update(Scalar dt)
    {
    }
    void EngineSystem::Shutdown()
    {
    }

    void EngineSystem::Serialize()
    {
    }
    void EngineSystem::Deserialize()
    {
    }
};