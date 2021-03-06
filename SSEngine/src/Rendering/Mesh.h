#ifndef SandboxSimulator_Mesh_h
#define SandboxSimulator_Mesh_h

#include <Rendering/Texture.h>
#include <Rendering/Shader.h>

namespace SandboxSimulator {
class Mesh : public Asset
{
    public:
        Mesh();
        ~Mesh();
    
        virtual bool OnLoad();
        //void LoadFromAssimpMesh(aiMesh* Mesh);
    
        bool IsAnimated() const { return false; }//m_IsAnimated; }
        //const Skeleton* GetSkeleton() const { return &m_Skeleton; }
        
        /* Attribute access */
        void Clear();
        
        void AddIndex(u16 Idx);
        void SetIndex(i32 vIdx,u16 Idx);
        u16 GetIndex(i32 vIdx);
    
        void AddVertex(const Vec3& v);
        void SetVertex(i32 Idx,const Vec3& v);
        Vec3 GetVertex(i32 Idx) const;
    
        void AddTexCoord(const Vec2& t);
        void SetTexCoord(i32 Idx,const Vec2& t);
        Vec2 GetTexCoord(i32 Idx) const;
    
        void AddNormal(const Vec3& n);
        void SetNormal(i32 Idx,const Vec3& n);
        Vec3 GetNormal(i32 Idx) const;
    
        void AddTangent(const Vec3& t);
        void SetTangent(i32 Idx,const Vec3& t);
        Vec3 GetTangent(i32 Idx);
    
        void AddVertexWeight1(f32 Weight);
        void AddVertexWeight2(f32 Weight0,f32 Weight1);
        void AddVertexWeight3(f32 Weight0,f32 Weight1,f32 Weight2);
        void AddVertexWeight4(f32 Weight0,f32 Weight1,f32 Weight2,f32 Weight3);
        void SetWeight0(i32 Idx,f32 Weight);
        void SetWeight1(i32 Idx,f32 Weight);
        void SetWeight2(i32 Idx,f32 Weight);
        void SetWeight3(i32 Idx,f32 Weight);
        f32 GetWeight0(i32 Idx);
        f32 GetWeight1(i32 Idx);
        f32 GetWeight2(i32 Idx);
        f32 GetWeight3(i32 Idx);
    
        void AddVertexBoneIndex1(i32 Idx);
        void AddVertexBoneIndex2(i32 Idx0,i32 Idx1);
        void AddVertexBoneIndex3(i32 Idx0,i32 Idx1,i32 Idx2);
        void AddVertexBoneIndex4(i32 Idx0,i32 Idx1,i32 Idx2,i32 Idx3);
        void SetBoneIndex0(i32 vIdx,i32 bIdx);
        void SetBoneIndex1(i32 vIdx,i32 bIdx);
        void SetBoneIndex2(i32 vIdx,i32 bIdx);
        void SetBoneIndex3(i32 vIdx,i32 bIdx);
        i32 GetBoneIndex0(i32 Idx);
        i32 GetBoneIndex1(i32 Idx);
        i32 GetBoneIndex2(i32 Idx);
        i32 GetBoneIndex3(i32 Idx);
    
        i32 GetVertexCount() const;
        i32 GetIndexCount() const;
        GLuint GetVAO() const { return m_VAO; }
        bool NeedsSync() const;
        bool HasChanged() { if(m_HasChanged) { m_HasChanged = false; return true; } return false; }
    
        void SyncBuffers();
    
        void SetTexParam(i32 TexNum,GLenum Param,GLint Value);
        
        void GetMinMaxVertices(Vec3& Min,Vec3& Max);
    
        std::vector<iv3>     m_TexParams;
        Texture* m_Textures[MAX_TEXTURES];
    
    protected:
        friend class RenderSystem;
        friend class RenderComponent;
        std::vector<u16> m_Indices;
        bool m_IndicesUpdated;
        std::vector<Vec3> m_Vertices;
        bool m_VerticesUpdated;
        std::vector<Vec3> m_Normals;
        bool m_NormalsUpdated;
        std::vector<Vec3> m_Tangents;
        bool m_TangentsUpdated;
        std::vector<Vec2> m_TexCoords;
        bool m_TexCoordsUpdated;
        //std::vector<iv4> m_BoneIndices;
        //bool m_BoneIndicesUpdated;
        //std::vector<Vec4> m_Weights;
        //bool m_WeightsUpdated;
        bool m_HasChanged;
    
    
        //bool m_IsAnimated;
        //Skeleton m_Skeleton;
    
        GLuint m_IndexBuff;
        GLuint m_VertBuff;
        GLuint m_NormBuff;
        GLuint m_TangBuff;
        GLuint m_TexCBuff;
        //GLuint m_BoneIdxBuff;
        //GLuint m_WeightBuff;
    
        GLuint m_VAO;
};
}

#endif
