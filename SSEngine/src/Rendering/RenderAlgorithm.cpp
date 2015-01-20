#include <Rendering/RenderAlgorithm.h>
#include <Rendering/RenderSystem.h>
#include <Engine.h>

namespace SandboxSimulator
{
    void RenderList::AddEntity(Entity *E)
    {
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        //If shader list exists
        Shader* s = r->GetShader();
        auto m = m_ShaderEntityListMap.find(s);
        if(m != m_ShaderEntityListMap.end())
        {
            //Add entity to it
            r->m_ShdrListPos = (i32)m->second.size();
            m->second.push_back(E);
        }
        else
        {
            //Otherwise, create list and add entity to it
            m_Shaders.push_back(s);
            m_ShaderEntityListMap[s] = std::vector<Entity*>();
            m_ShaderEntityListMap[s].push_back(E);
            r->m_ShdrListPos = 0;
        }
        
        if(E->GetComponentByType(CT_LIGHT)) m_Lights.push_back(E);
    }
    void RenderList::RemoveEntity(Entity *E)
    {
        //If shader list exists
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        Shader* s = r->GetShader();
        auto m = m_ShaderEntityListMap.find(s);
        if(m != m_ShaderEntityListMap.end())
        {
            //Add entity to it
            m->second.erase(m->second.begin() + r->m_ShdrListPos);
            for(i32 i = r->m_ShdrListPos;i < m->second.size();i++) ((RenderComponent*)m->second[i]->GetComponentByType(CT_RENDER))->m_ShdrListPos--;
        }
        
        if(E->GetComponentByType(CT_LIGHT))
        {
            for(i32 i = 0;i < m_Lights.size();i++)
            {
                if(m_Lights[i] == E)
                {
                    m_Lights.erase(m_Lights.begin() + i);
                    break;
                }
            }
        }
    }

