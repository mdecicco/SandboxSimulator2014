/*
 *  RenderSystem.cpp
 *
 *  Created by Michael DeCicco on 7/2/14.
 *
 */

#include "RenderSystem.h"
#include "Engine.h"
#include "stdlib.h"

namespace Reality
{
    void GLFW_error_callback(i32 ErrorID,Literal Description)
    {
        Log("GLFW Error <%d> %s.\n",ErrorID,Description);
    }
    
    RenderComponent::RenderComponent() : Component(CT_RENDER), m_Entity(0), m_Shdr(0), m_GUIData(0), m_StayVisible(false), m_OctreeNode(0), m_Mesh(0)
    {
    }
    RenderComponent::~RenderComponent()
    {
        if(m_Mesh) m_Mesh->Destroy();
        if(m_Shdr) m_Shdr->Destroy();
        if(m_GUIData) delete m_GUIData;
    }
    void RenderComponent::SetGUIFlag(bool Flag)
    {
        if(Flag)
        {
            Engine::GetEngine()->GetRenderSystem()->AddGUI(m_Entity);
            return;
        }
        Engine::GetEngine()->GetRenderSystem()->RemoveGUI(m_Entity);
    }
    

    TransformComponent::TransformComponent(Entity* Ent) : Component(CT_TRANSFORM), m_Entity(Ent), m_IsStatic(false), m_DidMove(true), m_DidRotate(true), m_Body(0)
    {
        Identity();
        m_Volume.TargetCenter = &m_Position;
        m_Volume.Dimensions = Vector3(1,1,1);
    }
    TransformComponent::~TransformComponent()
    {
    }
    void TransformComponent::Identity()
    {
        if(m_Position.x + m_Position.y + m_Position.z != 0.0f) m_DidMove = true;
        m_Transform = Matrix4::Identity;
        m_Position = Vector3();
        m_Orientation = Quaternion(0.0f,1.0f,0.0f,0.0f);
        m_Scale = Vector3(1.0f,1.0f,1.0f);
        m_Updated = false;
    }
    void TransformComponent::Translate(const Vector3& t)
    {
        m_Position += t;
        m_Updated = true;
        m_DidMove = true;
    }
    void TransformComponent::Translate(Scalar x,Scalar y,Scalar z)
    {
        m_Position += Vector3(x,y,z);
        m_Updated = true;
        m_DidMove = true;
    }
    void TransformComponent::Rotate(const Quaternion& q)
    {
        m_Orientation *= q;
        m_Updated = true;
    }
    void TransformComponent::Rotate(const Vector3& Axis,Scalar Angle)
    {
        m_Orientation *= Quaternion(Axis,Angle);
        m_Updated = true;
    }
    void TransformComponent::Rotate(Scalar Ax,Scalar Ay,Scalar Az,Scalar Angle)
    {
        m_Orientation *= Quaternion(Ax,Ay,Az,Angle);
        m_Updated = true;
    }
    void TransformComponent::Scale(Scalar s)
    {
        m_Scale *= s;
        m_Updated = true;
    }
    void TransformComponent::Scale(Scalar x,Scalar y,Scalar z)
    {
        m_Scale.x *= x;
        m_Scale.y *= y;
        m_Scale.z *= z;
        m_Updated = true;
    }
    void TransformComponent::Scale(const Vector3& s)
    {
        m_Scale *= s;
        m_Updated = true;
    }

    const Matrix4& TransformComponent::GetTransform()
    {
        if(m_Body)
        {
            btTransform t;
            m_Body->getMotionState()->getWorldTransform(t);
            const btVector3   & o = t.getOrigin();
            const btQuaternion& q = t.getRotation();
            
            if(o.x() != m_Position.x || o.y() != m_Position.y || o.z() != m_Position.z)
            {
                m_Position.x = o.x();
                m_Position.y = o.y();
                m_Position.z = o.z();
                m_Updated = true;
                m_DidMove = true;
            }
            
            if(q.x() != m_Orientation.x || q.y() != m_Orientation.y || q.z() != m_Orientation.z || q.w() != m_Orientation.w)
            {
                m_Orientation.x = q.x();
                m_Orientation.y = q.y();
                m_Orientation.z = q.z();
                m_Orientation.w = q.w();
                m_Updated = true;
                m_DidRotate = true;
            }
        }
        if(m_Updated)
        {
            /* To do: Optimize. */
            m_Transform = Matrix4::Identity;
            if(m_Position.x + m_Position.y + m_Position.z != 0.0f) m_Transform = Translation(m_Position);
            if(m_Orientation.w != 0.0f) m_Transform *= m_Orientation.ToMatrix();
            m_Transform *= Reality::Scale(m_Scale);
            m_NormalMatrix = m_Orientation.ToMatrix().Inverse().Transpose();
            m_Updated = false;
        }
        return m_Transform;
    }
    const Matrix4& TransformComponent::GetNormalMatrix()
    {
        //Update stuff if necessary
        GetTransform();
        return m_NormalMatrix;
    }
    
