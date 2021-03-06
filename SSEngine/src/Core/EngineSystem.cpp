//
//  EngineSystem.cpp
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#include <Core/EngineSystem.h>
#include <Engine.h>

namespace SandboxSimulator
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
        for(i32 i = 0;i < m_MessageTypes.size();i++)
            if(m_MessageTypes[i] == MsgType)
                return true;
        
        return false;
    }
    
    void EngineSystem::AddComponentType(COMPONENT_TYPE ComponentType)
    {
        m_ComponentTypes.push_back(ComponentType);
    }
    
    bool EngineSystem::AcceptsComponentType(COMPONENT_TYPE ComponentType) const
    {
        for(i32 i = 0;i < m_ComponentTypes.size();i++)
            if(m_ComponentTypes[i] == ComponentType)
                return true;
        
        return false;
    }
    
    void EngineSystem::AddComponent(Component* Comp)
    {
        m_Components.push_back(Comp);
    }
    
    void EngineSystem::RemoveComponent(Component *Comp)
    {
        for(i32 i = 0;i < m_Components.size();i++) if(m_Components[i] == Comp) {
            m_Components.erase(m_Components.begin() + i);
            return;
        }
    }
    
    void EngineSystem::Initialize(SSEngine* ParentEngine)
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

}
