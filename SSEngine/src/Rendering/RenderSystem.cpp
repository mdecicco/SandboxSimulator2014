#include <Rendering/RenderSystem.h>
#include <Core/Message.h>
#include <Engine.h>

#include <stdio.h>
#include <iostream>

namespace SandboxSimulator
{
    RenderComponent::RenderComponent() : Component(CT_RENDER), m_Shdr(0), m_StayVisible(false), m_Mesh(new Mesh()),
                                         m_UseScissorRegion(false), m_UseBlending(false), m_UseDepthTest(true), m_WriteDepth(true), m_Opacity(1.0f),
                                         m_Hide(false), m_PrimType(GL_TRIANGLES), m_Font(0), m_GUIData(0)
    {
    }
    RenderComponent::~RenderComponent()
    {
        if(m_Mesh) delete m_Mesh;
        if(m_Shdr) delete m_Shdr;//TODO
        if(m_GUIData) delete m_GUIData;
        if(m_Font) delete m_Font;
    }

    void RenderComponent::SetShape(RC_SHAPES Shape)
    {
        m_Shape = Shape;
        m_Shdr = new Shader();
        Texture* t = new Texture();
        t->Load("Data/Textures/checker.png");
        m_Mesh->m_Textures[0] = t;
        i32 NumRings = 16;
        i32 NumSectors = 32;
        f32 Radius = 0.5f;
        switch(Shape)
        {
            case RC_TRIANGLE:
                m_Shdr->Load("Data/Shaders/TestShader.glsl");
                AddVertex(Vec3(0   , 0.5,0));
                AddVertex(Vec3(0.5 ,-0.5,0));
                AddVertex(Vec3(-0.5,-0.5,0));
                break;
            case RC_SQUARE:
                m_Shdr->Load("Data/Shaders/TestShader.glsl");
                AddVertex(Vec3(0.5 , 0.5,0));
                AddTexCoord(Vec2(0,0));
                AddNormal(Vec3(0,0,1));
                AddVertex(Vec3(-0.5,-0.5,0));
                AddTexCoord(Vec2(1,1));
                AddNormal(Vec3(0,0,1));
                AddVertex(Vec3(0.5 ,-0.5,0));
                AddTexCoord(Vec2(0,1));
                AddNormal(Vec3(0,0,1));

                AddVertex(Vec3(-0.5,-0.5,0));
                AddTexCoord(Vec2(1,1));
                AddNormal(Vec3(0,0,1));
                AddVertex(Vec3( 0.5, 0.5,0));
                AddTexCoord(Vec2(0,0));
                AddNormal(Vec3(0,0,1));
                AddVertex(Vec3(-0.5, 0.5,0));
                AddTexCoord(Vec2(1,0));
                AddNormal(Vec3(0,0,1));
                break;
            case RC_SPHERE:
                m_Shdr->Load("Data/Shaders/TestShader.glsl");
                for(int r = 0; r <= NumRings; r++)
                {
                    f32 Theta = r * PI / NumRings;
                    f32 SinTheta = sin(Theta);
                    f32 CosTheta = cos(Theta);
                    for(int s = 0; s <= NumSectors; s++)
                    {
                        f32 Phi = s * 2 * PI / NumSectors;
                        f32 SinPhi = sin(Phi);
                
                        f32 x = cos(Phi) * SinTheta;
                        f32 y = CosTheta;
                        f32 z = -(SinPhi * SinTheta);
                
                        f32 u = s * (1.0/NumSectors);
                        f32 v = r * (1.0/NumRings);
                
                        AddVertex(Vec3(x*Radius,y*Radius,z*Radius));
                        AddNormal(Vec3(x,y,z));
                        AddTexCoord(Vec2(u,v));
                    }
                }
                
                for(int r = 0; r < NumRings; r++) for(int s = 0; s < NumSectors; s++)
                {
                    int first = r* (NumSectors + 1) + s;
                    int second = first + NumSectors + 1;
                    int third = first+1;
                    m_Mesh->AddIndex(first);
                    m_Mesh->AddIndex(second);
                    m_Mesh->AddIndex(third);
                
                    m_Mesh->AddIndex(second);
                    m_Mesh->AddIndex(second+1);
                    m_Mesh->AddIndex(third);
                }

                break;
            case RC_SKY_SPHERE:
                m_Shdr->Load("Data/Shaders/SkyGradient.glsl");
                for(int r = 0; r <= NumRings; r++)
                {
                    f32 Theta = r * PI / NumRings;
                    f32 SinTheta = sin(Theta);
                    f32 CosTheta = cos(Theta);
                    for(int s = 0; s <= NumSectors; s++)
                    {
                        f32 Phi = s * 2 * PI / NumSectors;
                        f32 SinPhi = sin(Phi);
                
                        f32 x = cos(Phi) * SinTheta;
                        f32 y = CosTheta;
                        f32 z = -(SinPhi * SinTheta);
                
                        f32 u = s * (1.0/NumSectors);
                        f32 v = r * (1.0/NumRings);
                
                        AddVertex(Vec3(x*Radius,y*Radius,z*Radius));
                        AddNormal(Vec3(x,y,z));
                        AddTexCoord(Vec2(u,v));
                    }
                }
                
                for(int r = 0; r < NumRings; r++) for(int s = 0; s < NumSectors; s++)
                {
                    int first = r* (NumSectors + 1) + s;
                    int second = first + NumSectors + 1;
                    int third = first+1;
                    m_Mesh->AddIndex(first);
                    m_Mesh->AddIndex(third);
                    m_Mesh->AddIndex(second);
                
                    m_Mesh->AddIndex(second);
                    m_Mesh->AddIndex(third);
                    m_Mesh->AddIndex(second+1);
                }

                break;
            case RC_GROUND_PLANE:
                m_Shdr->Load("Data/Shaders/TestShader.glsl");
                AddVertex(Vec3(100 ,0.0,100));
                AddTexCoord(Vec2(0,0));
                AddNormal(Vec3(0,1,0));
                AddVertex(Vec3(100,0,-100));
                AddTexCoord(Vec2(0,1));
                AddNormal(Vec3(0,1,0));
                AddVertex(Vec3(-100,0,-100));
                AddTexCoord(Vec2(1,1));
                AddNormal(Vec3(0,1,0));

                AddVertex(Vec3(-100,0,-100));
                AddTexCoord(Vec2(1,1));
                AddNormal(Vec3(0,1,0));
                AddVertex(Vec3(-100,0,100));
                AddTexCoord(Vec2(1,0));
                AddNormal(Vec3(0,1,0));
                AddVertex(Vec3(100,0,100));
                AddTexCoord(Vec2(0,0));
                AddNormal(Vec3(0,1,0));
                break;
        }
    }

