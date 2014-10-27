/*
 *  Mesh.cpp
 *
 *  Created by Michael DeCicco on 9/8/14.
 *
 */

#include "Mesh.h"

#include "assimp.hpp"
#include "aiPostProcess.h"
#include "aiScene.h"
using namespace Assimp;

namespace Reality
{
    aiNode* FindNode(const string& Name,aiNode* Root)
    {
        if(Root->mName == Name) return Root;
        
        for(i32 i = 0;i < Root->mNumChildren;i++)
        {
            aiNode* n = FindNode(Name,Root->mChildren[i]);
            if(n) return n;
        }
        return 0;
    }
    Mesh::Mesh() : m_TangBuff(0), m_NormBuff(0), m_TexCBuff(0), m_VertBuff(0), m_VAO(0)
    {
        for(i32 i = 0;i < MAX_TEXTURES;i++) m_Textures[i] = 0;
    }
    Mesh::~Mesh()
    {
        for(i32 i = 0;i < MAX_TEXTURES;i++)
        {
            if(m_Textures[i]) m_Textures[i]->Destroy();
        }
    }
    bool Mesh::Reload()
    {
        Assimp::Importer Importer;
        const aiScene* Scene = Importer.ReadFileFromMemory(GetPtr(),GetSize(),aiProcessPreset_TargetRealtime_Fast);
        SetPosition(0);
        if(!Scene) return false;
        
        if(Scene->HasMeshes())
        {
            if(Scene->mNumMeshes > 1) Log("Warning: <%s> contains multiple meshes, using first for entity.\n",m_Path.c_str());
            Clear();
            i32 mIdx = 0;
            aiMesh* Mesh = Scene->mMeshes[mIdx];
            bool HasSkel = Mesh->mNumBones != 0;
            
            for(i32 i = 0;i < Mesh->mNumVertices;i++)
            {
                if(Mesh->mVertices)
                {
                    aiVector3D pos    = Mesh->mVertices        [i];
                    AddVertex(*(Vector3*)&pos);
                }
                if(Mesh->mTextureCoords)
                {
                    aiVector3D uv     = Mesh->mTextureCoords[0][i];
                    AddTexCoord(Vector2(uv.x,uv.y));
                }
                if(Mesh->mNormals)
                {
                    aiVector3D normal = Mesh->mNormals         [i];
                    AddNormal(*(Vector3*)&normal);
                }
                if(Mesh->mTangents)
                {
                    aiVector3D tang   = Mesh->mTangents        [i];
                    AddTangent(*(Vector3*)&tang);
                }
                /*
                 * Placeholder values (because assimp doesn't
                 * know how to store the bone IDs and weights
                 * with the vertex information where it truly
                 * should be... Fuckin motherfuckers...
                 */
                if(HasSkel) { AddVertexBoneIndex4(-1,-1,-1,-1); AddVertexWeight4(-1,-1,-1,-1); }
            }
            
            for(u32 i = 0;i < Mesh->mNumFaces;i++)
            {
                const aiFace& face = Mesh->mFaces[i];
                
                AddIndex(face.mIndices[0]);
                AddIndex(face.mIndices[1]);
                AddIndex(face.mIndices[2]);
            }
            
            
            if(HasSkel)
            {
                /* Add bones to skeleton */
                for(i32 i = 0;i < Mesh->mNumBones;i++)
                {
                    aiBone* a_bn = Mesh->mBones[i];
                    string BoneName = a_bn->mName.data;
                    
                    Bone* b = new Bone(&m_Skeleton);
                    memcpy(&b->InverseBindPose.x.x,&a_bn->mOffsetMatrix.a1,16 * sizeof(Scalar));
                    
                    m_Skeleton.AddBone(b,BoneName);
                }
                
                /*
                 * Create bone heierarchy (because apparently assimp
                 * can not store this information with the bones...)
                 */
                for(i32 i = 0;i < Mesh->mNumBones;i++)
                {
                    aiBone* a_bn = Mesh->mBones[i];
                    string BoneName = a_bn->mName.data;
                    
                    Bone* b = m_Skeleton.GetBone(BoneName);
                    aiNode* bNode = FindNode(BoneName,Scene->mRootNode);
                    
                    if(bNode)
                    {
                        if(bNode->mParent)
                        {
                            b->m_ParentIdx = m_Skeleton.GetBoneIndex(bNode->mParent->mName.data);
                            if(b->m_ParentIdx != -1)
                            {
                                m_Skeleton.GetBone(b->m_ParentIdx)->m_Children.push_back(i);
                            }
                        }
                    }
                }
                
                bool WarnMaxBPV = false;
                for(i32 b = 0;b < Mesh->mNumBones;b++)
                {
                    for(i32 i = 0;i < Mesh->mBones[b]->mNumWeights;i++)
                    {
                        i32  VertexID = Mesh->mBones[b]->mWeights[i].mVertexId;
                        Scalar Weight = Mesh->mBones[b]->mWeights[i].mWeight;
                        
                        if     (GetBoneIndex0(VertexID) == -1) SetBoneIndex0(VertexID,b);
                        else if(GetBoneIndex1(VertexID) == -1) SetBoneIndex1(VertexID,b);
                        else if(GetBoneIndex2(VertexID) == -1) SetBoneIndex2(VertexID,b);
                        else if(GetBoneIndex3(VertexID) == -1) SetBoneIndex3(VertexID,b);
                        else WarnMaxBPV = true;
                        
                        if     (GetWeight0(VertexID) == -1) SetWeight0(VertexID,Weight);
                        else if(GetWeight1(VertexID) == -1) SetWeight1(VertexID,Weight);
                        else if(GetWeight2(VertexID) == -1) SetWeight2(VertexID,Weight);
                        else if(GetWeight3(VertexID) == -1) SetWeight3(VertexID,Weight);
                        else WarnMaxBPV = true;
                    }
                }
                m_IsAnimated = HasSkel;
                if(WarnMaxBPV) Log("Reality only supports 4 bones per vertex. Unexpected results will ensue.\n");
            }
            
            Importer.FreeScene();
            return true;
        }
        else Log("Error: No meshes in file <%s>.",m_Path.c_str());
        
        return false;
    }
    void Mesh::LoadFromAssimpMesh(aiMesh *Mesh)
    {
    }
    /* Attribute access */
    void Mesh::Clear()
    {
        m_VAO = 0;
        m_Vertices   .clear();
        m_TexCoords  .clear();
        m_Normals    .clear();
        m_Tangents   .clear();
        m_BoneIndices.clear();
        m_Weights    .clear();
    }
    void Mesh::AddIndex(u16 Idx)
    {
        m_Indices.push_back(Idx);
        m_IndicesUpdated = true;
    }
    void Mesh::SetIndex(i32 vIdx,u16 Idx)
    {
        m_Indices[vIdx] = Idx;
        m_IndicesUpdated = true;
    }
    u16 Mesh::GetIndex(i32 vIdx)
    {
        return m_Indices[vIdx];
    }
    void Mesh::AddVertex(const Vector3& v)
    {
        /* to do: edit data in gl buffers? glbuffersubdata or some shit */
        m_Vertices.push_back(v);
        m_VerticesUpdated = true;
        m_HasChanged = true;
    }
    void Mesh::SetVertex(i32 Idx,const Vector3& v)
    {
        m_Vertices[Idx] = v;
        m_VerticesUpdated = true;
        m_HasChanged = true;
    }
    Vector3 Mesh::GetVertex(i32 Idx) const
    {
        return m_Vertices[Idx];
    }

