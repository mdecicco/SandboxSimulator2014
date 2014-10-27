/*
 *  RenderSystem.h
 *
 *  Created by Michael DeCicco on 7/2/14.
 *
 */

#pragma once
#include "EngineSystem.h"
#include "EngineMath.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "CullingAlgorithm.h"
#include "RenderingAlgorithm.h"
#include "GUIManager.h"
#include "glfw3.h"

#include "btIDebugDraw.h"
#include <list>
using namespace std;

class btRigidBody;
namespace Reality
{
    class NestedCoordinateSystem;
    class PNode;
    class CameraComponent;
    class TransformComponent;
    class RenderList;
    
    class RenderComponent : public Component
    {
        public:
            RenderComponent();
            ~RenderComponent();
        
            void LoadMesh(const string& File);
        
            void SetGUIFlag(bool Flag);
            bool IsGUI() const { return m_GUIData != 0; }
        
            /* Attribute access */
            void Clear() { m_Mesh->Clear(); }
            
            void AddVertex(const Vector3& v) { m_Mesh->AddVertex(v); }
            void SetVertex(i32 Idx,const Vector3& v) { m_Mesh->SetVertex(Idx,v); }
            Vector3 GetVertex(i32 Idx) const { return m_Mesh->GetVertex(Idx); }
        
            void AddTexCoord(const Vector2& t) { m_Mesh->AddTexCoord(t); }
            void SetTexCoord(i32 Idx,const Vector2& t) { m_Mesh->SetTexCoord(Idx,t); }
            Vector2 GetTexCoord(i32 Idx) const { return m_Mesh->GetTexCoord(Idx); }
        
            void AddNormal(const Vector3& n) { m_Mesh->AddNormal(n); }
            void SetNormal(i32 Idx,const Vector3& n) { m_Mesh->SetNormal(Idx,n); }
            Vector3 GetNormal(i32 Idx) const { return m_Mesh->GetNormal(Idx); }
        
            void AddTangent(const Vector3& t) { m_Mesh->AddTangent(t); }
            void SetTangent(i32 Idx,const Vector3& t) { m_Mesh->SetTangent(Idx,t); }
            Vector3 GetTangent(i32 Idx) { return m_Mesh->GetTangent(Idx); }
        
            void SetPrimType(i32 Type) { m_PrimType = Type; }
            i32 GetPrimType() const { return m_PrimType; }
        
            i32 GetVertexCount() const { return m_Mesh->GetVertexCount(); }
        
            void SyncBuffers() { m_Mesh->SyncBuffers(); }
        
            void SetTexParam(i32 TexNum,GLenum Param,GLint Value) { m_Mesh->SetTexParam(TexNum,Param,Value); }
        
            SceneOctree* GetOctreeNode() const { return m_OctreeNode; }
            void AcquireDimensions() { if(!m_Mesh) return; Vector3 min,max; m_Mesh->GetMinMaxVertices(min,max); m_Dimensions = max - min; }
            Vector3 GetDimensions() const { return m_Dimensions; }
        
            Mesh* GetMesh() const { return m_Mesh; }
        
            bool StayVisible() const { return m_StayVisible; }
        
        protected:
            /* Does having this many friend classes mean that there are code design flaws? */
            friend class RenderSystem;
            friend class RenderList;
            friend class SceneOctree;
            friend class GUIManager;
            bool m_StayVisible;
            Entity* m_Entity;
            GUIElementData* m_GUIData;
            SceneOctree* m_OctreeNode;
            i32 m_OctreeIndex;
            Vector3 m_Dimensions;
            Mesh* m_Mesh;
            Shader* m_Shdr;
            i32 m_ShdrListPos;
            i32 m_PrimType;
    };
    
    class TransformComponent : public Component
    {
        public:
            TransformComponent(Entity* Ent);
            ~TransformComponent();
        
            bool IsStatic() const { return m_IsStatic; }
            bool DidMove() { bool tmp = m_DidMove; m_DidMove = false; return tmp; }
            bool DidRotate() { bool tmp = m_DidRotate; m_DidRotate = false; return tmp; }
        
            void Identity();
            void Translate(const Vector3& t);
            void Translate(Scalar x,Scalar y,Scalar z);
            void Rotate(const Quaternion& q);
            void Rotate(const Vector3& Axis,Scalar Angle);
            void Rotate(Scalar Ax,Scalar Ay,Scalar Az,Scalar Angle);
            void Scale(Scalar s);
            void Scale(Scalar x,Scalar y,Scalar z);
            void Scale(const Vector3& s);
            Vector3 GetPosition() const { return m_Position; }
            Vector3 GetScale() const { return m_Scale; }
            Quaternion GetOrientation() const { return m_Orientation; }
        
            const Matrix4& GetTransform();
            const Matrix4& GetNormalMatrix();
        