    /* Render System */
	RenderSystem::RenderSystem() : m_TriangleCount(0), m_LastTriangleCount(0), m_FrameID(0)
	{
		m_Resolution = Vec2(1280,720);
        m_ActiveCamera = nullptr;
        m_FullScreen = false;
		AddComponentType(CT_RENDER);
        m_RenderAlg = new ForwardRenderingAlgorithm();
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

    Vec3 RenderSystem::GetSunPosition() const
    {
        Scalar FarDist = ((CameraComponent*)m_ActiveCamera->GetComponentByType(CT_CAMERA))->m_FarPlane;
        Scalar tm = m_Engine->GetTimeOfDay() - 0.25f;
        return Vec3(0,sin(tm * 6.283185306f) * FarDist,cos(tm * 6.283185306f) * FarDist);
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
        //glfwSwapInterval(1);
        glClearColor(1.0f,1.0f,1.0f, 1.0f);

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

        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        m_GUIManager = new GUIManager();
        m_GUIProj = Orthogonal(0.0f,m_Resolution.x,0.0f,m_Resolution.y,0.1f,100.0f);
        m_GUIView = Mat4::Identity;
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

        m_LastTriangleCount = m_TriangleCount;
        m_TriangleCount = 0;

        glfwPollEvents();

        glViewport(0,0,w,h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        i32 VisibleEntityCount = 0;
        m_VisibleEntityList = new RenderList();
        m_VisibleTransparentEntityList = new RenderList();
        for(i32 i = 0;i < m_Components.size();i++)
        {
            RenderComponent* r = (RenderComponent*)m_Components[i];
            if(r->IsHidden()) continue;
            if(!r->UseBlending()) m_VisibleEntityList->AddEntity(r->GetParent());
            else m_VisibleTransparentEntityList->AddEntity(r->GetParent());
            VisibleEntityCount++;
        }
        m_VisibleEntityCount = VisibleEntityCount;

        m_IsRendering = true;
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        if(m_VisibleEntityList->GetShaderCount() > 0) Render(*m_VisibleEntityList);
        if(m_VisibleTransparentEntityList->GetShaderCount() > 0) Render(*m_VisibleTransparentEntityList);
        glDisable(GL_CULL_FACE);
        //Post Processing

        glDisable(GL_DEPTH_TEST);
        //if(m_GUIList->GetShaderCount() > 0) Render(*m_GUIList);
        glEnable(GL_DEPTH_TEST);
        
        glfwSwapBuffers(m_Window);
        
        m_IsRendering = false;
        
        delete m_VisibleEntityList; m_VisibleEntityList = 0;
        //delete m_GUIList; m_GUIList = 0;
        m_FrameID++;
	}

    void RenderSystem::Render(RenderList& l)
    {
        if(!m_RenderAlg) return;
        m_RenderAlg->Render(this,l);
    }

	void RenderSystem::Shutdown() 
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void RenderSystem::Serialize() {}

	void RenderSystem::Deserialize() {}
}
