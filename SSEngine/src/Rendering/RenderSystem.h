#ifndef SandboxSimulator_RenderSystem_h
#define SandboxSimulator_RenderSystem_h

#include <Core/EngineSystem.h>
#include <GLFW/glfw3.h>
#include <Utils/Math.h>

namespace SandboxSimulator
{
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

		protected:
			SSEngine* m_Engine;
			GLFWwindow* m_Window;
			Vec2 m_Resolution;
	};
};

#endif