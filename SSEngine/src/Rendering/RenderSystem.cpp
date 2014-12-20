#include <Rendering/RenderSystem.h>
#include <Engine.h>

namespace SandboxSimulator
{
	RenderSystem::RenderSystem() 
	{
		m_Resolution = Vec2(800,600);
	}

	RenderSystem::~RenderSystem() {}

	void RenderSystem::HandleMessage(const EngineMessage* Msg) {}

	void RenderSystem::Initialize(SSEngine* Eng) 
	{
		m_Engine = Eng;
		m_Engine->Log("Initializing render system. \n");
		if(!glfwInit())
		{
			m_Engine->Log("Unable to initialize graphics context.\n");
			return;
		}
		m_Window = glfwCreateWindow(m_Resolution.x, m_Resolution.y, "Sandbox Simulator", NULL, NULL);
		if(!m_Window)
		{
			glfwTerminate();
			m_Engine->Log("Unable to open window.\n");
			return;
		}
		glfwMakeContextCurrent(m_Window);
	}

	void RenderSystem::Update(Scalar dt) 
	{
		if(!m_Window) return;
        if(glfwWindowShouldClose(m_Window))
        {
            m_Engine->SendMessage(MT_SHUTDOWN);
            return;
        }
        Scalar ratio = m_Resolution.x / m_Resolution.y;
        glViewport(0, 0,m_Resolution.x,m_Resolution.y);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(-0.6f, -0.4f, 0.f);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(0.6f, -0.4f, 0.f);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
	}

	void RenderSystem::Shutdown() 
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void RenderSystem::Serialize() {}

	void RenderSystem::Deserialize() {}
}