    void Mesh::AddTexCoord(const Vector2& t)
    {
        m_TexCoords.push_back(t);
        m_TexCoordsUpdated = true;
    }
    void Mesh::SetTexCoord(i32 Idx,const Vector2& t)
    {
        m_TexCoords[Idx] = t;
        m_TexCoordsUpdated = true;
    }
    Vector2 Mesh::GetTexCoord(i32 Idx) const
    {
        return m_TexCoords[Idx];
    }

    void Mesh::AddNormal(const Vector3& n)
    {
        m_Normals.push_back(n);
        m_NormalsUpdated = true;
    }
    void Mesh::SetNormal(i32 Idx,const Vector3& n)
    {
        m_Normals[Idx] = n;
        m_NormalsUpdated = true;
    }
    Vector3 Mesh::GetNormal(i32 Idx) const
    {
        return m_Normals[Idx];
    }

    void Mesh::AddTangent(const Vector3& t)
    {
        m_Tangents.push_back(t);
        m_TangentsUpdated = true;
    }
    void Mesh::SetTangent(i32 Idx,const Vector3& t)
    {
        m_Tangents.push_back(t);
        if(m_TangBuff)
        {
            glDeleteBuffers(1,&m_TangBuff);
            m_TangBuff = 0;
        }
        m_TangentsUpdated = true;
    }
    Vector3 Mesh::GetTangent(i32 Idx)
    {
        return m_Tangents[Idx];
    }
    void Mesh::AddVertexWeight1(f32 Weight)
    {
        m_Weights.push_back(Vector4(Weight,-1,-1,-1));
        m_WeightsUpdated = true;
    }
    void Mesh::AddVertexWeight2(f32 Weight0,f32 Weight1)
    {
        m_Weights.push_back(Vector4(Weight0,Weight1,-1,-1));
        m_WeightsUpdated = true;
    }
    void Mesh::AddVertexWeight3(f32 Weight0,f32 Weight1,f32 Weight2)
    {
        m_Weights.push_back(Vector4(Weight0,Weight1,Weight2,-1));
        m_WeightsUpdated = true;
    }
    void Mesh::AddVertexWeight4(f32 Weight0,f32 Weight1,f32 Weight2,f32 Weight3)
    {
        m_Weights.push_back(Vector4(Weight0,Weight1,Weight2,Weight3));
        m_WeightsUpdated = true;
    }
    void Mesh::SetWeight0(i32 Idx,f32 Weight)
    {
        m_Weights[Idx].x = Weight;
        m_WeightsUpdated = true;
    }
    void Mesh::SetWeight1(i32 Idx,f32 Weight)
    {
        m_Weights[Idx].y = Weight;
        m_WeightsUpdated = true;
    }
    void Mesh::SetWeight2(i32 Idx,f32 Weight)
    {
        m_Weights[Idx].z = Weight;
        m_WeightsUpdated = true;
    }
    void Mesh::SetWeight3(i32 Idx,f32 Weight)
    {
        m_Weights[Idx].w = Weight;
        m_WeightsUpdated = true;
    }
    f32 Mesh::GetWeight0(i32 Idx)
    {
        return m_Weights[Idx].x;
    }
    f32 Mesh::GetWeight1(i32 Idx)
    {
        return m_Weights[Idx].y;
    }
    f32 Mesh::GetWeight2(i32 Idx)
    {
        return m_Weights[Idx].z;
    }
    f32 Mesh::GetWeight3(i32 Idx)
    {
        return m_Weights[Idx].w;
    }

