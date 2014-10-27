/*
 *  RenderingAlgorithm.cpp
 *
 *  Created by Michael DeCicco on 9/9/14.
 *
 */

#include "RenderingAlgorithm.h"
#include "RenderSystem.h"
#include "Engine.h"

namespace Reality
{
    i32 ForwardRenderingAlgorithm::Render(RenderSystem* rSys,RenderList& l)
    {
        i32 VisibleEntityCount = 0;
        
        CameraComponent* Observer = 0;
        TransformComponent* ObserverTransform = 0;
        Entity* m_Camera = rSys->GetCurrentCamera();
        if(m_Camera)
        {
            Observer = (CameraComponent*)m_Camera->GetComponentByType(CT_CAMERA);
            ObserverTransform = (TransformComponent*)m_Camera->GetComponentByType(CT_TRANSFORM);
        }
        else return 0;
        if(!Observer) return 0;
        
        const Matrix4& p = Observer->GetProjection();
        const Matrix4& v = ObserverTransform ? ObserverTransform->GetTransform().Inverse() : Matrix4::Identity;
        Matrix4 pv = p * v;
        
        for(i32 i = 0;i < l.GetShaderCount();i++)
        {
            Shader* S = l.GetShader(i);
            S->Enable();
            
            #define Uf(u,v) \
            { i32 loc = S->GetUniformLoc(u); if(loc != -1) glUniform1f(loc,v); }
            #define Uv3(u,v) \
            { i32 loc = S->GetUniformLoc(u); if(loc != -1) glUniform3f(loc,v.x,v.y,v.z); }
            #define Uv4(u,v) \
            { i32 loc = S->GetUniformLoc(u); if(loc != -1) glUniform4f(loc,v.x,v.y,v.z,v.w); }
            #define Um(u,m) \
            { i32 loc = S->GetUniformLoc(u); if(loc != -1) glUniformMatrix4fv(loc,1,GL_TRUE,&m.x.x); }
            
            /* Upload constant uniform data */
            if(Observer)
            {
                Um(SU_PROJECTION_MATRIX,p);
                Um(SU_VIEW_MATRIX,v);
                Uf(SU_EXPOSURE,Observer->m_Exposure);
                Uf(SU_NEAR_PLANE,Observer->m_NearPlane);
                Uf(SU_FAR_PLANE,Observer->m_FarPlane);
            }
            Uf(SU_TIME_OF_DAY,Engine::GetEngine()->GetTimeOfDay());
            Uv3(SU_SUN_POSITION,rSys->GetSunPosition());
            
            bool UsingCameraViewFlag = true;
            
            const vector<Entity*>& Entities = l.GetShaderEntityList(i);
            for(i32 e = 0;e < Entities.size();e++)
            {
                Entity* Ent = Entities[e];
                if(Ent == m_Camera) continue;
                RenderComponent* r = (RenderComponent*)Ent->GetComponentByType(CT_RENDER);
                
                /* Sync GPU buffers with current vertex data if necessary */
                r->SyncBuffers();
                
                /* Upload entity-specific uniform data */
                TransformComponent* t = (TransformComponent*)Ent->GetComponentByType(CT_TRANSFORM);
                if(t)
                {
                    const Matrix4& m = t->GetTransform();
                    Matrix4 mvp;
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
                    Um(SU_NORMAL_MATRIX,t->GetNormalMatrix());
                    Um(SU_MVP_MATRIX,mvp);
                }
                else
                {
                    Um(SU_MODEL_MATRIX,Matrix4::Identity);
                    if(r->IsGUI())
                    {
                        Matrix4 gpv = rSys->GetGUIProj() * rSys->GetGUIView();
                        Um(SU_MVP_MATRIX,gpv);
                        if(UsingCameraViewFlag) { Um(SU_VIEW_MATRIX,rSys->GetGUIView()); Um(SU_PROJECTION_MATRIX,rSys->GetGUIProj()); UsingCameraViewFlag = false; }
                    }
                    else
                    {
                        Um(SU_MVP_MATRIX,pv);
                        if(!UsingCameraViewFlag) { Um(SU_VIEW_MATRIX,v); Um(SU_PROJECTION_MATRIX,p); UsingCameraViewFlag = true; }
                    }
                }
                
                if(r->GetMesh()->IsAnimated())
                {
                    const Skeleton* s = r->GetMesh()->GetSkeleton();
                    i16 c = s->GetBoneCount();
                    for(i16 i = 0;i < c;i++)
                    {
                        Matrix4& b = s->GetBone(i)->LocalTransform;
                        b *= Translation(Vector3(Random(-0.0002f,0.0002f),Random(-0.0002f,0.0002f),Random(-0.0002f,0.0002f)));
                        Um((SHADER_UNIFORM)(SU_BONE0 + i),b);
                    }
                }
                
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
                
                /* Render entity */
                glBindVertexArray(r->GetMesh()->GetVAO());
                if(r->GetMesh()->GetIndexCount() == 0) glDrawArrays(r->GetPrimType(),0,r->GetVertexCount());
                else glDrawElements(r->GetPrimType(),r->GetMesh()->GetIndexCount(),GL_UNSIGNED_SHORT,0);
                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D,0);
                
                VisibleEntityCount++;
            }
            S->Disable();
        }
        return VisibleEntityCount;
    }
};