    CameraComponent::CameraComponent() : Component(CT_CAMERA), m_FieldOfView(70.0f,70.0f), m_NearPlane(1.0f),
                                         m_FarPlane(1000.0f), m_Speed(0.0f), m_UpdateProjection(true), m_Exposure(1.0f)
    {
    }
    CameraComponent::~CameraComponent()
    {
    }

    const Matrix4& CameraComponent::GetProjection()
    {
        if(m_UpdateProjection)
        {
            m_Projection = PerspectiveMultiFov(m_FieldOfView.x,m_FieldOfView.y,m_NearPlane,m_FarPlane);
            m_UpdateProjection = false;
        }
        return m_Projection;
    }
    
    RenderSystem::RenderSystem(PNode* GfxSettings)
    {
        PNode* Res = GfxSettings->Search("Resolution");
        if(Res) m_Resolution = *Res;
        else m_Resolution = Vector2(800,600);
        
        PNode* Fs = GfxSettings->Search("Fullscreen");
        if(Fs) m_Fullscreen = *Fs;
        else m_Fullscreen = false;
        
        PNode* Rend = GfxSettings->Search("Renderer");
        if(Rend)
        {
            string Renderer = (CString)*Rend;
            if(Renderer == "Deferred")
            {
                Log("Not supported yet...\n");
                Engine::GetEngine()->SendMessage(MT_SHUTDOWN);
                m_RenderingAlgorithm = 0;
            }
            else if(Renderer == "Forward")
            {
                m_RenderingAlgorithm = new ForwardRenderingAlgorithm();
            }
            else if(Renderer == "Z-Prepass")
            {
                Log("Not supported yet...\n");
                Engine::GetEngine()->SendMessage(MT_SHUTDOWN);
                m_RenderingAlgorithm = 0;
            }
        }
        else
        {
            Log("Error: Must define \"Renderer\" in engine config file (in <Graphics> settings).\nOptions are: \"Deferred\", \"Forward\", \"Z-Prepass\"\n");
            Engine::GetEngine()->SendMessage(MT_SHUTDOWN);
            m_RenderingAlgorithm = 0;
        }
        PNode* Cull = GfxSettings->Search("Culling");
        if(Cull)
        {
            string Culler = (CString)*Cull;
            if(Culler == "Octree")
            {
                m_CullingAlgorithm = new DefaultFrustumCullingAlgorithm();
            }
            else if(Culler == "Brute Force")
            {
                m_CullingAlgorithm = new BruteForceFrustumCullingAlgorithm();
            }
        }
        else
        {
            Log("Error: Must define \"Culling\" in engine config file (in <Graphics> settings).\nOptions are: \"Octree\", \"Brute Force\".\n");
            Engine::GetEngine()->SendMessage(MT_SHUTDOWN);
            m_CullingAlgorithm = 0;
        }
        
        m_Camera = 0;
        m_IsRendering = false;
        m_RenderDebugPhys = false;
        setDebugMode(DBG_DrawAabb);
    }
    RenderSystem::~RenderSystem()
    {
    }

