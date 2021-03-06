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

#include <Rendering/RenderSystem.h>

namespace SandboxSimulator
{
    Component::Component(COMPONENT_TYPE Type) : m_RefCount(0), m_Type(Type) 
    {}
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
        return m_Components[Type];
    }

    SceneGraph::SceneGraph(SSEngine* Eng) : m_Engine(Eng)
    {
    }
    
    SceneGraph::~SceneGraph()
    {
        m_Entities.clear();
    }
    
    Entity* SceneGraph::CreateEntity()
    {
        UID id = m_Entities.size();
        //increment id until we get one that hasn't been used.
        while(HasEntity(id))
            id++;

        EntityPtr ptr(new Entity(this));
        ptr.get()->m_UID = id;
        m_Entities.insert(EntityPair(m_Entities.size(), std::move(ptr)));
        return m_Entities[m_Entities.size()-1].get();
    }
    
    Entity* SceneGraph::CreateEntity(UID EntID)
    {
        if(EntID == (u32)-1) return CreateEntity();
        if(HasEntity(EntID))
            return nullptr;

        EntityPtr ptr(new Entity(this));
        ptr.get()->m_UID = EntID;
        m_Entities.insert(EntityPair(EntID, std::move(ptr)));
        return GetEntity(EntID);
    }

    void SceneGraph::DestroyEntity(Entity* E)
    {
        for(i32 i = 0;i < CT_COUNT;i++) {   
            E->m_Components[i]->Destroy();
            delete E->m_Components[i];
        }
    }
    
    void SceneGraph::AddComponent(Entity* E,Component* Comp)
    {
        if(E) {
            Comp->SetEngine(m_Engine);
            E->m_Components[Comp->GetType()] = Comp;
            E->m_HasType[Comp->GetType()] = true;
            Comp->SetParent(E);
		    Comp->AddRef();
            m_Engine->Broadcast(new ComponentAddedMessage(E,Comp));
        }
    }
    
    void SceneGraph::RemoveComponent(Entity* E,Component* Comp)
    {
        for(i32 i = 0;i < CT_COUNT;i++)
        {
            if(E->m_Components[i] == Comp)
            {
                m_Engine->Broadcast(new ComponentRemovedMessage(E,Comp));
                E->m_Components[i]->Destroy();
                delete E->m_Components[i];
            }
        }
    }
    
    void SceneGraph::RemoveComponentByType(Entity* E,i32 Type)
    {
        if(E->m_Components[Type]) {
            m_Engine->Broadcast(new ComponentRemovedMessage(E,E->m_Components[Type]));
            E->m_Components[Type]->Destroy();
            delete E->m_Components[Type];
        }
    }

    bool SceneGraph::HasEntity(UID EntID)
    {
        return m_Entities.find(EntID) != m_Entities.end();
    }
}