    i32 ForwardRenderingAlgorithm::Render(RenderSystem* rSys,RenderList& l)
    {
        i32 VisibleEntityCount = 0;
        
        CameraComponent* Observer = 0;
        TransformComponent* ObserverTransform = 0;
        Entity* Camera = rSys->GetCamera();
        if(Camera)
        {
            Observer = (CameraComponent*)Camera->GetComponentByType(CT_CAMERA);
            ObserverTransform = (TransformComponent*)Camera->GetComponentByType(CT_TRANSFORM);
        }
        else return 0;
        if(!Observer || !Camera->HasComponentType(CT_CAMERA)) return 0;
        
        const Mat4& p = Observer->GetProjection();
        const Mat4& v = ObserverTransform ? ObserverTransform->GetTransform(false).Inverse() : Mat4::Identity;
        Mat4 pv = p * v;
        
        for(i32 i = 0;i < l.GetShaderCount();i++)
        {
            Shader* S = l.GetShader(i);
            S->Enable();
            
            #define Uf(u,v) \
            { i32 loc = S->GetUniformLoc(u); if(loc != -1) glUniform1f(loc,v); }
            #define Uv2(u,v) \
            { i32 loc = S->GetUniformLoc(u); if(loc != -1) glUniform2f(loc,v.x,v.y); }
            #define Uv3(u,v) \
            { i32 loc = S->GetUniformLoc(u); if(loc != -1) glUniform3f(loc,v.x,v.y,v.z); }
            #define Uv4(u,v) \
            { i32 loc = S->GetUniformLoc(u); if(loc != -1) glUniform4f(loc,v.x,v.y,v.z,v.w); }
            #define Um(u,mat) \
            { i32 loc = S->GetUniformLoc(u); if(loc != -1) glUniformMatrix4fv(loc,1,GL_TRUE,&mat.x.x); }
            
            /* Upload constant uniform data */
            if(Observer)
            {
                Um(SU_PROJECTION_MATRIX,p);
                Um(SU_VIEW_MATRIX,v);
                Uf(SU_EXPOSURE,Observer->m_Exposure);
                Uf(SU_NEAR_PLANE,Observer->m_NearPlane);
                Uf(SU_FAR_PLANE,Observer->m_FarPlane);
                Vec3 cp = ObserverTransform->GetPosition(false);
                Uv3(SU_CAMERA_POSITION,cp);
            }
            Uf(SU_RUNTIME,rSys->GetEngine()->GetElapsedTime());
            Uv2(SU_RESOLUTION,rSys->GetResolution());
            Uf(SU_TIME_OF_DAY,rSys->GetEngine()->GetTimeOfDay());
            Uv3(SU_SUN_POSITION,rSys->GetSunPosition());
            
            bool UsingCameraViewFlag = true;
            
            const vector<Entity*>& Entities = l.GetShaderEntityList(i);
            for(i32 e = 0;e < Entities.size();e++)
            {
                Entity* Ent = Entities[e];
                if(Ent == Camera) continue;
                RenderComponent* r = (RenderComponent*)Ent->GetComponentByType(CT_RENDER);
                if(r->IsHidden()) continue;
                
                /* Sync GPU buffers with current vertex data if necessary */
                r->SyncBuffers();
                TransformComponent* t = (TransformComponent*)Ent->GetComponentByType(CT_TRANSFORM);
                if(t)
                {
                    Mat4 m = t->GetTransform(false);
                    Mat4 n = t->GetNormalMatrix(false);
                    Mat4 mvp;
                    if(r->IsGUI())
                    {
                        mvp = rSys->GetGUIProj() * rSys->GetGUIView() * m;
                        if(UsingCameraViewFlag) { Um(SU_VIEW_MATRIX,rSys->GetGUIView()); Um(SU_PROJECTION_MATRIX,rSys->GetGUIProj()); UsingCameraViewFlag = false; }
                    }
                    else
                    {
                        mvp = pv * m;
                        if(!UsingCameraViewFlag) { Um(SU_VIEW_MATRIX,v); Um(SU_PROJECTION_MATRIX,p); UsingCameraViewFlag = true; }
                    }
                    Um(SU_MODEL_MATRIX,m);
                    Um(SU_NORMAL_MATRIX,n);
                    Um(SU_MVP_MATRIX,mvp);
                }
                else
                {
                    Um(SU_MODEL_MATRIX,Mat4::Identity);
                    if(r->IsGUI())
                    {
                        Mat4 gpv = rSys->GetGUIProj() * rSys->GetGUIView();
                        Um(SU_MVP_MATRIX,gpv);
                        if(UsingCameraViewFlag) { Um(SU_VIEW_MATRIX,rSys->GetGUIView()); Um(SU_PROJECTION_MATRIX,rSys->GetGUIProj()); UsingCameraViewFlag = false; }
                    }
                    else
                    {
                        Um(SU_MVP_MATRIX,pv);
                        if(!UsingCameraViewFlag) { Um(SU_VIEW_MATRIX,v); Um(SU_PROJECTION_MATRIX,p); UsingCameraViewFlag = true; }
                    }
                }
                
                Uf(SU_OPACITY,r->GetOpacity());
                
                for(i32 i = 0;i < r->GetMesh()->m_TexParams.size();i++)
                {
                    r->GetMesh()->m_Textures[r->GetMesh()->m_TexParams[i].x]->Bind();
                    glTexParameteri(GL_TEXTURE_2D,r->GetMesh()->m_TexParams[i].y,r->GetMesh()->m_TexParams[i].z);
                }
                
                for(i32 i = 0;i < MAX_TEXTURES;i++)
                {
                    Texture* Tex = r->GetMesh()->m_Textures[i];
                    if(Tex)
                    {
                        i32 TexiLoc = S->GetUniformLoc((SHADER_UNIFORM)(SU_TEXTURE0 + i));
                        if(TexiLoc != -1)
                        {
                            glActiveTexture(GL_TEXTURE0 + i);
                            glBindTexture(GL_TEXTURE_2D,Tex->GetTexID());
                            glUniform1i(TexiLoc,i);
                        }
                    }
                }

                if(!r->UseDepthTest()) glDisable(GL_DEPTH_TEST);
                if(r->UseScissor())
                {
                    glEnable(GL_SCISSOR_TEST);
                    Vec2 v0 = r->GetScissorPos();
                    Vec2 v1 = r->GetScissorSize();
                    i32 x,y;
                    glfwGetWindowSize(rSys->GetWindow(),&x,&y);
                    glScissor(v0.x,(y - (i32)v1.y) - (i32)v0.y,v1.x,v1.y);
                }
                if(r->UseBlending())
                {
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
                    //glDepthMask(GL_FALSE);
                }
                if(!r->DepthWritesEnabled()) glDepthMask(GL_FALSE);
                
                /* Render entity */
                glBindVertexArray(r->GetMesh()->GetVAO());
                if(r->GetMesh()->GetIndexCount() == 0)
                {
                    glDrawArrays(r->GetPrimType(),0,r->GetVertexCount());
                    if(r->GetPrimType() == GL_TRIANGLES) rSys->IncTriCount(r->GetVertexCount() / 3);
                    else if(r->GetPrimType() == GL_TRIANGLE_FAN) rSys->IncTriCount((r->GetVertexCount() - 2) / 3);
                    else if(r->GetPrimType() == GL_TRIANGLE_STRIP) rSys->IncTriCount((r->GetVertexCount() - 2) / 3);
                }
                else
                {
                    glDrawElements(r->GetPrimType(),r->GetMesh()->GetIndexCount(),GL_UNSIGNED_SHORT,0);
                    if(r->GetPrimType() == GL_TRIANGLES) rSys->IncTriCount(r->GetMesh()->GetIndexCount() / 3);
                    else if(r->GetPrimType() == GL_TRIANGLE_FAN) rSys->IncTriCount((r->GetMesh()->GetIndexCount() - 2) / 3);
                    else if(r->GetPrimType() == GL_TRIANGLE_STRIP) rSys->IncTriCount((r->GetMesh()->GetIndexCount() - 2) / 3);
                }
                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D,0);
                glDisable(GL_SCISSOR_TEST);
                if(!r->UseDepthTest()) glEnable(GL_DEPTH_TEST);
                if(r->UseBlending()) glDisable(GL_BLEND);
                if(!r->DepthWritesEnabled()) glDepthMask(GL_TRUE);
                //glEnable(GL_DEPTH_TEST);
                
                VisibleEntityCount++;
            }
            S->Disable();
        }
        return VisibleEntityCount;
    }
}