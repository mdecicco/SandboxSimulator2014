#include <Rendering/RenderSystem.h>
#include <Core/Message.h>
#include <Engine.h>

#include <stdio.h>
#include <iostream>

namespace SandboxSimulator
{

    RenderComponent::RenderComponent() : Component(CT_RENDER), m_VertBuff(0), m_NormBuff(0), m_TangBuff(0), m_TexCBuff(0), m_Vao(0), m_NeedsUpdate(true)
    {}

    RenderComponent::~RenderComponent()
    {}

    void RenderComponent::AddVertex(const Vec3 &v)
    {
        m_Vertices.push_back(v);
        if(m_VertBuff)
        {
            glDeleteBuffers(1, &m_VertBuff);
            m_VertBuff = 0;
        }
        m_NeedsUpdate = true;
    }

    void RenderComponent::SetVertex(i32 Idx, const Vec3 &v)
    {
        m_Vertices[Idx] = v;
        if(m_VertBuff)
        {
            glDeleteBuffers(1, &m_VertBuff);
            m_VertBuff = 0;
        }
        m_NeedsUpdate = true;
    }

    Vec3 RenderComponent::GetVertex(i32 Idx) const
    {
        return m_Vertices[Idx];
    }

    void RenderComponent::AddTexCoord(const Vec2& t)
    {
        m_TexCoords.push_back(t);
        if(m_TexCBuff)
        {
            glDeleteBuffers(1,&m_TexCBuff);
            m_TexCBuff = 0;
        }
        m_NeedsUpdate = true;
    }

    void RenderComponent::SetTexCoord(i32 Idx,const Vec2& t)
    {
        m_TexCoords[Idx] = t;
        if(m_TexCBuff)
        {
            glDeleteBuffers(1,&m_TexCBuff);
            m_TexCBuff = 0;
        }
        m_NeedsUpdate = true;
    }

    Vec2 RenderComponent::GetTexCoord(i32 Idx) const
    {
        return m_TexCoords[Idx];
    }

    void RenderComponent::AddNormal(const Vec3& n)
    {
        m_Normals.push_back(n);
        if(m_NormBuff)
        {
            glDeleteBuffers(1,&m_NormBuff);
            m_NormBuff = 0;
        }
        m_NeedsUpdate = true;
    }
    void RenderComponent::SetNormal(i32 Idx,const Vec3& n)
    {
        m_Normals[Idx] = n;
        if(m_NormBuff)
        {
            glDeleteBuffers(1,&m_NormBuff);
            m_NormBuff = 0;
        }
        m_NeedsUpdate = true;
    }
    Vec3 RenderComponent::GetNormal(i32 Idx) const
    {
        return m_Normals[Idx];
    }

    void RenderComponent::AddTangent(const Vec3& t)
    {
        m_Tangents.push_back(t);
        if(m_TangBuff)
        {
            glDeleteBuffers(1,&m_TangBuff);
            m_TangBuff = 0;
        }
        m_NeedsUpdate = true;
    }
    void RenderComponent::SetTangent(i32 Idx,const Vec3& t)
    {
        m_Tangents.push_back(t);
        if(m_TangBuff)
        {
            glDeleteBuffers(1,&m_TangBuff);
            m_TangBuff = 0;
        }
        m_NeedsUpdate = true;
    }

    Vec3 RenderComponent::GetTangent(i32 Idx) const
    {
        return m_Tangents[Idx];
    }

    i32 RenderComponent::GetVertexCount() const
    {
        return (i32)m_Vertices.size();
    }