    void Mesh::AddVertexBoneIndex1(i32 Idx)
    {
        m_BoneIndices.push_back(iv4(Idx,-1,-1,-1));
        m_BoneIndicesUpdated = true;
    }
    void Mesh::AddVertexBoneIndex2(i32 Idx0,i32 Idx1)
    {
        m_BoneIndices.push_back(iv4(Idx0,Idx1,-1,-1));
        m_BoneIndicesUpdated = true;
    }
    void Mesh::AddVertexBoneIndex3(i32 Idx0,i32 Idx1,i32 Idx2)
    {
        m_BoneIndices.push_back(iv4(Idx0,Idx1,Idx2,-1));
        m_BoneIndicesUpdated = true;
    }
    void Mesh::AddVertexBoneIndex4(i32 Idx0,i32 Idx1,i32 Idx2,i32 Idx3)
    {
        m_BoneIndices.push_back(iv4(Idx0,Idx1,Idx2,Idx3));
        m_BoneIndicesUpdated = true;
    }
    void Mesh::SetBoneIndex0(i32 vIdx,i32 bIdx)
    {
        m_BoneIndices[vIdx].x = bIdx;
        m_BoneIndicesUpdated = true;
    }
    void Mesh::SetBoneIndex1(i32 vIdx,i32 bIdx)
    {
        m_BoneIndices[vIdx].y = bIdx;
        m_BoneIndicesUpdated = true;
    }
    void Mesh::SetBoneIndex2(i32 vIdx,i32 bIdx)
    {
        m_BoneIndices[vIdx].z = bIdx;
        m_BoneIndicesUpdated = true;
    }
    void Mesh::SetBoneIndex3(i32 vIdx,i32 bIdx)
    {
        m_BoneIndices[vIdx].w = bIdx;
        m_BoneIndicesUpdated = true;
    }
    i32 Mesh::GetBoneIndex0(i32 Idx)
    {
        return m_BoneIndices[Idx].x;
    }
    i32 Mesh::GetBoneIndex1(i32 Idx)
    {
        return m_BoneIndices[Idx].y;
    }
    i32 Mesh::GetBoneIndex2(i32 Idx)
    {
        return m_BoneIndices[Idx].z;
    }
    i32 Mesh::GetBoneIndex3(i32 Idx)
    {
        return m_BoneIndices[Idx].w;
    }

