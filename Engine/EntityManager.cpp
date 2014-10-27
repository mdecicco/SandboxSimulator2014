/*
 *  EntityManager.cpp
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#include "EntityManager.h"
#include "EngineSystem.h"
#include "Message.h"
#include "Engine.h"
#include "Lights.h"

#include <string>
using namespace std;

namespace Reality
{
    Component::Component(COMPONENT_TYPE Type) : m_Engine(Engine::GetEngine()), m_RefCount(1), m_Type(Type) { }
    Component::~Component() { }
    
    void Component::AddRef()
    {
        m_RefCount++;
    }
    void Component::Destroy()
    {
        m_RefCount--;
        if(m_RefCount == 0) delete this;
    }

    i32 Entity::Destroy()
    {
        m_RefCount--;
        if(m_RefCount == 0)
        {
            Engine::GetEngine()->GetEntityManager()->DestroyEntity(this);
            for(i32 i = 0;i < CT_COUNT;i++)
            {
                if(m_Components[i]) m_Components[i]->Destroy();
                m_Components[i] = 0;
            }
            if(m_Definition) m_Definition->Destroy();
            m_Definition = 0;
            delete this;
            return 0;
        }
        return m_RefCount;
    }
    void Entity::AddScript(const string& ScriptPath,Scalar Rate)
    {
        Engine::GetEngine()->Broadcast(new AddScriptMessage(this,ScriptPath,Rate));
    }
    void Entity::AddVertex(const Vector3& v)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->AddVertex(v);
    }
    void Entity::SetGUIFlag(bool Flag)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->SetGUIFlag(Flag);
        if(Flag) Engine::GetEngine()->GetRenderSystem()->AddGUI(this);
        else Engine::GetEngine()->GetRenderSystem()->RemoveGUI(this);
    }
    void Entity::AddText(const string &Fnt,const Vector3 &Offset,i32 TextureID,const string &Text)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        Font* fnt = (Font*)Engine::GetEngine()->GetAssetManager()->LoadAsset(Fnt.c_str(),AT_FONT);
        if(!fnt) return;
        fnt->AddTextToEntity(this,Text,Offset,TextureID);
    }
    bool Entity::IsGUI() const
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return false;
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->IsGUI();
    }
    void Entity::ClearVertices()
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->Clear();
    }
    void Entity::SetVertex(i32 Idx,const Vector3& v)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->SetVertex(Idx,v);
    }
    Vector3 Entity::GetVertex(i32 Idx) const
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return Vector3();
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetVertex(Idx);
    }
    void Entity::AddTexCoord(const Vector2& t)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->AddTexCoord(t);
    }
    void Entity::SetTexCoord(i32 Idx,const Vector2& t)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->SetTexCoord(Idx,t);
    }
    Vector2 Entity::GetTexCoord(i32 Idx) const
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return Vector2();
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetTexCoord(Idx);
    }
    void Entity::AddNormal(const Vector3& n)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->AddNormal(n);
    }
    void Entity::SetNormal(i32 Idx,const Vector3& n)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->SetNormal(Idx,n);
    }
    Vector3 Entity::GetNormal(i32 Idx) const
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return Vector3();
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetNormal(Idx);
    }
    void Entity::AddTangent(const Vector3& t)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->AddTangent(t);
    }
    void Entity::SetTangent(i32 Idx,const Vector3& t)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->SetTangent(Idx,t);
    }
    Vector3 Entity::GetTangent(i32 Idx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return Vector3();
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetTangent(Idx);
    }
    void Entity::AddVertexWeight1(f32 Weight)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->AddVertexWeight1(Weight);
    }
    void Entity::AddVertexWeight2(f32 Weight0,f32 Weight1)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->AddVertexWeight2(Weight0,Weight1);
    }
    void Entity::AddVertexWeight3(f32 Weight0,f32 Weight1,f32 Weight2)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->AddVertexWeight3(Weight0,Weight1,Weight2);
    }
    void Entity::AddVertexWeight4(f32 Weight0,f32 Weight1,f32 Weight2,f32 Weight3)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->AddVertexWeight4(Weight0,Weight1,Weight2,Weight3);
    }
    void Entity::SetWeight0(i32 Idx,f32 Weight)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->SetWeight0(Idx,Weight);
    }
    void Entity::SetWeight1(i32 Idx,f32 Weight)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->SetWeight1(Idx,Weight);
    }
    void Entity::SetWeight2(i32 Idx,f32 Weight)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->SetWeight2(Idx,Weight);
    }
    void Entity::SetWeight3(i32 Idx,f32 Weight)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->SetWeight3(Idx,Weight);
    }
    f32 Entity::GetWeight0(i32 Idx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0;
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->GetWeight0(Idx);
    }
    f32 Entity::GetWeight1(i32 Idx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0;
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->GetWeight1(Idx);
    }
    f32 Entity::GetWeight2(i32 Idx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0;
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->GetWeight2(Idx);
    }
    f32 Entity::GetWeight3(i32 Idx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0;
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->GetWeight3(Idx);
    }

    void Entity::AddVertexBoneIndex1(i32 Idx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->AddVertexBoneIndex1(Idx);
    }
    void Entity::AddVertexBoneIndex2(i32 Idx0,i32 Idx1)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->AddVertexBoneIndex2(Idx0,Idx1);
    }
    void Entity::AddVertexBoneIndex3(i32 Idx0,i32 Idx1,i32 Idx2)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->AddVertexBoneIndex3(Idx0,Idx1,Idx2);
    }
    void Entity::AddVertexBoneIndex4(i32 Idx0,i32 Idx1,i32 Idx2,i32 Idx3)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->AddVertexBoneIndex4(Idx0,Idx1,Idx2,Idx3);
    }
    void Entity::SetBoneIndex0(i32 vIdx,i32 bIdx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->SetBoneIndex0(vIdx,bIdx);
    }
    void Entity::SetBoneIndex1(i32 vIdx,i32 bIdx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->SetBoneIndex1(vIdx,bIdx);
    }
    void Entity::SetBoneIndex2(i32 vIdx,i32 bIdx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->SetBoneIndex2(vIdx,bIdx);
    }
    void Entity::SetBoneIndex3(i32 vIdx,i32 bIdx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->SetBoneIndex3(vIdx,bIdx);
    }
    i32 Entity::GetBoneIndex0(i32 Idx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0;
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->GetBoneIndex0(Idx);
    }
    i32 Entity::GetBoneIndex1(i32 Idx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0;
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->GetBoneIndex1(Idx);
    }
    i32 Entity::GetBoneIndex2(i32 Idx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0;
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->GetBoneIndex2(Idx);
    }
    i32 Entity::GetBoneIndex3(i32 Idx)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0;
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetMesh()->GetBoneIndex3(Idx);
    }
    i32 Entity::GetVertexCount() const
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0;
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetVertexCount();
    }
    void Entity::SetTexParam(i32 TexNum,i32 Param,i32 Val)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->SetTexParam(TexNum,Param,Val);
    }
    void Entity::SetShader(const string& Path)
    {
        Engine::GetEngine()->Broadcast(new SetShaderMessage(this,Path));
    }
    void Entity::SetPrimType(i32 Type)
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->SetPrimType(Type);
    }
    i32 Entity::GetPrimType() const
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0;
        }
        return ((RenderComponent*)m_Components[CT_RENDER])->GetPrimType();
    }
    void Entity::AcquireDimensions()
    {
        if(!m_Components[CT_RENDER])
        {
            Log("Error calling %s with entity <%d>. Entity is not renderable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((RenderComponent*)m_Components[CT_RENDER])->AcquireDimensions();
    }
    void Entity::SetLightType(i32 Type)
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((LightComponent*)m_Components[CT_LIGHT])->m_Type = (LightComponent::LIGHT_TYPE)Type;
    }
    i32 Entity::GetLightType() const
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return -1;
        }
        return ((LightComponent*)m_Components[CT_LIGHT])->m_Type;
    }
    void Entity::SetAmbient(const Vector4& Color)
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((LightComponent*)m_Components[CT_LIGHT])->m_Ambient = Color;
    }
    Vector4 Entity::GetAmbient() const
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return Vector4();
        }
        return ((LightComponent*)m_Components[CT_LIGHT])->m_Ambient;
    }
    void Entity::SetDiffuse(const Vector4& Color)
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((LightComponent*)m_Components[CT_LIGHT])->m_Diffuse = Color;
    }
    Vector4 Entity::GetDiffuse() const
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return Vector4();
        }
        return ((LightComponent*)m_Components[CT_LIGHT])->m_Diffuse;
    }
    void Entity::SetSpecular(const Vector4& Color)
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((LightComponent*)m_Components[CT_LIGHT])->m_Specular = Color;
    }
    Vector4 Entity::GetSpecular() const
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return Vector4();
        }
        return ((LightComponent*)m_Components[CT_LIGHT])->m_Specular;
    }
    void Entity::SetConstantAttenuation(Scalar Att)
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((LightComponent*)m_Components[CT_LIGHT])->m_ConstantAttenuation = Att;
    }
    Scalar Entity::GetConstantAttenuation() const
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((LightComponent*)m_Components[CT_LIGHT])->m_ConstantAttenuation;
    }
    void Entity::SetLinearAttenuation(Scalar Att)
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((LightComponent*)m_Components[CT_LIGHT])->m_LinearAttenuation = Att;
    }
    Scalar Entity::GetLinearAttenuation() const
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((LightComponent*)m_Components[CT_LIGHT])->m_LinearAttenuation;
    }
    void Entity::SetQuadraticAttenuation(Scalar Att)
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((LightComponent*)m_Components[CT_LIGHT])->m_QuadraticAttenuation = Att;
    }
    Scalar Entity::GetQuadraticAttenuation() const
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((LightComponent*)m_Components[CT_LIGHT])->m_QuadraticAttenuation;
    }
    void Entity::SetAttenuation(Scalar C,Scalar L,Scalar Q)
    {
        LightComponent* l = (LightComponent*)m_Components[CT_LIGHT];
        if(!l)
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        l->m_ConstantAttenuation  = C;
        l->m_LinearAttenuation    = L;
        l->m_QuadraticAttenuation = Q;
    }
    void Entity::SetExponent(Scalar Exp)
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((LightComponent*)m_Components[CT_LIGHT])->m_Exponent = Exp;
    }
    Scalar Entity::GetExponent() const
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((LightComponent*)m_Components[CT_LIGHT])->m_Exponent;
    }
    void Entity::SetIntensity(Scalar Intensity)
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((LightComponent*)m_Components[CT_LIGHT])->m_Intensity = Intensity;
    }
    Scalar Entity::GetIntensity() const
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((LightComponent*)m_Components[CT_LIGHT])->m_Intensity;
    }
    void Entity::SetCutoffAngle(Scalar Cutoff)
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        ((LightComponent*)m_Components[CT_LIGHT])->m_CutoffAngle = Cutoff;
    }
    Scalar Entity::GetCutoffAngle() const
    {
        if(!m_Components[CT_LIGHT])
        {
            Log("Error calling %s with entity <%d>. Entity is not a light. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((LightComponent*)m_Components[CT_LIGHT])->m_CutoffAngle;
    }
    void Entity::SetNear(Scalar Near)
    {
        if(!m_Components[CT_CAMERA])
        {
            Log("Error calling %s with entity <%d>. Entity is not a camera. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        CameraComponent* c = ((CameraComponent*)m_Components[CT_CAMERA]);
        c->m_NearPlane = Near;
        c->m_UpdateProjection = true;
    }
    Scalar Entity::GetNear()
    {
        if(!m_Components[CT_CAMERA])
        {
            Log("Error calling %s with entity <%d>. Entity is not a camera. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((CameraComponent*)m_Components[CT_CAMERA])->m_NearPlane;
    }
    void Entity::SetFar(Scalar Far)
    {
        if(!m_Components[CT_CAMERA])
        {
            Log("Error calling %s with entity <%d>. Entity is not a camera. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        CameraComponent* c = ((CameraComponent*)m_Components[CT_CAMERA]);
        c->m_FarPlane = Far;
        c->m_UpdateProjection = true;
    }
    Scalar Entity::GetFar()
    {
        if(!m_Components[CT_CAMERA])
        {
            Log("Error calling %s with entity <%d>. Entity is not a camera. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((CameraComponent*)m_Components[CT_CAMERA])->m_FarPlane;
    }
    void Entity::SetPlanes(Scalar Near,Scalar Far)
    {
        if(!m_Components[CT_CAMERA])
        {
            Log("Error calling %s with entity <%d>. Entity is not a camera. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        CameraComponent* c = ((CameraComponent*)m_Components[CT_CAMERA]);
        c->m_FarPlane = Far;
        c->m_NearPlane = Near;
        c->m_UpdateProjection = true;
    }
    void Entity::SetExposure(Scalar Exp)
    {
        if(!m_Components[CT_CAMERA])
        {
            Log("Error calling %s with entity <%d>. Entity is not a camera. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        CameraComponent* c = ((CameraComponent*)m_Components[CT_CAMERA]);
        c->m_Exposure = Exp;
        c->m_UpdateProjection = true;
    }
    Scalar Entity::GetExposure()
    {
        if(!m_Components[CT_CAMERA])
        {
            Log("Error calling %s with entity <%d>. Entity is not a camera. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((CameraComponent*)m_Components[CT_CAMERA])->m_Exposure;
    }
    void Entity::SetFieldOfView(Scalar FOV)
    {
        if(!m_Components[CT_CAMERA])
        {
            Log("Error calling %s with entity <%d>. Entity is not a camera. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return;
        }
        CameraComponent* c = ((CameraComponent*)m_Components[CT_CAMERA]);
        c->m_FieldOfView.x = FOV;
        c->m_UpdateProjection = true;
    }
    Scalar Entity::GetFieldOfView()
    {
        if(!m_Components[CT_CAMERA])
        {
            Log("Error calling %s with entity <%d>. Entity is not a camera. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((CameraComponent*)m_Components[CT_CAMERA])->m_FieldOfView.x;
    }
    void Entity::Identity()
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->Identity();
    }
    void Entity::Translate(const Vector3& t)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->Translate(t);
    }
    void Entity::Translate(Scalar x,Scalar y,Scalar z)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->Translate(x,y,z);
    }
    void Entity::Rotate(const Quaternion& q)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->Rotate(q);
    }
    void Entity::Rotate(const Vector3& Axis,Scalar Angle)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->Rotate(Axis,Angle);
    }
    void Entity::Rotate(Scalar Ax,Scalar Ay,Scalar Az,Scalar Angle)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->Rotate(Ax,Ay,Az,Angle);
    }
    void Entity::Scale(Scalar s)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->Scale(s);
    }
    void Entity::Scale(Scalar x,Scalar y,Scalar z)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->Scale(x,y,z);
    }
    void Entity::Scale(const Vector3& s)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->Scale(s);
    }
    Vector3 Entity::GetPosition() const
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return Vector3();
        }
        return ((TransformComponent*)m_Components[CT_TRANSFORM])->GetPosition();
    }
    Vector3 Entity::GetScale() const
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return Vector3();
        }
        return ((TransformComponent*)m_Components[CT_TRANSFORM])->GetScale();
    }
    Quaternion Entity::GetOrientation() const
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return Quaternion();
        }
        return ((TransformComponent*)m_Components[CT_TRANSFORM])->GetOrientation();
    }

    const Matrix4& Entity::GetTransform()
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable. Undefined results will ensue.\n",__FUNCTION__,m_UID);
            return Matrix4::Identity;
        }
        return ((TransformComponent*)m_Components[CT_TRANSFORM])->GetTransform();
    }
    void Entity::SetGravity(const Vector3& Gravity)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->setGravity(Gravity);
    }
    void Entity::ApplyImpulse(const Vector3& Impulse,const Vector3& Point)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->applyImpulse(Impulse,Point);
    }
    void Entity::ApplyTorque(const Vector3& Torque)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->applyTorque(Torque);
    }
    void Entity::ApplyTorqueImpulse(const Vector3& Torque)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->applyTorqueImpulse(Torque);
    }
    void Entity::SetAngularDamping(Scalar Damping)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        TransformComponent* t = (TransformComponent*)m_Components[CT_TRANSFORM];
        t->m_Body->setDamping(t->m_Body->getLinearDamping(),Damping);
    }
    Scalar Entity::GetAngularDamping() const
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->getAngularDamping();
    }
    void Entity::SetLinearDamping(Scalar Damping)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        TransformComponent* t = (TransformComponent*)m_Components[CT_TRANSFORM];
        t->m_Body->setDamping(Damping,t->m_Body->getAngularDamping());
    }
    Scalar Entity::GetLinearDamping() const
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return 0.0f;
        }
        return ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->getLinearDamping();
    }
    Vector3 Entity::GetLinearVelocity() const
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return Vector3();
        }
        return ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->getLinearVelocity();
    }
    Vector3 Entity::GetAngularVelocity() const
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return Vector3();
        }
        return ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->getAngularVelocity();
    }
    void Entity::SetAngularFactor(const Vector3& Factor)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->setAngularFactor(Factor);
    
    }
    Vector3 Entity::GetAngularFactor() const
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return Vector3();
        }
        return ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->getAngularFactor();
    
    }
    void Entity::SetSleepWhenInactive(bool Flag)
    {
        if(!m_Components[CT_TRANSFORM])
        {
            Log("Error calling %s with entity <%d>. Entity is not transformable.\n",__FUNCTION__,m_UID);
            return;
        }
        if(!Flag) ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->setActivationState(DISABLE_DEACTIVATION);
        else      ((TransformComponent*)m_Components[CT_TRANSFORM])->m_Body->setActivationState(WANTS_DEACTIVATION);
    }
    void Entity::SubscribeToInput()
    {
        Engine::GetEngine()->GetInputManager()->SubscribeEntity(this);
    }
    void Entity::Send(const Event& E)
    {
        Engine::GetEngine()->SendEvent(this,E);
    }
    Component* Entity::GetComponentByType(COMPONENT_TYPE Type)
    {
        return m_Components[Type];
    }
    
    EntityManager::EntityManager() : m_Engine(Engine::GetEngine())
    {
    }
    EntityManager::~EntityManager()
    {
    }

    Entity* EntityManager::CreateEntity()
    {
        Entity* E = new Entity();
        m_Entities.push_back(E);
        E->m_UID = (UID)m_Entities.size() - 1;
        return E;
    }
    void EntityManager::DestroyEntity(Entity *E)
    {
        m_Entities[E->GetID()] = 0;
    }

    void EntityManager::AddComponent(Entity* E,Component* Comp)
    {
        E->m_Components[Comp->GetType()] = Comp;
        Comp->AddRef();
        m_Engine->Broadcast(new ComponentAddedMessage(E,Comp));
    }
    void EntityManager::RemoveComponent(Entity* E,Component* Comp)
    {
        m_Engine->Broadcast(new ComponentRemovedMessage(E,Comp));
        E->m_Components[Comp->GetType()] = 0;
        Comp->Destroy();
    }
    void EntityManager::RemoveComponentByType(Entity* E,COMPONENT_TYPE Type)
    {
        m_Engine->Broadcast(new ComponentRemovedMessage(E,E->m_Components[Type]));
        E->m_Components[Type]->Destroy();
        E->m_Components[Type] = 0;
    }
};