            void SetDimensions(const Vector3& Dimensions) { m_Volume.Dimensions = Dimensions; }
            const BoundingVolume& GetBoundingVolume() const { return m_Volume; }
            BoundingVolume& GetBoundingVolume() { return m_Volume; }
        
            void SetBody(btRigidBody* Body) { m_Body = Body; }
            btRigidBody* GetBody() const { return m_Body; }
        
        protected:
            friend class Engine;
            friend class PhysicsSystem;
            friend class Entity;
            BoundingVolume m_Volume;
            btRigidBody* m_Body;
            Vector3 m_Position;
            Vector3 m_Scale;
            Quaternion m_Orientation;
            bool m_Updated;
            bool m_IsStatic;
            bool m_DidMove;
            bool m_DidRotate;
            Matrix4 m_Transform;
            Matrix4 m_NormalMatrix;
            Entity* m_Entity;
    };
    
    class CameraComponent : public Component
    {
        public:
            CameraComponent();
            ~CameraComponent();
        
            Vector2 m_FieldOfView;
            Scalar m_NearPlane;
            Scalar m_FarPlane;
            Scalar m_Exposure;
            Scalar m_Speed;
            bool m_UpdateProjection;
        
            const Matrix4& GetProjection();
        
        protected:
            Matrix4 m_Projection;
    };
    
    class RenderSystem : public EngineSystem, btIDebugDraw
    {
        public:
            RenderSystem(PNode* GfxSettings);
            ~RenderSystem();
        
            virtual void HandleMessage(const EngineMessage* Msg);
        
            void SetCullingAlgorithm(CullingAlgorithm* Algo) { m_CullingAlgorithm = Algo; }
            CullingAlgorithm* GetCullingAlgorithm() const { return m_CullingAlgorithm; }
            void ToggleDebugCullDisplay() { m_CullingAlgorithm->ToggleDebugDisplay(); }
            void ToggleDebugPhysDisplay() { m_RenderDebugPhys = !m_RenderDebugPhys; }
            void SetRenderingAlgorithm(RenderingAlgorithm* Algo) { m_RenderingAlgorithm = Algo; }
            RenderingAlgorithm* GetRenderingAlgorithm() const { return m_RenderingAlgorithm; }
        
            Entity* GetCurrentCamera() const { m_Camera->AddRef(); return m_Camera; }
        
            void SetWorldSize(Scalar Size) { m_WorldSize = Size; }
            Scalar GetWorldSize() const { return m_WorldSize; }
        
            Scalar GetFPS() const { return m_FPS; }
            bool IsRendering() const { return m_IsRendering; }
            i32 GetVisibleEntityCount() const { return m_VisibleEntityCount; }
        
            Vector3 GetSunPosition() const;
        
            virtual void Initialize();
            virtual void Update(Scalar dt);
            virtual void Shutdown();
        
            virtual void Serialize();
            virtual void Deserialize();
        
            void CreateRenderTask();
        
            void Render(RenderList& r);
            void AddGUI(Entity* E);
            void RemoveGUI(Entity* E);
            const Matrix4& GetGUIProj() const { return m_GUIProj; }
            const Matrix4& GetGUIView() const { return m_GUIView; }
            GUIManager* GetGUIManager() const { return m_GUIManager; }
        
            GLFWwindow* GetWindow() const { return m_Window; }
            Vector2 GetResolution() const { return m_Resolution; }
        
            virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
            virtual	void drawTriangle(const btVector3& v0,const btVector3& v1,const btVector3& v2,const btVector3& color, btScalar /*alpha*/);
            virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
            virtual void reportErrorWarning(const char* warningString) { Log("Bullet: %s\n",warningString); }
            virtual void draw3dText(const btVector3& location,const char* textString) { }
            virtual void setDebugMode(int debugMode) { m_PhysDebugMode = debugMode; }
            virtual int	 getDebugMode() const { return m_PhysDebugMode; }
    
        protected:
            GLFWwindow* m_Window;
            GUIManager* m_GUIManager;
            CullingAlgorithm* m_CullingAlgorithm;
            RenderingAlgorithm* m_RenderingAlgorithm;
            RenderList* m_VisibleEntityList;
            RenderList* m_GUIList;
            Shader* m_PhysShader;
            Entity* m_Camera;
            Matrix4 m_GUIProj;
            Matrix4 m_GUIView;
            Matrix4 m_PhysMVP;
            Scalar m_WorldSize;
            Vector2 m_Resolution;
            bool m_Fullscreen;
            bool m_RenderDebugPhys;
            i32 m_PhysDebugMode;
            Scalar m_FPS;
            bool m_IsRendering;
        
            i32 m_VisibleEntityCount;
        
            vector<Entity*> m_RenderableEntities;
            vector<Entity*> m_AlwaysVisibleEntities;
    };
};