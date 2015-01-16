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

    void Entity::BinarySerialize(sf::Packet* Packet)
    {
        u8 numComponents = 0;
        for(i32 i = 0; i < CT_COUNT; i++) {
            if(HasComponentType((COMPONENT_TYPE)i)) 
                numComponents++;
        }

        (*Packet) << m_UID << numComponents;

        for(i32 i = 0; i < CT_COUNT; i++)
        {
            if(HasComponentType((COMPONENT_TYPE)i))
                m_Components[i]->BinarySerialize(Packet);
        }
    }

    void Entity::BinarySerializePosition(sf::Packet* Packet)
    {
        if(HasComponentType(CT_TRANSFORM)) {
            TransformComponent* Trans = (TransformComponent*)GetComponentByType(CT_TRANSFORM);
            Vec3 Pos = Trans->GetPosition();
            (*Packet) << m_UID << Pos.x << Pos.y << Pos.z;
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
            if(!HasComponentType((COMPONENT_TYPE)ComponentType)) {
                switch((COMPONENT_TYPE)ComponentType)
                {
                    case CT_RENDER:
                        m_SceneGraph->AddComponent(this, new RenderComponent());
                        break;
                    case CT_TRANSFORM:
                        m_SceneGraph->AddComponent(this, new TransformComponent());
                        break;
                    default:

                        break;
                }
            }

            m_Components[ComponentType]->BinaryDeserialize(Packet);
        }
    }

    void Entity::BinaryDeserializePosition(sf::Packet* Packet)
    {
        if(HasComponentType(CT_TRANSFORM)) {
            Vec3 Pos = Vec3();
            (*Packet) >> Pos.x >> Pos.y >> Pos.z;
            TransformComponent* Trans = (TransformComponent*)GetComponentByType(CT_TRANSFORM);
            Trans->SetPosition(Pos);
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
    
    Entity* SceneGraph::CreateEntity(UID EntID)
    {
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

    void SceneGraph::BinarySerialize(sf::Packet* Packet)
    {
        (*Packet) << (u32)m_Entities.size();
        
        for (EntityMap::iterator it=m_Entities.begin(); it!=m_Entities.end(); ++it)
            it->second->BinarySerialize(Packet);
    }

    void SceneGraph::BinarySerialize(sf::Packet* Packet, UID ExcludeID)
    {
        if(HasEntity(ExcludeID))
            (*Packet) << (u32)m_Entities.size()-1;
        else
            (*Packet) << (u32)m_Entities.size();

        for (EntityMap::iterator it=m_Entities.begin(); it!=m_Entities.end(); ++it) {
            if(it->second->m_UID != ExcludeID)
                it->second->BinarySerialize(Packet);
        }
    }

    void SceneGraph::BinarySerializePositions(sf::Packet* Packet)
    {
        (*Packet) << (u32)m_Entities.size();

        for (EntityMap::iterator it=m_Entities.begin(); it!=m_Entities.end(); ++it)
            it->second->BinarySerializePosition(Packet);
    }

    void SceneGraph::BinarySerializePositions(sf::Packet* Packet, UID ExcludeID)
    {
        if(HasEntity(ExcludeID))
            (*Packet) << (u32)m_Entities.size()-1;
        else
            (*Packet) << (u32)m_Entities.size();

        for (EntityMap::iterator it=m_Entities.begin(); it!=m_Entities.end(); ++it) {
            if(it->second->m_UID != ExcludeID)
                it->second->BinarySerializePosition(Packet);
        }
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
                if(CreateEntity(id) != nullptr)
                    GetEntity(id)->BinaryDeserialize(Packet);
            } else {
                GetEntity(id)->BinaryDeserialize(Packet);
            }
        }
    }

    void SceneGraph::BinaryDeserializePositions(sf::Packet* Packet)
    {
        u32 numEntities;
        (*Packet) >> numEntities;
        for(i32 i = 0; i < numEntities; i++)
        {
            UID id;
            (*Packet) >> id;
            if(HasEntity(id)) {
                GetEntity(id)->BinaryDeserializePosition(Packet);
            } else {
                //Just advance the packet by the byte amount, we should get a packet telling us about the new entity and a new position for it later on.
                Vec3 Pos;
                (*Packet) >> Pos.x >> Pos.y >> Pos.z;
            }
        }
    }

    bool SceneGraph::HasEntity(UID EntID)
    {
        return m_Entities.find(EntID) != m_Entities.end();
    }
}
