/*
 *  CullingAlgorithm.cpp
 *
 *  Created by Michael DeCicco on 9/4/14.
 *
 */

#include "Engine.h"
#include "EngineTasks.h"
#include "CullingAlgorithm.h"

namespace Reality
{
    void RenderList::AddEntity(Entity *E)
    {
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        //If shader list exists
        Shader* s = r->m_Shdr;
        auto m = m_ShaderEntityListMap.find(s);
        if(m != m_ShaderEntityListMap.end())
        {
            //Add entity to it
            r->m_ShdrListPos = (i32)m->second.size();
            m->second.push_back(E);
        }
        else
        {
            //Otherwise, create list and add entity to it
            m_Shaders.push_back(s);
            m_ShaderEntityListMap[s] = vector<Entity*>();
            m_ShaderEntityListMap[s].push_back(E);
            r->m_ShdrListPos = 0;
        }
        
        if(E->GetComponentByType(CT_LIGHT)) m_Lights.push_back(E);
    }
    void RenderList::RemoveEntity(Entity *E)
    {
        //If shader list exists
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        Shader* s = r->m_Shdr;
        auto m = m_ShaderEntityListMap.find(s);
        if(m != m_ShaderEntityListMap.end())
        {
            //Add entity to it
            m->second.erase(m->second.begin() + r->m_ShdrListPos);
            for(i32 i = r->m_ShdrListPos;i < m->second.size();i++) ((RenderComponent*)m->second[i]->GetComponentByType(CT_RENDER))->m_ShdrListPos--;
        }
        
        if(E->GetComponentByType(CT_LIGHT))
        {
            for(i32 i = 0;i < m_Lights.size();i++)
            {
                if(m_Lights[i] == E)
                {
                    m_Lights.erase(m_Lights.begin() + i);
                    break;
                }
            }
        }
    }
    
    SceneOctree::SceneOctree()
    {
        m_Parent = 0;
        m_Volume.TargetCenter = &m_Center;
        
        for(i32 x = 0;x < 2;x++)
        {
            for(i32 y = 0;y < 2;y++)
            {
                for(i32 z = 0;z < 2;z++)
                {
                    m_Children[x][y][z] = 0;
                }
            }
        }
    }
    SceneOctree::~SceneOctree()
    {
        for(i32 x = 0;x < 2;x++)
        {
            for(i32 y = 0;y < 2;y++)
            {
                for(i32 z = 0;z < 2;z++)
                {
                    delete m_Children[x][y][z];
                }
            }
        }
        m_Volume.TargetCenter = 0;
    }

    void SceneOctree::BuildOctree(Scalar Size,const Vector3& Center,i32 Depth,SceneOctree* Parent)
    {
        m_Parent = Parent;
        m_Center = Center;
        m_Volume.Dimensions = Vector3(Size,Size,Size);
        m_Level = Depth;
        
        if(Depth <= 0) return;
        
        for(i32 x = 0;x < 2;x++)
        {
            for(i32 y = 0;y < 2;y++)
            {
                for(i32 z = 0;z < 2;z++)
                {
                    m_Children[x][y][z] = new SceneOctree();
                }
            }
        }
        
        Scalar hSize = Size * 0.5f;
        Scalar fSize = hSize * 0.5f;
        m_Children[LEFT ][TOP   ][FAR ]->BuildOctree(hSize,Center + Vector3(-fSize, fSize, fSize),Depth - 1,this);
        m_Children[LEFT ][TOP   ][NEAR]->BuildOctree(hSize,Center + Vector3(-fSize, fSize,-fSize),Depth - 1,this);
        m_Children[LEFT ][BOTTOM][FAR ]->BuildOctree(hSize,Center + Vector3(-fSize,-fSize, fSize),Depth - 1,this);
        m_Children[LEFT ][BOTTOM][NEAR]->BuildOctree(hSize,Center + Vector3(-fSize,-fSize,-fSize),Depth - 1,this);
        m_Children[RIGHT][TOP   ][FAR ]->BuildOctree(hSize,Center + Vector3( fSize, fSize, fSize),Depth - 1,this);
        m_Children[RIGHT][TOP   ][NEAR]->BuildOctree(hSize,Center + Vector3( fSize, fSize,-fSize),Depth - 1,this);
        m_Children[RIGHT][BOTTOM][FAR ]->BuildOctree(hSize,Center + Vector3( fSize,-fSize, fSize),Depth - 1,this);
        m_Children[RIGHT][BOTTOM][NEAR]->BuildOctree(hSize,Center + Vector3( fSize,-fSize,-fSize),Depth - 1,this);
    }

