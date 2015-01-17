#ifndef SandboxSimulator_RenderAlgorithm_h
#define SandboxSimulator_RenderAlgorithm_h

#include <Core/SceneGraph.h>
#include <unordered_map>
#include <vector>

#include <Utils/Math.h>

namespace SandboxSimulator {
class RenderComponent;
class CameraComponent;
class TransformComponent;
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
        std::vector<Entity*>& GetShaderEntityList(i32 Idx) { return m_ShaderEntityListMap[m_Shaders[Idx]]; }
    
    protected:
        std::unordered_map<Shader*,std::vector<Entity*>> m_ShaderEntityListMap;
        std::vector<Shader*>m_Shaders;
        std::vector<Entity*>m_Lights;
};

class RenderSystem;
class RenderingAlgorithm
{
    public:
        RenderingAlgorithm() { }
        virtual ~RenderingAlgorithm() { }
    
        virtual const char* GetAlgorithmName() const = 0;
        /* Returns number of entities rendered */
        virtual i32 Render(RenderSystem* rSys,RenderList& List) = 0;
};

class ForwardRenderingAlgorithm : public RenderingAlgorithm
{
    public:
        ForwardRenderingAlgorithm() { }
        ~ForwardRenderingAlgorithm() { }
    
        virtual const char* GetAlgorithmName() const { return "Forward"; }
        virtual i32 Render(RenderSystem* rSys,RenderList& List);
};

#ifdef CULALG_USE
class CullingAlgorithm
{
    public:
        CullingAlgorithm() : m_EntityList(0), m_FreezeClipPlanes(false), m_TaskCount(0) { }
        virtual ~CullingAlgorithm() { }
    
        void InitScene(std::vector<Entity*>& InEntities) { m_EntityList = &InEntities; VisibilityFlags.reserve(InEntities.size()); Init(); }
        virtual void Init() { }
        virtual void OnEntityAdd(Entity* E) { VisibilityFlags.push_back(false); }
        virtual void OnEntityRemove(Entity* E) { VisibilityFlags.pop_back(); }
    
        virtual bool CreateTask() = 0;
    
        virtual void ToggleDebugDisplay() { }
        void ToggleFreezeClipPlanes() { m_FreezeClipPlanes = !m_FreezeClipPlanes; }
        void ComputeClipPlanes(CameraComponent* Camera,TransformComponent* CamTrans);
    
        void IncTaskCount() { m_TaskCountMutex.Lock(); m_TaskCount++; m_TaskCountMutex.Unlock(); }
        void SignalTaskComplete() { m_TaskCountMutex.Lock(); m_TaskCount--; if(m_TaskCount == 0) { m_CompleteCondition.Signal(); } m_TaskCountMutex.Unlock(); }
        void WaitForComplete();
    
        Vec4 ClipPlanes[6];
        std::vector<Entity*>* m_EntityList;
        std::vector<char> VisibilityFlags;
    protected:
        bool m_FreezeClipPlanes;
        i32 m_TaskCount;
        Mutex m_TaskCountMutex;
        ThreadCondition m_CompleteCondition;
};
class DefaultFrustumCullingAlgorithm;
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
    
        void BuildOctree(Scalar Size,const Vec3& Center,i32 Depth,SceneOctree* Parent = 0);
    
        bool AddEntity(Entity* E,bool SkipCheck = false);
        void RemoveEntity(Entity* E);
        void Update();
        void AddDebugVerticesToMesh(DefaultFrustumCullingAlgorithm* Algo,RenderComponent* vDest);
        void ComputeVisibleEntities(DefaultFrustumCullingAlgorithm* Algo,Vec4 *Planes,bool* VisibilityFlags,bool SkipTest = false,RenderComponent* vDest = 0);
    
        SceneOctree* m_Parent;
        SceneOctree* m_Children[2][2][2];
    protected:
        bool InternalAddEntity(Entity* E);
        BoundingVolume m_Volume;
        Vec3 m_Center;
        u8 m_Level;
    
        std::vector<Entity*> m_StaticEntities;
        std::vector<Entity*> m_DynamicEntities;
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
        void RecurseCreateTask(SceneOctree* Branch,i32 MaxLevel,i32 Level);
        
        virtual void ToggleDebugDisplay();
    
        CameraComponent* Camera;
        TransformComponent* CamTrans;
        Entity* m_DebugDisplay;
        Mutex m_DebugDisplayMutex;
        std::unordered_map<Entity*,i32> EntityFlagMap;
        
    protected:
        SceneOctree* m_Octree;
        i32 m_VisibleLeaves;
};

class OctreeFrustumCullingTask : public Task
{
    public:
        OctreeFrustumCullingTask(Task* Parent) : Task(Parent) { }
        ~OctreeFrustumCullingTask() { }
    
        virtual void Run();
    
        bool* VisibilityFlags;
        SceneOctree* Leaf;
        DefaultFrustumCullingAlgorithm* Algo;
};

class SystemUpdateTask;
class BruteForceFrustumCullingAlgorithm : public CullingAlgorithm
{
    public:
        BruteForceFrustumCullingAlgorithm() : m_ParentTask(0), m_EntitiesPerTask(200) { }
        ~BruteForceFrustumCullingAlgorithm() { }
    
        void SetParentTask(SystemUpdateTask* ParentTask) { m_ParentTask = ParentTask; }
    
        virtual void Init();
        virtual bool CreateTask();
    
        i32 m_EntitiesPerTask;
    protected:
        SystemUpdateTask* m_ParentTask;
    
};

class BruteForceCullTask : public Task
{
    public:
        BruteForceCullTask(Task* Parent) : Task(Parent) { }
        ~BruteForceCullTask() { }
    
        virtual void Run();
    
        i32 Begin;
        i32 Count;
        BruteForceFrustumCullingAlgorithm* Algo;
};

#endif
}

#endif