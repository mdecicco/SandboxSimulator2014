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
    class Component;
    class SSEngine;
    class Entity
    {
        public:
            Entity() : m_UID(-1) {}
            ~Entity();
        
        protected:
            friend class SceneGraph;
            std::vector<Component*> m_Components;
            UID m_UID;
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
        
        protected:
            i32 m_DeadEntityCount;
            std::vector<Entity*> m_Entities;
            SSEngine* m_Engine;
    };
}

#endif
