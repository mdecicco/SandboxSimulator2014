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
    Component::Component(COMPONENT_TYPE Type) : m_RefCount(0), m_Type(Type) {}
    Component::~Component() {}

    void Component::AddRef()
    {
        m_RefCount++;
    }

    void Component::Destroy()
    {
        m_RefCount--;
        if(m_RefCount == 0) delete this;
    }

    void Entity::Destroy()
    {
        m_RefCount--;
        if(m_RefCount == 0) delete this;
    }

    Component* Entity::GetComponentByType(COMPONENT_TYPE Type)
    {
        for(i32 i = 0; i < m_Components.size(); i++)
        {
            if(m_Components[i]->GetType() == Type)
                return m_Components[i];
        }

        return nullptr;
    }

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
        //Check if entity already has a component of that type
        if(E->GetComponentByType(Comp->GetType()))
        {
            m_Engine->Log("Entity <%d> already has a component of that type!", E->GetID());
            return;
        }

        Comp->SetEngine(m_Engine);
        E->m_Components.push_back(Comp);
		Comp->AddRef();
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
}
