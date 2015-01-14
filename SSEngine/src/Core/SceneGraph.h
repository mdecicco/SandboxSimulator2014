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

namespace SandboxSimulator
{
    class SSEngine;

    enum COMPONENT_TYPE
    {
        CT_RENDER,
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

        protected:
            SSEngine* m_Engine;
            i32 m_RefCount;
            COMPONENT_TYPE m_Type;
    };

    class SceneGraph;

    class Entity
    {
        public:
            Entity(SceneGraph* scene) : m_SceneGraph(scene), m_UID(-1) {}
            ~Entity() {}
        
            void AddRef() {m_RefCount++;}
            void Destroy();

            Component* GetComponentByType(COMPONENT_TYPE Type);

            void BinarySerialize(sf::Packet* Packet);
            void BinaryDeserialize(sf::Packet* Packet);

            UID GetID() { return m_UID; }

        protected:
            friend class SceneGraph;
            Component* m_Components[CT_COUNT];
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
            void DestroyEntity(Entity* E);
        
            void AddComponent(Entity* E,Component* Comp);
            void RemoveComponent(Entity* E,Component* Comp);
            void RemoveComponentByType(Entity* E,i32 Type);

            void BinarySerialize(sf::Packet* Packet);
            void BinaryDeserialize(sf::Packet* Packet);
        
        protected:
            i32 m_DeadEntityCount;
            std::vector<Entity*> m_Entities;
            SSEngine* m_Engine;
    };
}

#endif
