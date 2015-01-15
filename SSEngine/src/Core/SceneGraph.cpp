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
        return m_Components[Type];
    }

    void Entity::BinarySerialize(sf::Packet* Packet)
    {
        u8 numComponents = 0;
        for(i32 i = 0; i < CT_COUNT; i++) {
            if(m_Components[i]) 
                numComponents++;
        }

        (*Packet) << m_UID << numComponents;

        for(i32 i = 0; i < CT_COUNT; i++)
        {
            if(m_Components[i])
                m_Components[i]->BinarySerialize(Packet);
        }
    }

    void Entity::BinaryDeserialize(sf::Packet* Packet)
    {
        u8 numComponents;
        (*Packet) >> numComponents;

        for(int i = 0; i < numComponents; i++)
        {
            u8 ComponentType;
            (*Packet) >> ComponentType;
            switch((COMPONENT_TYPE)ComponentType)
            {
                case CT_RENDER:
                    m_SceneGraph->AddComponent(this, new RenderComponent());
                    break;
                default:

                    break;
            }

            m_Components[ComponentType]->BinaryDeserialize(Packet);
        }
    }

    SceneGraph::SceneGraph(SSEngine* Eng) : m_Engine(Eng)
    {
    }
    
    SceneGraph::~SceneGraph()
    {
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
    
    bool SceneGraph::CreateEntity(UID EntID)
    {
        if(HasEntity(EntID))
            return false;

        EntityPtr ptr(new Entity(this));
        ptr.get()->m_UID = EntID;
        m_Entities.insert(EntityPair(EntID, std::move(ptr)));
        return m_Entities[m_Entities.size()-1].get();
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
        //Check if entity already has a component of that type
        Comp->SetEngine(m_Engine);
        E->m_Components[Comp->GetType()] = Comp;
		Comp->AddRef();
        m_Engine->Broadcast(new ComponentAddedMessage(E,Comp));
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

    void SceneGraph::BinarySerialize(sf::Packet* Packet)
    {
        (*Packet) << (u32)m_Entities.size();
        
        for (EntityMap::iterator it=m_Entities.begin(); it!=m_Entities.end(); ++it)
            it->second->BinarySerialize(Packet);
    }

    void SceneGraph::BinaryDeserialize(sf::Packet* Packet)
    {
        u32 numEntities;
        (*Packet) >> numEntities;
        for(i32 i = 0; i < numEntities; i++)
        {
            UID id;
            (*Packet) >> id;
            if(!HasEntity(id)) {
                if(CreateEntity(id))
                    GetEntity(id)->BinaryDeserialize(Packet);
            } else {
                GetEntity(id)->BinaryDeserialize(Packet);
            }
        }
    }

    bool SceneGraph::HasEntity(UID EntID)
    {
        return m_Entities.find(EntID) != m_Entities.end();
    }
}