    bool SceneOctree::AddEntity(Entity* E,bool SkipCheck)
    {
        TransformComponent* t = (TransformComponent*)E->GetComponentByType(CT_TRANSFORM);
        if(!t) return false;
        const BoundingVolume& tv = t->GetBoundingVolume();
        if(!SkipCheck)
        {
            if(m_Volume.Dimensions.x < tv.Dimensions.x
            || m_Volume.Dimensions.y < tv.Dimensions.y
            || m_Volume.Dimensions.z < tv.Dimensions.z)
            {
                return m_Parent->AddEntity(E);
            }
            if(tv.IntersectsVolume(m_Volume))
            {
                if(!m_Children[0][0][0])
                {
                    return InternalAddEntity(E);
                }
                for(i32 x = 0;x < 2;x++)
                {
                    for(i32 y = 0;y < 2;y++)
                    {
                        for(i32 z = 0;z < 2;z++)
                        {
                            if(m_Children[x][y][z]->m_Volume.IntersectsVolume(t->GetBoundingVolume()))
                            {
                                if(!m_Children[x][y][z]->AddEntity(E,true)) return InternalAddEntity(E);
                                return true;
                            }
                        }
                    }
                }
                //Log("Problem!\n");
                //The only solution...?
                //((RenderComponent*)E->GetComponentByType(CT_RENDER))->GetOctreeNode()->InternalAddEntity(E);
                return false;
            }
            else if(m_Parent)
            {
                return m_Parent->AddEntity(E);
            }
            else return false;
        }
        else
        {
            if(!m_Children[0][0][0])
            {
                return InternalAddEntity(E);
            }
            
            for(i32 x = 0;x < 2;x++)
            {
                for(i32 y = 0;y < 2;y++)
                {
                    for(i32 z = 0;z < 2;z++)
                    {
                        if(m_Children[x][y][z]->m_Volume.IntersectsVolume(t->GetBoundingVolume()))
                        {
                            if(!m_Children[x][y][z]->AddEntity(E,true)) return InternalAddEntity(E);
                            return true;
                        }
                    }
                }
            }
            //Log("Problem!\n");
            //The only solution...?
            //((RenderComponent*)E->GetComponentByType(CT_RENDER))->GetOctreeNode()->InternalAddEntity(E);
            return false;
        }
    }
    void SceneOctree::RemoveEntity(Entity* E)
    {
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        if(!r->m_OctreeNode) return;
        if(r->m_OctreeNode != this) { r->m_OctreeNode->RemoveEntity(E); return; }
        
        TransformComponent* t = (TransformComponent*)E->GetComponentByType(CT_TRANSFORM);
        if(t->IsStatic())
        {
            m_StaticEntities.erase(m_StaticEntities.begin() + r->m_OctreeIndex);
        }
        else
        {
            m_DynamicEntities.erase(m_DynamicEntities.begin() + r->m_OctreeIndex);
        }
        r->m_OctreeIndex = 0;
        r->m_OctreeNode = 0;
    }
    bool SceneOctree::InternalAddEntity(Entity *E)
    {
        TransformComponent* t = (TransformComponent*)E->GetComponentByType(CT_TRANSFORM);
        const BoundingVolume& tv = t->GetBoundingVolume();
        if(m_Volume.Dimensions.x < tv.Dimensions.x
        || m_Volume.Dimensions.y < tv.Dimensions.y
        || m_Volume.Dimensions.z < tv.Dimensions.z) return false;
        
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        
        if(t->IsStatic()) { r->m_OctreeIndex = (i32)m_StaticEntities.size(); m_StaticEntities.push_back(E); }
        else { r->m_OctreeIndex = (i32)m_DynamicEntities.size(); m_DynamicEntities.push_back(E); }
        
        ((RenderComponent*)E->GetComponentByType(CT_RENDER))->m_OctreeNode = this;
        return true;
    }
    void SceneOctree::Update()
    {
        if(!m_Children[0][0][0])
        {
            for(i32 i = 0;i < m_DynamicEntities.size();i++)
            {
                TransformComponent* t = (TransformComponent*)m_DynamicEntities[i]->GetComponentByType(CT_TRANSFORM);
                RenderComponent   * r = (RenderComponent   *)m_DynamicEntities[i]->GetComponentByType(CT_RENDER   );
                if(!t->DidMove() && !r->IsGUI()) continue;
                
                t->SetDimensions(r->m_Dimensions);
                const BoundingVolume& tv = t->GetBoundingVolume();
                if(r->IsGUI()
                || !m_Volume.IntersectsVolume(tv)
                ||  m_Volume.Dimensions.x < tv.Dimensions.x
                ||  m_Volume.Dimensions.y < tv.Dimensions.y
                ||  m_Volume.Dimensions.z < tv.Dimensions.z)
                {
                    if(m_Parent && !r->IsGUI())
                    {
                        if(!m_Parent->AddEntity(m_DynamicEntities[i]))
                        {
                            Log("Warning: Entity[%d] out of world bounds.\n",m_DynamicEntities[i]->GetID());
                        }
                    }
                    m_DynamicEntities.erase(m_DynamicEntities.begin() + i);
                    i--;
                }
            }
            return;
        }
        
        m_Children[LEFT ][TOP   ][FAR ]->Update();
        m_Children[LEFT ][TOP   ][NEAR]->Update();
        m_Children[LEFT ][BOTTOM][FAR ]->Update();
        m_Children[LEFT ][BOTTOM][NEAR]->Update();
        m_Children[RIGHT][TOP   ][FAR ]->Update();
        m_Children[RIGHT][TOP   ][NEAR]->Update();
        m_Children[RIGHT][BOTTOM][FAR ]->Update();
        m_Children[RIGHT][BOTTOM][NEAR]->Update();
    }
    void SceneOctree::AddDebugVerticesToMesh(RenderComponent *vDest)
    {
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f, 0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f, 0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f, 0.5f,-0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f, 0.5f,-0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f,-0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f,-0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f,-0.5f,-0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f,-0.5f,-0.5f)));
        
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f, 0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f, 0.5f,-0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f,-0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f,-0.5f,-0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f, 0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f, 0.5f,-0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f,-0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f,-0.5f,-0.5f)));
            
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f, 0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f,-0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f, 0.5f,-0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3( 0.5f,-0.5f,-0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f, 0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f,-0.5f, 0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f, 0.5f,-0.5f)));
        vDest->AddVertex(m_Center + (m_Volume.Dimensions * Vector3(-0.5f,-0.5f,-0.5f)));
        
        Vector3 c =  ColorFunc(Scalar(m_Level) / 6.0f);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        vDest->AddNormal(c);
        
        if(m_Parent) m_Parent->AddDebugVerticesToMesh(vDest);
    }
    void SceneOctree::ComputeVisibleEntities(Vector4 *Planes,RenderList *VisibleEntityList,i32& VisibleLeafCount,bool SkipTest,RenderComponent* vDest)
    {
        if(!m_Children[0][0][0] && m_StaticEntities.size() + m_DynamicEntities.size() <= 0) return;
        
        if(SkipTest)
        {
            if(m_Children[0][0][0])
            {
                if(vDest && m_StaticEntities.size() + m_DynamicEntities.size() > 0) AddDebugVerticesToMesh(vDest);
                for(i32 i = 0;i < m_StaticEntities.size();i++) VisibleEntityList->AddEntity(m_StaticEntities[i]);
                for(i32 i = 0;i < m_DynamicEntities.size();i++) VisibleEntityList->AddEntity(m_DynamicEntities[i]);
                
                m_Children[LEFT ][TOP   ][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[LEFT ][TOP   ][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[LEFT ][BOTTOM][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[LEFT ][BOTTOM][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[RIGHT][TOP   ][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[RIGHT][TOP   ][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[RIGHT][BOTTOM][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[RIGHT][BOTTOM][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
            }
            else
            {
                if(vDest) AddDebugVerticesToMesh(vDest);
                for(i32 i = 0;i < m_StaticEntities.size();i++) VisibleEntityList->AddEntity(m_StaticEntities[i]);
                for(i32 i = 0;i < m_DynamicEntities.size();i++) VisibleEntityList->AddEntity(m_DynamicEntities[i]);
                
                VisibleLeafCount++;
                return;
            }
        }
        else if(m_Children[0][0][0])
        {
            u8 r = m_Volume.IntersectsFrustum(Planes);
            if(r == 1)
            {
                if(vDest && m_StaticEntities.size() + m_DynamicEntities.size() > 0) AddDebugVerticesToMesh(vDest);
                for(i32 i = 0;i < m_StaticEntities.size();i++) VisibleEntityList->AddEntity(m_StaticEntities[i]);
                for(i32 i = 0;i < m_DynamicEntities.size();i++) VisibleEntityList->AddEntity(m_DynamicEntities[i]);
                
                //Intersecting
                m_Children[LEFT ][TOP   ][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,false,vDest);
                m_Children[LEFT ][TOP   ][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,false,vDest);
                m_Children[LEFT ][BOTTOM][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,false,vDest);
                m_Children[LEFT ][BOTTOM][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,false,vDest);
                m_Children[RIGHT][TOP   ][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,false,vDest);
                m_Children[RIGHT][TOP   ][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,false,vDest);
                m_Children[RIGHT][BOTTOM][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,false,vDest);
                m_Children[RIGHT][BOTTOM][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,false,vDest);
            }
            else if(r == 2)
            {
                if(vDest && m_StaticEntities.size() + m_DynamicEntities.size() > 0) AddDebugVerticesToMesh(vDest);
                for(i32 i = 0;i < m_StaticEntities.size();i++) VisibleEntityList->AddEntity(m_StaticEntities[i]);
                for(i32 i = 0;i < m_DynamicEntities.size();i++) VisibleEntityList->AddEntity(m_DynamicEntities[i]);
                
                //Completely inside
                m_Children[LEFT ][TOP   ][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[LEFT ][TOP   ][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[LEFT ][BOTTOM][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[LEFT ][BOTTOM][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[RIGHT][TOP   ][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[RIGHT][TOP   ][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[RIGHT][BOTTOM][FAR ]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
                m_Children[RIGHT][BOTTOM][NEAR]->ComputeVisibleEntities(Planes,VisibleEntityList,VisibleLeafCount,true,vDest);
            }
            else return;
        }
        else
        {
            if(vDest) AddDebugVerticesToMesh(vDest);
            for(i32 i = 0;i < m_StaticEntities.size();i++) VisibleEntityList->AddEntity(m_StaticEntities[i]);
            for(i32 i = 0;i < m_DynamicEntities.size();i++) VisibleEntityList->AddEntity(m_DynamicEntities[i]);
            
            VisibleLeafCount++;
        }
    }
    void DefaultFrustumCullingAlgorithm::Init()
    {
        if(m_Octree)
        {
            delete m_Octree;
        }
        m_Octree = new SceneOctree();
        m_Octree->BuildOctree(Engine::GetEngine()->GetRenderSystem()->GetWorldSize(),Vector3(0,0,0),6);
        
        for(i32 i = 0;i < m_EntityList->size();i++) m_Octree->AddEntity((*m_EntityList)[i]);
    }
    void DefaultFrustumCullingAlgorithm::OnEntityAdd(Entity* E)
    {
        CullingAlgorithm::OnEntityAdd(E);
        m_Octree->AddEntity(E);
    }
    void DefaultFrustumCullingAlgorithm::OnEntityRemove(Entity* E)
    {
        CullingAlgorithm::OnEntityRemove(E);
        m_Octree->RemoveEntity(E);
    }
    /*
    void DefaultFrustumCullingAlgorithm::ComputeVisibleEntities(Entity* Camera,RenderList* VisibleEntityList)
    {
        CameraComponent* c = (CameraComponent*)Camera->GetComponentByType(CT_CAMERA);
        TransformComponent* t = (TransformComponent*)Camera->GetComponentByType(CT_TRANSFORM);
        
        Matrix4 pv = c->GetProjection() * t->GetTransform().Inverse();
        ClipPlanes[0].x = pv.w.x + pv.x.x;
        ClipPlanes[0].y = pv.w.y + pv.x.y;
        ClipPlanes[0].z = pv.w.z + pv.x.z;
        ClipPlanes[0].w = pv.w.w + pv.x.w;
        ClipPlanes[1].x = pv.w.x - pv.x.x;
        ClipPlanes[1].y = pv.w.y - pv.x.y;
        ClipPlanes[1].z = pv.w.z - pv.x.z;
        ClipPlanes[1].w = pv.w.w - pv.x.w;
        ClipPlanes[2].x = pv.w.x - pv.y.x;
        ClipPlanes[2].y = pv.w.y - pv.y.y;
        ClipPlanes[2].z = pv.w.z - pv.y.z;
        ClipPlanes[2].w = pv.w.w - pv.y.w;
        ClipPlanes[3].x = pv.w.x + pv.y.x;
        ClipPlanes[3].y = pv.w.y + pv.y.y;
        ClipPlanes[3].z = pv.w.z + pv.y.z;
        ClipPlanes[3].w = pv.w.w + pv.y.w;
        ClipPlanes[4].x = pv.w.x + pv.z.x;
        ClipPlanes[4].y = pv.w.y + pv.z.y;
        ClipPlanes[4].z = pv.w.z + pv.z.z;
        ClipPlanes[4].w = pv.w.w + pv.z.w;
        ClipPlanes[5].x = pv.w.x - pv.z.x;
        ClipPlanes[5].y = pv.w.y - pv.z.y;
        ClipPlanes[5].z = pv.w.z - pv.z.z;
        ClipPlanes[5].w = pv.w.w - pv.z.w;
        
        ClipPlanes[0] /= ClipPlanes[0].xyz().Magnitude();
        ClipPlanes[1] /= ClipPlanes[1].xyz().Magnitude();
        ClipPlanes[2] /= ClipPlanes[2].xyz().Magnitude();
        ClipPlanes[3] /= ClipPlanes[3].xyz().Magnitude();
        ClipPlanes[4] /= ClipPlanes[4].xyz().Magnitude();
        ClipPlanes[5] /= ClipPlanes[5].xyz().Magnitude();
        
        RenderComponent* r = m_DebugDisplay ? (RenderComponent*)m_DebugDisplay->GetComponentByType(CT_RENDER) : 0;
        if(r)
        {
            r->Clear();
            r->SetPrimType(GL_LINES);
            VisibleEntityList->AddEntity(m_DebugDisplay);
        }
        
        m_VisibleLeaves = 0;
        m_Octree->Update();
        m_Octree->ComputeVisibleEntities(ClipPlanes,VisibleEntityList,m_VisibleLeaves,false,r);
        //Log("Visible leaves: %d\n",m_VisibleLeaves);
    }
    */
    bool DefaultFrustumCullingAlgorithm::CreateTask()
    {
        return false;
    }
    void DefaultFrustumCullingAlgorithm::ToggleDebugDisplay()
    {
        if(!m_DebugDisplay) m_DebugDisplay = Engine::GetEngine()->SpawnEntity("Entities/Test/DebugOctreeDisplay.plst");
        else { Engine::GetEngine()->DestroyEntity(m_DebugDisplay); m_DebugDisplay = 0; }
    }
    void BruteForceCullTask::Run()
    {
        if(!EntityArrayPointer || !FlagArrayPointer || !Planes) return;
        for(i32 i = 0;i < Count;i++)
        {
            RenderComponent* r = GetRenderComponent(EntityArrayPointer[i]);
            if(r->StayVisible()) { FlagArrayPointer[i] = true; continue; }
            
            TransformComponent* t = GetTransformComponent(EntityArrayPointer[i]);
            
            if(r->GetMesh()->HasChanged()) { r->AcquireDimensions(); t->SetDimensions(r->GetDimensions()); }
            
            if(t->GetBoundingVolume().IntersectsFrustum(Planes)) FlagArrayPointer[i] = true;
            else FlagArrayPointer[i] = false;
        }
    }
    void BruteForceFrustumCullingAlgorithm::Init()
    {
    }
    bool BruteForceFrustumCullingAlgorithm::CreateTask()
    {
        Entity* Camera = Engine::GetEngine()->GetCurrentCamera();
        
        CameraComponent* c = (CameraComponent*)Camera->GetComponentByType(CT_CAMERA);
        TransformComponent* t = (TransformComponent*)Camera->GetComponentByType(CT_TRANSFORM);
        
        if(t->DidMove() || t->DidRotate())
        {
            Matrix4 pv = c->GetProjection() * t->GetTransform().Inverse();
            ClipPlanes[0].x = pv.w.x + pv.x.x;
            ClipPlanes[0].y = pv.w.y + pv.x.y;
            ClipPlanes[0].z = pv.w.z + pv.x.z;
            ClipPlanes[0].w = pv.w.w + pv.x.w;
            ClipPlanes[1].x = pv.w.x - pv.x.x;
            ClipPlanes[1].y = pv.w.y - pv.x.y;
            ClipPlanes[1].z = pv.w.z - pv.x.z;
            ClipPlanes[1].w = pv.w.w - pv.x.w;
            ClipPlanes[2].x = pv.w.x - pv.y.x;
            ClipPlanes[2].y = pv.w.y - pv.y.y;
            ClipPlanes[2].z = pv.w.z - pv.y.z;
            ClipPlanes[2].w = pv.w.w - pv.y.w;
            ClipPlanes[3].x = pv.w.x + pv.y.x;
            ClipPlanes[3].y = pv.w.y + pv.y.y;
            ClipPlanes[3].z = pv.w.z + pv.y.z;
            ClipPlanes[3].w = pv.w.w + pv.y.w;
            ClipPlanes[4].x = pv.w.x + pv.z.x;
            ClipPlanes[4].y = pv.w.y + pv.z.y;
            ClipPlanes[4].z = pv.w.z + pv.z.z;
            ClipPlanes[4].w = pv.w.w + pv.z.w;
            ClipPlanes[5].x = pv.w.x - pv.z.x;
            ClipPlanes[5].y = pv.w.y - pv.z.y;
            ClipPlanes[5].z = pv.w.z - pv.z.z;
            ClipPlanes[5].w = pv.w.w - pv.z.w;
            
            ClipPlanes[0] /= ClipPlanes[0].xyz().Magnitude();
            ClipPlanes[1] /= ClipPlanes[1].xyz().Magnitude();
            ClipPlanes[2] /= ClipPlanes[2].xyz().Magnitude();
            ClipPlanes[3] /= ClipPlanes[3].xyz().Magnitude();
            ClipPlanes[4] /= ClipPlanes[4].xyz().Magnitude();
            ClipPlanes[5] /= ClipPlanes[5].xyz().Magnitude();
        }
        
        /* Split into multiple tasks if possible */
        if(m_EntityList->size() > 100)
        {
            i32 TaskCount = floor(m_EntityList->size() / 100);
            for(i32 i = 0;i < TaskCount;i++)
            {
                BruteForceCullTask* Task = new BruteForceCullTask(m_ParentTask);
                Task->Count = 100;
                Task->EntityArrayPointer = &(*m_EntityList)[i * 100];
                Task->FlagArrayPointer   = (bool*)&VisibilityFlags[i * 100];
                Task->Planes             = ClipPlanes;
                Engine::GetEngine()->GetTaskManager()->AddTask(Task);
            }
            i32 Remainder = (i32)m_EntityList->size() - (TaskCount * 100);
            if(Remainder)
            {
                BruteForceCullTask* Task = new BruteForceCullTask(m_ParentTask);
                Task->Count = Remainder;
                Task->EntityArrayPointer = &(*m_EntityList)[TaskCount * 100];
                Task->FlagArrayPointer   = (bool*)&VisibilityFlags[TaskCount * 100];
                Task->Planes             = ClipPlanes;
                Engine::GetEngine()->GetTaskManager()->AddTask(Task);
            }
        }
        else
        {
            BruteForceCullTask* Task = new BruteForceCullTask(m_ParentTask);
            Task->Count = (i32)VisibilityFlags.size();
            Task->EntityArrayPointer = &(*m_EntityList)[0];
            Task->FlagArrayPointer   = (bool*)&VisibilityFlags[0];
            Task->Planes             = ClipPlanes;
            Engine::GetEngine()->GetTaskManager()->AddTask(Task);
        }
        return true;
    }
};