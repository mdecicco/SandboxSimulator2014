//
//  SceneGraph.h
//  SandboxSimulator
//
//  Created by Jordan Duty on 12/19/14.
//
//

#ifndef SandboxSimulator_SceneGraph_h
#define SandboxSimulator_SceneGraph_h

#include <System/SSTypes.h>
#include <vector>

#include <memory>

#define EntityPtr std::unique_ptr<Entity>
#define EntityMap std::map<UID, EntityPtr>
#define EntityPair std::pair<UID, EntityPtr>
#define GetEntity(n) m_Entities.find(n)->second.get()

namespace SandboxSimulator
{
    class SSEngine;
    class Entity;

    enum COMPONENT_TYPE
    {
        CT_RENDER,
        CT_TRANSFORM,
        CT_CAMERA,
        CT_COUNT
    };

    class Component
    {
        public:
            Component(COMPONENT_TYPE Type);
            virtual ~Component();

            void AddRef();
            void Destroy();

            void SetEngine(SSEngine* Eng) {m_Engine = Eng;}

            COMPONENT_TYPE GetType() const {return m_Type;}

            virtual void BinarySerialize(sf::Packet* Packet) = 0;
            virtual void BinaryDeserialize(sf::Packet* Packet) = 0;

            void SetParent(Entity* Ent) { m_Parent = Ent; }
            Entity* GetParent() { return m_Parent; }

        protected:
            SSEngine* m_Engine;
            i32 m_RefCount;
            COMPONENT_TYPE m_Type;

            Entity* m_Parent;
    };

    class SceneGraph;

    class Entity
    {
        public:
            Entity(SceneGraph* scene) : m_SceneGraph(scene), m_UID(-1) 
            {
                for(i32 i = 0; i < CT_COUNT; i++)
                    m_HasType[i] = false;
            }


            ~Entity() {}
        
            void AddRef() {m_RefCount++;}
            void Destroy();

            Component* GetComponentByType(COMPONENT_TYPE Type);

            void BinarySerialize(sf::Packet* Packet);
            void BinarySerializePosition(sf::Packet* Packet);
            void BinaryDeserialize(sf::Packet* Packet);
            void BinaryDeserializePosition(sf::Packet* Packet);

            UID GetID() { return m_UID; }

            bool HasComponentType(COMPONENT_TYPE Type)
            {
                return m_HasType[Type];
            }

        protected:
            friend class SceneGraph;
            Component* m_Components[CT_COUNT];
            bool m_HasType[CT_COUNT];
            UID m_UID;
            i32 m_RefCount;

            SceneGraph* m_SceneGraph;
    };

    class SceneGraph
    {
        public:
            SceneGraph(SSEngine* Eng);
            ~SceneGraph();

            Entity* CreateEntity();
            Entity* CreateEntity(UID EntID);
            void DestroyEntity(Entity* E);

            void AddComponent(Entity* E,Component* Comp);
            void RemoveComponent(Entity* E,Component* Comp);
            void RemoveComponentByType(Entity* E,i32 Type);

            void BinarySerialize(sf::Packet* Packet);
            void BinarySerialize(sf::Packet* Packet, UID ExcludeID);
            void BinarySerializePositions(sf::Packet* Packet);
            void BinarySerializePositions(sf::Packet* Packet, UID ExcludeID);
            void BinaryDeserialize(sf::Packet* Packet);
            void BinaryDeserializePositions(sf::Packet* Packet);
            Entity* GetEntityById(UID ID) { return GetEntity(ID); }

            bool HasEntity(UID EntID);

        protected:
            std::map<UID, EntityPtr> m_Entities;
            SSEngine* m_Engine;
    };
}

#endif
