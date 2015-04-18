#ifndef SandboxSimulator_RenderSystem_h
#define SandboxSimulator_RenderSystem_h

#include <Core/EngineSystem.h>
#include <Rendering/Shader.h>
#include <Utils/Math.h>
#include <list>

#include <Rendering/CameraComponent.h>
#include <Rendering/Mesh.h>
#include <Rendering/RenderAlgorithm.h>

#include <Rendering/Gui/Font.h>
#include <Rendering/Gui/Gui.h>

using namespace std;
namespace SandboxSimulator
{
    enum RC_SHAPES
    {
        RC_NONE,
        RC_TRIANGLE,
        RC_SQUARE,
        RC_BOX,
        RC_SPHERE,
        RC_SKY_SPHERE,
        RC_GROUND_PLANE,
        RC_UI,
        RC_POINT_SPRITE,
        RC_COUNT
    };

    class RenderComponent : public Component
    {
        public:
            RenderComponent();
            ~RenderComponent();

            void LoadMesh(const string& File);
        
            void SetGUIFlag(bool Flag);
        
            /* Attribute access */
            void Clear() { m_Mesh->Clear(); }
            
            void AddVertex(const Vec3& v) { m_Mesh->AddVertex(v); }
            void SetVertex(i32 Idx,const Vec3& v) { m_Mesh->SetVertex(Idx,v); }
            Vec3 GetVertex(i32 Idx) const { return m_Mesh->GetVertex(Idx); }
        
            void AddTexCoord(const Vec2& t) { m_Mesh->AddTexCoord(t); }
            void SetTexCoord(i32 Idx,const Vec2& t) { m_Mesh->SetTexCoord(Idx,t); }
            Vec2 GetTexCoord(i32 Idx) const { return m_Mesh->GetTexCoord(Idx); }
        
            void AddNormal(const Vec3& n) { m_Mesh->AddNormal(n); }
            void SetNormal(i32 Idx,const Vec3& n) { m_Mesh->SetNormal(Idx,n); }
            Vec3 GetNormal(i32 Idx) const { return m_Mesh->GetNormal(Idx); }
        
            void AddTangent(const Vec3& t) { m_Mesh->AddTangent(t); }
            void SetTangent(i32 Idx,const Vec3& t) { m_Mesh->SetTangent(Idx,t); }
            Vec3 GetTangent(i32 Idx) { return m_Mesh->GetTangent(Idx); }

            void SetPrimType(i32 Type) { m_PrimType = Type; }
            i32 GetPrimType() const { return m_PrimType; }
        
            i32 GetVertexCount() const { return m_Mesh->GetVertexCount(); }
        
            void SyncBuffers() { m_Mesh->SyncBuffers(); }
        
            void SetTexParam(i32 TexNum,GLenum Param,GLint Value) { m_Mesh->SetTexParam(TexNum,Param,Value); }
        
            void AcquireDimensions() { if(!m_Mesh) return; Vec3 min,max; m_Mesh->GetMinMaxVertices(min,max); m_Dimensions = max - min; }
            Vec3 GetDimensions() const { return m_Dimensions; }
        
            void SetScissorRegion(const Vec2& Pos,const Vec2& Size) { m_ScissorPos = Pos; m_ScissorSize = Size; }
            Vec2 GetScissorPos() const { return m_ScissorPos; }
            Vec2 GetScissorSize() const { return m_ScissorSize; }
            void SetScissor(bool Flag) { m_UseScissorRegion = Flag; }
            bool UseScissor  ()       const { return m_UseScissorRegion; }
            bool UseBlending ()       const { return m_UseBlending;      }
            bool UseDepthTest()       const { return m_UseDepthTest;     }
            bool DepthWritesEnabled() const { return m_WriteDepth;       }
            void SetOpacity(Scalar Opacity) { m_Opacity = Opacity;       }
            Scalar GetOpacity(bool Relative = false) const { return m_Opacity; };
        
            Mesh* GetMesh() const { return m_Mesh; }
        
            bool StayVisible() const { return m_StayVisible; }
            bool IsHidden() const { return m_Hide; };
            void ToggleHide() { m_Hide = !m_Hide; }

            Shader* GetShader() { return m_Shdr; }
            void SetShape(RC_SHAPES Shape);

            void SetFont(Font* Fnt) { m_Font = Fnt; }
            Font* GetFont() const { return m_Font; }
            void SetIsGui(bool isg) { m_IsGui = isg; }
            bool IsGUI() const { return m_IsGui; }

            void SetMesh(Mesh* mesh) { if(m_Mesh) m_Mesh->Destroy(); m_Mesh = mesh; m_Mesh->AddRef(); }

            void ReloadShader() { m_Shdr->m_Loaded = false;m_Shdr->Load(m_Shdr->GetPath()); }

        protected:
            friend class RenderSystem;
            friend class RenderList;

            bool m_StayVisible;
            bool m_Hide;
            UID m_IsCulled;
            i32 m_OctreeIndex;
            Vec3 m_Dimensions;
            i32 m_PrimType;
            Mesh* m_Mesh;
            Shader* m_Shdr;
            i32 m_ShdrListPos;
            bool m_UseScissorRegion;
            Vec2 m_ScissorPos;
            Vec2 m_ScissorSize;
            bool m_UseBlending;
            bool m_UseDepthTest;
            bool m_WriteDepth;
            Scalar m_Opacity;

            bool m_NeedsUpdate;
            RC_SHAPES m_Shape;

            bool m_IsGui;
            Font* m_Font;
    };

	class RenderSystem : public EngineSystem
	{
		public:
			RenderSystem();
			~RenderSystem();

			virtual void HandleMessage(const EngineMessage* Msg);

			virtual void Initialize(SSEngine* Eng);
			virtual void Update(Scalar dt);
			virtual void Shutdown();

			virtual void Serialize();
			virtual void Deserialize();

            Vec3 GetSunPosition() const;

            GLFWwindow* GetWindow() {return m_Window;}
            Vec2 GetResolution() const { return m_Resolution; }

            void Render(RenderList& r);

            void SetCamera(Entity* CameraComp)
            {
                m_ActiveCamera = CameraComp;
            }

            Entity* GetCamera() { return m_ActiveCamera; }

            void IncTriCount(i32 Num) { m_TriangleCount += Num; }
            i32 GetTrianglesPerFrame() const { return m_LastTriangleCount; }

            UID GetFrameID() const { return m_FrameID; }
            bool IsRendering() const { return m_IsRendering; }
            i32 GetVisibleEntityCount() const { return m_VisibleEntityCount; }
            SSEngine* GetEngine() { return m_Engine; }

            void AddGUI(Entity* E);
            void RemoveGUI(Entity* E);
            const Mat4& GetGUIProj() const { return m_GUIProj; }
            GUIManager* GetGUIManager() const { return m_GUIManager; }

        protected:
            i32 m_TriangleCount;
            i32 m_LastTriangleCount;

			SSEngine* m_Engine;
			GLFWwindow* m_Window;
			Vec2 m_Resolution;
            bool m_FullScreen;
            bool m_IsRendering;

            Entity* m_ActiveCamera;

            RenderList* m_VisibleEntityList;
            RenderList* m_VisibleTransparentEntityList;
            i32 m_VisibleEntityCount;

            GUIManager* m_GUIManager;
            Mat4 m_GUIProj;
            Mat4 m_PhysMVP;

            UID m_FrameID;

            RenderingAlgorithm* m_RenderAlg;
	};
}

#endif
