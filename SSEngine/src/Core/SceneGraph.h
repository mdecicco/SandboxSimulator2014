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
        CT_LIGHT,
        CT_PHYSICS,
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


            ~Entity() 
            {
                for(i32 i = 0; i < CT_COUNT; i++) {
                    if(HasComponentType((COMPONENT_TYPE)i))
                        delete m_Components[i];
                }
            }
        
            void AddRef() {m_RefCount++;}
            void Destroy();

            Component* GetComponentByType(COMPONENT_TYPE Type);

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

    #define EntityList std::map<UID, EntityPtr>

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

            Entity* GetEntityById(UID ID) { return GetEntity(ID); }

            bool HasEntity(UID EntID);
            EntityList* GetEntities() { return &m_Entities; };

        protected:
            EntityList m_Entities;
            SSEngine* m_Engine;
    };
}

#endif
