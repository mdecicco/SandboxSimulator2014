#ifndef SandboxSimulator_RenderSystem_h
#define SandboxSimulator_RenderSystem_h

#include <Core/EngineSystem.h>
#include <Rendering/Shader.h>
#include <GLFW/glfw3.h>
#include <Utils/Math.h>
#include <list>

#include <Rendering/Material.h>

using namespace std;
namespace SandboxSimulator
{
    class RenderComponent : public Component
    {
        public:
            RenderComponent();
            ~RenderComponent();

            /* Attribute access */
            void AddVertex(const Vec3& v);
            void SetVertex(i32 Idx,const Vec3& v);
            Vec3 GetVertex(i32 Idx) const;

            void AddTexCoord(const Vec2& v);
            void SetTexCoord(i32 Idx,const Vec2& v);
            Vec2 GetTexCoord(i32 Idx) const;

            void AddNormal(const Vec3& v);
            void SetNormal(i32 Idx,const Vec3& v);
            Vec3 GetNormal(i32 Idx) const;

            void AddTangent(const Vec3& v);
            void SetTangent(i32 Idx,const Vec3& v);
            Vec3 GetTangent(i32 Idx) const;

            i32 GetVertexCount() const;

            void SyncBuffers();

            void SetShader(Shader* Shdr) {m_Shdr = Shdr;}
            void SetMaterial(Material* Mat) {m_Material = Mat;}

        protected:
            friend class RenderSystem;
            vector<Vec3> m_Vertices;
            vector<Vec3> m_Normals;
            vector<Vec3> m_Tangents;
            vector<Vec2> m_TexCoords;

            GLuint m_VertBuff;
            GLuint m_NormBuff;
            GLuint m_TangBuff;
            GLuint m_TexCBuff;
            GLuint m_Vao;

            Shader* m_Shdr;

            Material* m_Material;

            bool m_NeedsUpdate;
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

            GLFWwindow* GetWindow() {return m_Window;}

        protected:
			SSEngine* m_Engine;
			GLFWwindow* m_Window;
			Vec2 m_Resolution;
            bool m_FullScreen;
	};
}

#endif