    void RenderSystem::HandleMessage(const EngineMessage* Msg)
    {
        switch(Msg->m_MessageType)
        {
            case MT_MAKE_TRANSFORMABLE:
            {
                MakeTransformableMessage* tMsg = (MakeTransformableMessage*)Msg;
                Engine::GetEngine()->AddComponent(tMsg->m_Entity,new TransformComponent(tMsg->m_Entity));
                break;
            }
            case MT_MAKE_CAMERA:
            {
                if(m_Camera)
                {
                    Log("Universe may only have one observer. Current observer is entity number %d.\n",m_Camera->GetID());
                    return;
                }
                MakeCameraMessage* oMsg = (MakeCameraMessage*)Msg;
                Engine::GetEngine()->AddComponent(oMsg->m_Entity,new CameraComponent());
                m_Camera = oMsg->m_Entity;
                break;
            }
            case MT_MAKE_RENDERABLE:
            {
                MakeRenderableMessage* rMsg = (MakeRenderableMessage*)Msg;
                PropertyList* Def = rMsg->m_Entity->GetDef();
                RenderComponent* r = 0;
                bool NoRenderDef = false;
                if(Def)
                {
                    /* Load renderable data from file */
                    PNode* Base = Def->Search("Entity");
                    if(Base)
                    {
                        PNode* Render = Base->Search("Render");
                        if(Render)
                        {
                            r = new RenderComponent();
                            r->m_Entity = rMsg->m_Entity;
                            PNode* AlwaysRender = Render->Search("Always Render");
                            if(AlwaysRender && AlwaysRender->IsValue()) r->m_StayVisible = *AlwaysRender;
                            Engine::GetEngine()->AddComponent(rMsg->m_Entity,r);
                            PNode* MeshFile = Render->Search("Mesh");
                            if(MeshFile) r->m_Mesh = (Mesh*)Engine::GetEngine()->GetAssetManager()->LoadAsset((CString)*MeshFile,AT_MESH);
                            else r->m_Mesh = new Mesh();
                            r->AcquireDimensions();
                            
                            PNode* PrimType = Render->Search("Primitives");
                            if(PrimType)
                            {
                                string Type((CString)*PrimType);
                                if(Type      == "TRIANGLES"     ) r->SetPrimType(GL_TRIANGLES);
                                else if(Type == "TRIANGLE_FAN"  ) r->SetPrimType(GL_TRIANGLE_FAN);
                                else if(Type == "TRIANGLE_STRIP") r->SetPrimType(GL_TRIANGLE_STRIP);
                                else if(Type == "LINES"         ) r->SetPrimType(GL_LINES);
                                else if(Type == "LINE_STRIP"    ) r->SetPrimType(GL_LINE_STRIP);
                                else if(Type == "LINE_LOOP"     ) r->SetPrimType(GL_LINE_LOOP);
                                else if(Type == "POINTS"        ) r->SetPrimType(GL_POINTS);
                            }
                            
                            PNode* Text = Render->Search("Text");
                            if(Text)
                            {
                                PNode* Str = Text->Search("Str");
                                PNode* Fnt = Text->Search("Font");
                                PNode* Tid = Text->Search("TextureID");
                                PNode* Off = Text->Search("Offset");
                                if(Str)
                                {
                                    if(Fnt)
                                    {
                                        CString str = *Str;
                                        CString fnt = *Fnt;
                                        i32     tid = (Tid ? *Tid : 0);
                                        Vector3 off = (Off ? *Off : Vector3(0,0,0));
                                        
                                        Font* TheFont = (Font*)Engine::GetEngine()->GetAssetManager()->LoadAsset(fnt,AT_FONT);
                                        if(TheFont)
                                        {
                                            TheFont->AddTextToEntity(rMsg->m_Entity,str,off,tid);
                                        }
                                        else
                                        {
                                        }
                                    }
                                    else
                                    {
                                    }
                                }
                                else
                                {
                                }
                            }
                            
                            for(i32 i = 0;i < MAX_TEXTURES;i++)
                            {
                                CString TexName = MakeCString(16);
                                memset(TexName,0,16);
                                snprintf(TexName,16,"Texture%d",i);
                                PNode* TexNode = Render->Search(TexName);
                                FreeCString(TexName);
                                if(TexNode)
                                {
                                    PNode* Path = TexNode->Search("Path");
                                    if(Path)
                                    {
                                        /* Load texture */
                                        r->m_Mesh->m_Textures[i] = (Texture*)Engine::GetEngine()->GetAssetManager()->LoadAsset((CString)*Path,AT_TEXTURE);
                                        /* Set properties */
                                    }
                                    else if(TexNode->IsValue())
                                    {
                                        /* Load texture */
                                        r->m_Mesh->m_Textures[i] = (Texture*)Engine::GetEngine()->GetAssetManager()->LoadAsset((CString)*TexNode,AT_TEXTURE);
                                    }
                                    else
                                    {
                                        Log("Invalid texture definition. (Texture unit %d, no <Path></Path> defined)\n",i);
                                    }
                                }
                            }
                            
                            PNode* Shdr = Render->Search("Shader");
                            if(Shdr)
                            {
                                if(Shdr->IsValue())
                                {
                                    SetShaderMessage* msg = new SetShaderMessage(rMsg->m_Entity,string(*Shdr));
                                    HandleMessage(msg);
                                    delete msg;
                                }
                                else
                                {
                                    Log("What do?\n");
                                }
                            }
                            
                            m_RenderableEntities.push_back(rMsg->m_Entity);
                            m_CullingAlgorithm->OnEntityAdd(rMsg->m_Entity);
                        }
                        else NoRenderDef = true;
                    }
                    else NoRenderDef = true;
                }
                else NoRenderDef = true;
                
                if(NoRenderDef)
                {
                    /* Let the user do it */
                    RenderComponent* r = new RenderComponent();
                    r->m_Entity = rMsg->m_Entity;
                    Engine::GetEngine()->AddComponent(rMsg->m_Entity,r);
                    m_CullingAlgorithm->OnEntityAdd(rMsg->m_Entity);
                }
                break;
            }
            case MT_DESTROY_ENTITY:
            {
                DestroyEntityMessage* eMsg = (DestroyEntityMessage*)Msg;
                //If the entity is a renderable entity, it must be removed from culling algorithm
                if(eMsg->m_Entity->GetComponentByType(CT_RENDER)) m_CullingAlgorithm->OnEntityRemove(eMsg->m_Entity);
                for(i32 i = 0;i < m_RenderableEntities.size();i++) { if(m_RenderableEntities[i] == eMsg->m_Entity) { m_RenderableEntities.erase(m_RenderableEntities.begin() + i); break; } }
                //Engine::GetEngine()->RemoveComponent(eMsg->m_Entity,eMsg->m_Entity->GetComponentByType(CT_RENDER));
                //if(eMsg->m_Entity->GetComponentByType(CT_TRANSFORM)) Engine::GetEngine()->RemoveComponent(eMsg->m_Entity,eMsg->m_Entity->GetComponentByType(CT_TRANSFORM));
                break;
            }
            case MT_SET_SHADER:
            {
                SetShaderMessage* sMsg = (SetShaderMessage*)Msg;
                Shader* Shdr = (Shader*)Engine::GetEngine()->GetAssetManager()->LoadAsset(sMsg->m_ShaderPath.c_str(),AT_SHADER);
                if(Shdr)
                {
                    RenderComponent* r = ((RenderComponent*)sMsg->m_Entity->GetComponentByType(CT_RENDER));
                    if(r->m_Shdr) r->m_Shdr->Destroy();
                    r->m_Shdr = Shdr;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
    Vector3 RenderSystem::GetSunPosition() const
    {
        Scalar FarDist = m_Camera->GetFar();
        Scalar tm = Engine::GetEngine()->GetTimeOfDay();
        return Vector3(0,sin(tm * 6.283185306f) * FarDist,cos(tm * 6.283185306f) * FarDist);
    }
    void RenderSystem::Initialize()
    {
        glfwSetErrorCallback(GLFW_error_callback);
        if(!glfwInit())
        {
            Log("Unable to initialize graphics context.\n");
            return;
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
        
        m_Window = glfwCreateWindow(m_Resolution.x,m_Resolution.y,"Reality",m_Fullscreen ? glfwGetPrimaryMonitor() : 0,0);
        if(!m_Window)
        {
            glfwTerminate();
            Log("Unable to open window.\n");
            return;
        }
        glfwMakeContextCurrent(m_Window);
        glfwSetKeyCallback(m_Window,KeyCallback);
        glfwSetMouseButtonCallback(m_Window,MouseButtonCallback);
        glfwSetCursorPosCallback(m_Window,CursorPosCallback);
        
        Log("-----Graphic  profile-----\n");
        Log("OpenGL version: %s\n" ,glGetString(GL_VERSION ));
        Log("OpenGL renderer: %s\n",glGetString(GL_RENDERER));
        Log("OpenGL vendor: %s\n"  ,glGetString(GL_VENDOR  ));
        Log("Engine renderer: %s\n",m_RenderingAlgorithm->GetAlgorithmName());
        Log("Resolution: %dx%d\n"  ,(i32)m_Resolution.x,(i32)m_Resolution.y);
        Log("Display mode: %s\n"   ,m_Fullscreen ? "Fullscreen" : "Windowed");
        Log("--------------------------\n");
        
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        
        m_WorldSize = 5000.0f;
        
        if(m_CullingAlgorithm)
        {
            m_CullingAlgorithm->InitScene(m_RenderableEntities);
        }
        
        m_GUIManager = new GUIManager();
        m_GUIProj = Orthogonal(0.0f,m_Resolution.x,0.0f,m_Resolution.y,0.1f,100.0f);
        m_GUIView = Matrix4::Identity;
        
        m_PhysShader = (Shader*)Engine::GetEngine()->GetAssetManager()->LoadAsset("Shaders/Engine/DebugPhysics.shdr",AT_SHADER);
        Engine::GetEngine()->GetPhysicsSystem()->GetWorld()->setDebugDrawer(this);
    }
    void RenderSystem::Update(Scalar dt)
    {
        m_FPS = 1.0f / dt;
        if(!m_Window) return;
        if(glfwWindowShouldClose(m_Window))
        {
            Engine::GetEngine()->SendMessage(MT_SHUTDOWN);
            return;
        }
        i32 w = 0,h = 0;
        glfwGetWindowSize(m_Window,&w,&h);
        m_Resolution.x = w;
        m_Resolution.y = h;
        
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
        
        glViewport(0,0,m_Resolution.x,m_Resolution.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        m_VisibleEntityCount = 0;
        for(i32 i = 0;i < m_CullingAlgorithm->VisibilityFlags.size();i++)
        {
            if(m_CullingAlgorithm->VisibilityFlags[i])
            {
                m_VisibleEntityList->AddEntity(m_RenderableEntities[i]);
                m_VisibleEntityCount++;
            }
        }
        
        /* Cull GUI */
        m_GUIManager->GetVisibleEntities(m_GUIList);
        
        m_IsRendering = true;
        if(m_VisibleEntityList->GetShaderCount() > 0) Render(*m_VisibleEntityList);
        glDisable(GL_DEPTH_TEST);
        if(m_GUIList->GetShaderCount() > 0) Render(*m_GUIList);
        glEnable(GL_DEPTH_TEST);
        
        if(m_RenderDebugPhys)
        {
            CameraComponent* Observer = 0;
            TransformComponent* ObserverTransform = 0;
            if(m_Camera)
            {
                Observer = GetCameraComponent(m_Camera);
                ObserverTransform = GetTransformComponent(m_Camera);
                if(Observer)
                {
                    m_PhysMVP = Observer->GetProjection() * (ObserverTransform ? ObserverTransform->GetTransform().Inverse() : Matrix4::Identity);
                    Engine::GetEngine()->GetPhysicsSystem()->GetWorld()->debugDrawWorld();
                }
            }
        }
        
        m_IsRendering = false;
        
        delete m_VisibleEntityList; m_VisibleEntityList = 0;
        delete m_GUIList; m_GUIList = 0;
    }
    void RenderSystem::Shutdown()
    {
        if(m_CullingAlgorithm) delete m_CullingAlgorithm;
        if(m_RenderingAlgorithm) delete m_RenderingAlgorithm;
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void RenderSystem::Serialize()
    {
    }
    void RenderSystem::Deserialize()
    {
    }
    void RenderSystem::CreateRenderTask()
    {
        m_VisibleEntityList = new RenderList();
        m_GUIList = new RenderList();
        m_CullingAlgorithm->CreateTask();
    }
    void RenderSystem::Render(RenderList& l)
    {
        if(!m_RenderingAlgorithm) return;
        m_RenderingAlgorithm->Render(this,l);
    }
    void RenderSystem::AddGUI(Entity* E)
    {
        m_GUIManager->AddEntity(E);
    }
    void RenderSystem::RemoveGUI(Entity* E)
    {
        m_GUIManager->RemoveEntity(E);
    }
    void RenderSystem::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
    {
        if(!m_PhysShader) return;
        m_PhysShader->Enable();
        Mesh m;
        m.AddVertex(from);
        m.AddNormal(color);
        m.AddVertex(to);
        m.AddNormal(color);
        m.SyncBuffers();
        i32 mvpLoc = m_PhysShader->GetUniformLoc(SU_MVP_MATRIX);
        if(mvpLoc != -1) glUniformMatrix4fv(mvpLoc,1,GL_TRUE,&m_PhysMVP.x.x);
        glDrawArrays(GL_LINES,0,2);
        m_PhysShader->Disable();
    }
    void RenderSystem::drawTriangle(const btVector3& v0,const btVector3& v1,const btVector3& v2,const btVector3& color, btScalar /*alpha*/)
    {
        if(!m_PhysShader) return;
        m_PhysShader->Enable();
        Mesh m;
        m.AddVertex(v0);
        m.AddNormal(color);
        m.AddVertex(v1);
        m.AddNormal(color);
        
        m.AddVertex(v1);
        m.AddNormal(color);
        m.AddVertex(v2);
        m.AddNormal(color);
        
        m.AddVertex(v2);
        m.AddNormal(color);
        m.AddVertex(v0);
        m.AddNormal(color);
        
        m.SyncBuffers();
        i32 mvpLoc = m_PhysShader->GetUniformLoc(SU_MVP_MATRIX);
        if(mvpLoc != -1) glUniformMatrix4fv(mvpLoc,1,GL_TRUE,&m_PhysMVP.x.x);
        glDrawArrays(GL_LINES,0,6);
        m_PhysShader->Disable();
    }
    void RenderSystem::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
    {
    }
};