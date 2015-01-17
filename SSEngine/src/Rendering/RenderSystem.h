#ifndef SandboxSimulator_RenderSystem_h
#define SandboxSimulator_RenderSystem_h

#include <Core/EngineSystem.h>
#include <Rendering/Shader.h>
#include <Utils/Math.h>
#include <list>

#include <Rendering/CameraComponent.h>

using namespace std;
namespace SandboxSimulator
{
    enum RC_SHAPES
    {
        RC_NONE,
        RC_TRIANGLE,
        RC_SQUARE,
        RC_COUNT
    };

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

            void SetShape(RC_SHAPES Shape);

            i32 GetVertexCount() const;

            void SyncBuffers();

            Shader* GetShader() { return m_Shader; }

            virtual void BinarySerialize(sf::Packet* Packet);
            virtual void BinaryDeserialize(sf::Packet* Packet);

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

            bool m_NeedsUpdate;
            Shader* m_Shader;

            RC_SHAPES m_Shape;
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

            void SetCamera(CameraComponent* CameraComp)
            {
                m_ActiveCamera = CameraComp;
            }

        protected:
			SSEngine* m_Engine;
			GLFWwindow* m_Window;
			Vec2 m_Resolution;
            bool m_FullScreen;
            CameraComponent* m_ActiveCamera;
	};
}

#endif
