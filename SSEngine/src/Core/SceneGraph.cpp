//
//  SceneGraph.cpp
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#include <Core/SceneGraph.h>
#include <Core/EngineSystem.h>
#include <Core/Message.h>
#include <Engine.h>

namespace SandboxSimulator
{
    SceneGraph::SceneGraph(SSEngine* Eng) : m_Engine(Eng)
    {
    }
    
    SceneGraph::~SceneGraph()
    {
    }
    
    Entity* SceneGraph::CreateEntity()
    {
        Entity* E = new Entity();
        m_Entities.push_back(E);
        E->m_UID = (UID)m_Entities.size() - 1;
        return E;
    }
    void SceneGraph::DestroyEntity(Entity* E)
    {
        for(i32 i = 0;i < E->m_Components.size();i++) E->m_Components[i]->Destroy();
        E->m_Components.clear();
    }
    
    void SceneGraph::AddComponent(Entity* E,Component* Comp)
    {
        E->m_Components.push_back(Comp);
        m_Engine->Broadcast(new ComponentAddedMessage(E,Comp));
    }
    
    void SceneGraph::RemoveComponent(Entity* E,Component* Comp)
    {
        for(i32 i = 0;i < E->m_Components.size();i++)
        {
            if(E->m_Components[i] == Comp)
            {
                m_Engine->Broadcast(new ComponentRemovedMessage(E,Comp));
                E->m_Components[i]->Destroy();
                E->m_Components.erase(E->m_Components.begin() + i);
            }
        }
    }
    
    void SceneGraph::RemoveComponentByType(Entity* E,i32 Type)
    {
        for(i32 i = 0;i < E->m_Components.size();i++)
        {
            if(E->m_Components[i]->GetType() == Type)
            {
                m_Engine->Broadcast(new ComponentRemovedMessage(E,E->m_Components[i]));
                E->m_Components[i]->Destroy();
                E->m_Components.erase(E->m_Components.begin() + i);
            }
        }
    }
};