    i32 Mesh::GetVertexCount() const
    {
        return (i32)m_Vertices.size();
    }
    i32 Mesh::GetIndexCount() const
    {
        return (i32)m_Indices.size();
    }
    
    bool Mesh::NeedsSync() const
    {
        return m_VerticesUpdated || m_NormalsUpdated || m_TexCoordsUpdated || m_TangentsUpdated || m_IndicesUpdated;
    }
    void Mesh::SyncBuffers()
    {
        if(m_VerticesUpdated || (m_Vertices.size() == 0 && m_VertBuff))
        {
            glDeleteBuffers(1,&m_VertBuff);
            m_VertBuff = 0;
            m_VerticesUpdated = false;
        }
        if(m_TexCoordsUpdated || (m_TexCoords.size() == 0 && m_TexCBuff))
        {
            glDeleteBuffers(1,&m_TexCBuff);
            m_TexCBuff = 0;
            m_TexCoordsUpdated = false;
        }
        if(m_NormalsUpdated || (m_Normals.size() == 0 && m_NormBuff))
        {
            glDeleteBuffers(1,&m_NormBuff);
            m_NormBuff = 0;
            m_NormalsUpdated = false;
        }
        if(m_TangentsUpdated || (m_Tangents.size() == 0 && m_TangBuff))
        {
            glDeleteBuffers(1,&m_TangBuff);
            m_TangBuff = 0;
            m_TangentsUpdated = false;
        }
        if(m_BoneIndicesUpdated || (m_BoneIndices.size() == 0 && m_BoneIdxBuff))
        {
            glDeleteBuffers(1,&m_BoneIdxBuff);
            m_BoneIdxBuff = 0;
            m_BoneIndicesUpdated = false;
        }
        if(m_WeightsUpdated || (m_Weights.size() == 0 && m_WeightBuff))
        {
            glDeleteBuffers(1,&m_WeightBuff);
            m_WeightBuff = 0;
            m_WeightsUpdated = false;
        }
        if(m_VertBuff + m_TexCBuff + m_NormBuff + m_TangBuff + m_BoneIdxBuff + m_WeightBuff == 0 && m_VAO)
        {
            glDeleteVertexArrays(1,&m_VAO);
            m_VAO = 0;
            if(m_Vertices.size() + m_TexCoords.size() + m_Normals.size() + m_Tangents.size() + m_Weights.size() + m_BoneIndices.size() == 0) return;
        }
    
        if(!m_VAO) glGenVertexArrays(1,&m_VAO);
        glBindVertexArray(m_VAO);
        
        if(!m_IndexBuff && m_Indices.size() > 0)
        {
            glGenBuffers(1,&m_IndexBuff);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_IndexBuff);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_Indices.size() * sizeof(u16),&m_Indices[0],GL_STATIC_DRAW);
        }
        if(!m_VertBuff && m_Vertices.size() > 0)
        {
            glGenBuffers(1,&m_VertBuff);
            glBindBuffer(GL_ARRAY_BUFFER,m_VertBuff);
            glBufferData(GL_ARRAY_BUFFER,m_Vertices.size() * sizeof(Vector3),&m_Vertices[0],GL_STATIC_DRAW);
            glEnableVertexAttribArray(VA_POSITION);
            glVertexAttribPointer(VA_POSITION,3,GL_FLOAT,GL_FALSE,0,0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
        if(!m_NormBuff && m_Normals.size() > 0)
        {
            glGenBuffers(1,&m_NormBuff);
            glBindBuffer(GL_ARRAY_BUFFER,m_NormBuff);
            glBufferData(GL_ARRAY_BUFFER,m_Normals.size() * sizeof(Vector3),&m_Normals[0],GL_STATIC_DRAW);
            glEnableVertexAttribArray(VA_NORMAL);
            glVertexAttribPointer(VA_NORMAL,3,GL_FLOAT,GL_FALSE,0,0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
        if(!m_TangBuff && m_Tangents.size() > 0)
        {
            glGenBuffers(1,&m_TangBuff);
            glBindBuffer(GL_ARRAY_BUFFER,m_TangBuff);
            glBufferData(GL_ARRAY_BUFFER,m_Tangents.size() * sizeof(Vector3),&m_Tangents[0],GL_STATIC_DRAW);
            glEnableVertexAttribArray(VA_TANGENT);
            glVertexAttribPointer(VA_TANGENT,3,GL_FLOAT,GL_FALSE,0,0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
        if(!m_TexCBuff && m_TexCoords.size() > 0)
        {
            glGenBuffers(1,&m_TexCBuff);
            glBindBuffer(GL_ARRAY_BUFFER,m_TexCBuff);
            glBufferData(GL_ARRAY_BUFFER,m_TexCoords.size() * sizeof(Vector2),&m_TexCoords[0],GL_STATIC_DRAW);
            glEnableVertexAttribArray(VA_TEXCOORD);
            glVertexAttribPointer(VA_TEXCOORD,2,GL_FLOAT,GL_FALSE,0,0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
        if(!m_BoneIdxBuff && m_BoneIndices.size() > 0)
        {
            glGenBuffers(1,&m_BoneIdxBuff);
            glBindBuffer(GL_ARRAY_BUFFER,m_BoneIdxBuff);
            glBufferData(GL_ARRAY_BUFFER,m_BoneIndices.size() * sizeof(iv4),&m_BoneIndices[0],GL_STATIC_DRAW);
            glEnableVertexAttribArray(VA_BONEIDX);
            glVertexAttribIPointer(VA_BONEIDX,4,GL_INT,sizeof(iv4),0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
        if(!m_WeightBuff && m_Weights.size() > 0)
        {
            glGenBuffers(1,&m_WeightBuff);
            glBindBuffer(GL_ARRAY_BUFFER,m_WeightBuff);
            glBufferData(GL_ARRAY_BUFFER,m_Weights.size() * sizeof(Vector4),&m_Weights[0],GL_STATIC_DRAW);
            glEnableVertexAttribArray(VA_WEIGHT);
            glVertexAttribPointer(VA_WEIGHT,4,GL_FLOAT,GL_FALSE,0,0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
    }
    void Mesh::SetTexParam(i32 TexNum,GLenum Param,GLint Value)
    {
        if(!m_Textures[TexNum])
        {
            Log("Error setting texture parameter. Texture[%d] does not exist.\n",TexNum);
            return;
        }
        m_TexParams.push_back({TexNum,(i32)Param,Value});
    }
    void Mesh::GetMinMaxVertices(Vector3& Min,Vector3& Max)
    {
        if(m_Vertices.size() == 0) return;
        Min = Max = m_Vertices[0];
        for(i32 i = 0;i < m_Vertices.size();i++)
        {
            if(m_Vertices[i].x < Min.x) Min.x = m_Vertices[i].x;
            if(m_Vertices[i].y < Min.y) Min.y = m_Vertices[i].y;
            if(m_Vertices[i].z < Min.z) Min.z = m_Vertices[i].z;
            if(m_Vertices[i].x > Max.x) Max.x = m_Vertices[i].x;
            if(m_Vertices[i].y > Max.y) Max.y = m_Vertices[i].y;
            if(m_Vertices[i].z > Max.z) Max.z = m_Vertices[i].z;
        }
    }
};