    void RenderComponent::SyncBuffers()
    {
        if(m_NeedsUpdate)
        {
			m_NeedsUpdate = false;
            glGenVertexArrays(1,&m_Vao);

            glBindVertexArray(m_Vao);

            if(!m_VertBuff && m_Vertices.size() > 0)
            {
                glGenBuffers(1,&m_VertBuff);
                glBindBuffer(GL_ARRAY_BUFFER,m_VertBuff);
                glBufferData(GL_ARRAY_BUFFER,m_Vertices.size() * sizeof(Vec3),&m_Vertices[0],GL_STATIC_DRAW);
                glEnableVertexAttribArray(VA_POSITION);
                glVertexAttribPointer(VA_POSITION,3,GL_FLOAT,GL_FALSE,0,NULL);
                glBindBuffer(GL_ARRAY_BUFFER,0);
            }
            if(!m_NormBuff && m_Normals.size() > 0)
            {
                glGenBuffers(1,&m_NormBuff);
                glBindBuffer(GL_ARRAY_BUFFER,m_NormBuff);
                glBufferData(GL_ARRAY_BUFFER,m_Normals.size() * sizeof(Vec3),&m_Normals[0],GL_STATIC_DRAW);
                glEnableVertexAttribArray(VA_NORMAL);
                glVertexAttribPointer(VA_NORMAL,3,GL_FLOAT,GL_FALSE,0,NULL);
                glBindBuffer(GL_ARRAY_BUFFER,0);
            }
            if(!m_TangBuff && m_Tangents.size() > 0)
            {
                glGenBuffers(1,&m_TangBuff);
                glBindBuffer(GL_ARRAY_BUFFER,m_TangBuff);
                glBufferData(GL_ARRAY_BUFFER,m_Tangents.size() * sizeof(Vec3),&m_Tangents[0],GL_STATIC_DRAW);
                glEnableVertexAttribArray(VA_TANGENT);
                glVertexAttribPointer(VA_TANGENT,3,GL_FLOAT,GL_FALSE,0,NULL);
                glBindBuffer(GL_ARRAY_BUFFER,0);
            }
            if(!m_TexCBuff && m_TexCoords.size() > 0)
            {
                glGenBuffers(1,&m_TexCBuff);
                glBindBuffer(GL_ARRAY_BUFFER,m_TexCBuff);
                glBufferData(GL_ARRAY_BUFFER,m_TexCoords.size() * sizeof(Vec2),&m_TexCoords[0],GL_STATIC_DRAW);
                glEnableVertexAttribArray(VA_TEXCOORD);
                glVertexAttribPointer(VA_TEXCOORD,2,GL_FLOAT,GL_FALSE,0,NULL);
                glBindBuffer(GL_ARRAY_BUFFER,0);
            }
        }
    }

    /* Render System */
	RenderSystem::RenderSystem() 
	{
		m_Resolution = Vec2(800,600);
        m_FullScreen = false;
		AddComponentType(CT_RENDER);
        AddMessageType(MT_MAKE_RENDERABLE);
        AddMessageType(MT_SET_SHADER);
	}

	RenderSystem::~RenderSystem() {}

    void RenderSystem::HandleMessage(const EngineMessage* Msg)
    {
        switch(Msg->m_MessageType)
        {
            default:
            {
                break;
            }
        }
    }

	void RenderSystem::Initialize(SSEngine* Eng) 
	{
		m_Engine = Eng;
		m_Engine->Log("Initializing render system. \n");
		if(!glfwInit())
		{
			m_Engine->Log("Unable to initialize graphics context.\n");
			return;
		}

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_Window = glfwCreateWindow(m_Resolution.x, m_Resolution.y, "Sandbox Simulator", m_FullScreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if(!m_Window)
		{
			glfwTerminate();
			m_Engine->Log("Unable to open window.\n");
			return;
		}

		glfwMakeContextCurrent(m_Window);

		m_Engine->Log("-----Graphic Profile-----\n");

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if(err != GLEW_OK)
			m_Engine->Log("Glew initialization failed: %s\n", glewGetErrorString(err));
		else
			m_Engine->Log("Glew initialized succesfully!\n");

        m_Engine->Log("OpenGL version: %s\n", glGetString(GL_VERSION));
        m_Engine->Log("OpenGL renderer: %s\n", glGetString(GL_RENDERER));
        m_Engine->Log("OpenGL vendor: %s\n", glGetString(GL_VENDOR));
        m_Engine->Log("Resolution: %dx%d\n", (int)m_Resolution.x, (int)m_Resolution.y);
        m_Engine->Log("Display mode: %s\n", m_FullScreen ? "Fullscreen" : "Windowed");
        m_Engine->Log("-------------------------\n");
	}

	void RenderSystem::Update(Scalar dt) 
	{
        if(!m_Window) return;
        if(glfwWindowShouldClose(m_Window))
        {
            m_Engine->SendMessage(MT_SHUTDOWN);
            return;
        }
        i32 w = 0, h = 0;
        glfwGetWindowSize(m_Window, &w, &h);
        m_Resolution.x = w;
        m_Resolution.y = h;

        glfwPollEvents();
        glfwSwapBuffers(m_Window);

        glViewport(0,0,w,h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(i32 i = 0; i < m_Components.size(); i++)
        {
            RenderComponent* r = (RenderComponent*)m_Components[i];

            if(r->m_Material)
                r->m_Material->Bind();
            else
            {
                //bind default material
            }

            r->SyncBuffers();
            glBindVertexArray(r->m_Vao);
            glDrawArrays(GL_TRIANGLES, 0, r->GetVertexCount());
            glBindVertexArray(0);

            if(r->m_Material)
                r->m_Material->Unbind();
            else
            {
                //unbind default material
            }
        }
	}

	void RenderSystem::Shutdown() 
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void RenderSystem::Serialize() {}

	void RenderSystem::Deserialize() {}
}
