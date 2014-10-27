/*
 *  CullingAlgorithm.h
 *
 *  Created by Michael DeCicco on 9/4/14.
 *
 */

#pragma once
#include "EntityManager.h"
#include "TaskManager.h"

#include <unordered_map>
#include <vector>
using namespace std;

namespace Reality
{
    class RenderComponent;
    class CameraComponent;
    class Entity;
    class Shader;
    
    class RenderList
    {
        public:
            RenderList() { }
            ~RenderList() { }
        
            void AddEntity(Entity* E);
            void RemoveEntity(Entity* E);
        
            i32 GetShaderCount() const { return (i32)m_Shaders.size(); }
            Shader* GetShader(i32 Idx) const { return m_Shaders[Idx]; }
            vector<Entity*>& GetShaderEntityList(i32 Idx) { return m_ShaderEntityListMap[m_Shaders[Idx]]; }
        
        protected:
            unordered_map<Shader*,vector<Entity*>> m_ShaderEntityListMap;
            vector<Shader*>m_Shaders;
            vector<Entity*>m_Lights;
    };
    
    class Task;
    class CullingAlgorithm
    {
        public:
            CullingAlgorithm() : m_EntityList(0) { }
            virtual ~CullingAlgorithm() { }
        
            void InitScene(vector<Entity*>& InEntities) { m_EntityList = &InEntities; VisibilityFlags.reserve(InEntities.size()); Init(); }
            virtual void Init() { }
            virtual void OnEntityAdd(Entity* E) { VisibilityFlags.push_back(false); }
            virtual void OnEntityRemove(Entity* E) { VisibilityFlags.pop_back(); }
        
            virtual bool CreateTask() = 0;
        
            virtual void ToggleDebugDisplay() { }
        
            vector<char> VisibilityFlags;
        
        protected:
            vector<Entity*>* m_EntityList;
    };
    
    class SceneOctree
    {
        public:
            enum CHILD_ID
            {
                LEFT   = 0,
                RIGHT  = 1,
                TOP    = 0,
                BOTTOM = 1,
                NEAR   = 0,
                FAR    = 1
            };
            SceneOctree();
            ~SceneOctree();
        
            void BuildOctree(Scalar Size,const Vector3& Center,i32 Depth,SceneOctree* Parent = 0);
        
            bool AddEntity(Entity* E,bool SkipCheck = false);
            void RemoveEntity(Entity* E);
            void Update();
            void AddDebugVerticesToMesh(RenderComponent* vDest);
            void ComputeVisibleEntities(Vector4 *Planes,RenderList* VisibleEntityList,i32& VisibleLeafCount,bool SkipTest = false,RenderComponent* vDest = 0);
        
        protected:
            bool InternalAddEntity(Entity* E);
            BoundingVolume m_Volume;
            Vector3 m_Center;
            u8 m_Level;
        
            SceneOctree* m_Parent;
            SceneOctree* m_Children[2][2][2];
        
            vector<Entity*> m_StaticEntities;
            vector<Entity*> m_DynamicEntities;
    };
    
    class DefaultFrustumCullingAlgorithm : public CullingAlgorithm
    {
        public:
            DefaultFrustumCullingAlgorithm() : m_Octree(0), m_DebugDisplay(0) { }
            ~DefaultFrustumCullingAlgorithm() { }
        
            virtual void Init();
            virtual void OnEntityAdd(Entity* E);
            virtual void OnEntityRemove(Entity* E);
        
            virtual bool CreateTask();
            
            virtual void ToggleDebugDisplay();
        
        protected:
            Vector4 ClipPlanes[6];
            SceneOctree* m_Octree;
            i32 m_VisibleLeaves;
            Entity* m_DebugDisplay;
    };
    
    class BruteForceCullTask : public Task
    {
        public:
            BruteForceCullTask(Task* Parent) : Task(Parent) { }
            ~BruteForceCullTask() { }
        
            virtual void Run();
        
            Entity** EntityArrayPointer;
            bool* FlagArrayPointer;
            Vector4* Planes;
            i32 Count;
    };
    
    class SystemUpdateTask;
    class BruteForceFrustumCullingAlgorithm : public CullingAlgorithm
    {
        public:
            BruteForceFrustumCullingAlgorithm() { }
            ~BruteForceFrustumCullingAlgorithm() { }
        
            void SetParentTask(SystemUpdateTask* ParentTask) { m_ParentTask = ParentTask; }
        
            virtual void Init();
            virtual bool CreateTask();
        
        protected:
            Vector4 ClipPlanes[6];
            SystemUpdateTask* m_ParentTask;
        
    };
};