#include <Core/TransformComponent.h>

namespace SandboxSimulator
{
    TransformComponent::TransformComponent() : Component(CT_TRANSFORM), m_IsStatic(false),
                                                  m_DidMove(true), m_DidRotate(true), m_DidScale(true),
                                                  m_Clickable(true), m_InheritOrientation(true),
                                                  m_InheritPosition(true), m_InheritScale(true), m_Simulated(false), m_Updated(false),
                                                  m_FirstPersonXRot(0), m_FirstPersonYRot(0), m_FirstPerson(false), m_HasParent(false)
    {
        Identity();
    }
    TransformComponent::~TransformComponent()
    {}

    void TransformComponent::Identity()
    {
        if(m_Position.x + m_Position.y + m_Position.z != 0.0f) m_DidMove = true;
        m_Transform = Mat4::Identity;
        m_Position = Vec3();
        m_Orientation = Quat(0.0f,1.0f,0.0f,0.0f);
        m_Scale = Vec3(1.0f,1.0f,1.0f);
        m_Updated = false;
    }
    void TransformComponent::Translate(const Vec3& t)
    {
        if(m_DidRotate && m_FirstPerson) {
            Quat pitch = Quat(Vec3(1,0,0), m_FirstPersonYRot);
            Quat yaw = Quat(Vec3(0,1,0), m_FirstPersonXRot);
            m_Orientation = yaw * pitch;
        }
        if(m_FirstPerson)
            m_Position += (m_Orientation * t);
        else
            m_Position += t;
        m_Updated = true;
        m_DidMove = true;
    }
    void TransformComponent::Translate(Scalar x,Scalar y,Scalar z)
    {
        if(m_DidRotate && m_FirstPerson) {
            Quat pitch = Quat(Vec3(1,0,0), m_FirstPersonYRot);
            Quat yaw = Quat(Vec3(0,1,0), m_FirstPersonXRot);
            m_Orientation = yaw * pitch;
        }
        if(m_FirstPerson)
            m_Position += (m_Orientation * Vec3(x,y,z));
        else
            m_Position += Vec3(x,y,z);
        m_Updated = true;
        m_DidMove = true;
    }
    void TransformComponent::Rotate(const Quat& q)
    {
        m_Orientation *= q;
        m_Updated = true;
        m_DidRotate = true;
    }
    void TransformComponent::Rotate(const Vec3& Axis,Scalar Angle)
    {
        if(m_FirstPerson) {
            m_FirstPersonYRot += Axis.x*Angle;
            m_FirstPersonXRot += Axis.y*Angle;
            if(m_FirstPersonYRot > 90) m_FirstPersonYRot = 90;
            if(m_FirstPersonYRot < -90) m_FirstPersonYRot = -90;
        } else {
            m_Orientation *= Quat(Axis,Angle);
        }
        m_Updated = true;
        m_DidRotate = true;
    }
    void TransformComponent::Rotate(Scalar Ax,Scalar Ay,Scalar Az,Scalar Angle)
    {
        if(m_FirstPerson) {
            m_FirstPersonYRot += Ax*Angle;
            m_FirstPersonXRot += Ay*Angle;
            if(m_FirstPersonYRot > 90) m_FirstPersonYRot = 90;
            if(m_FirstPersonYRot < -90) m_FirstPersonYRot = -90;
        } else {
            m_Orientation *= Quat(Ax,Ay,Az,Angle);
        }
        m_Updated = true;
        m_DidRotate = true;
    }
    void TransformComponent::Scale(Scalar s)
    {
        m_Scale *= s;
        m_Updated = true;
        m_DidScale = true;
    }
    void TransformComponent::Scale(Scalar x,Scalar y,Scalar z)
    {
        m_Scale.x *= x;
        m_Scale.y *= y;
        m_Scale.z *= z;
        m_Updated = true;
        m_DidScale = true;
    }
    void TransformComponent::Scale(const Vec3& s)
    {
        m_Scale *= s;
        m_Updated = true;
        m_DidScale = true;
    }
    Vec3 TransformComponent::GetPosition(bool Relative) const
    {
        //Entity* P = m_Parent->GetParent();
        Vec3 Pos = m_Position;
        if(m_HasParent) {
            TransformComponent* t = (TransformComponent*)m_RelativeTo->GetComponentByType(CT_TRANSFORM);
            if(t) Pos += t->GetPosition(Relative);
        }
        /*if(P && !Relative && m_InheritPosition)
        {
            TransformComponent* t = GetTransformComponent(P);
            if(t) Pos += t->GetPosition(Relative);
            P->Destroy();
        }*/
        return Pos;
    }
    Vec3 TransformComponent::GetScale(bool Relative) const
    {
        //Entity* P = m_Entity->GetParent();
        Vec3 Sc = m_Scale;
        if(m_HasParent) {
            TransformComponent* t = (TransformComponent*)m_RelativeTo->GetComponentByType(CT_TRANSFORM);
            if(t) Sc *= t->GetScale(Relative);
        }
        /*if(P && !Relative && m_InheritScale)
        {
            TransformComponent* t = GetTransformComponent(P);
            if(t) Sc *= t->GetScale(Relative);
            P->Destroy();
        }*/
        return Sc;
    }
    Quat TransformComponent::GetOrientation(bool Relative) const
    {
        //Entity* P = m_Entity->GetParent();
        Quat Orientation = m_Orientation;
        if(m_HasParent) {
            TransformComponent* t = (TransformComponent*)m_RelativeTo->GetComponentByType(CT_TRANSFORM);
            if(t) Orientation *= t->GetOrientation(Relative);
        }
        /*if(P && !Relative && m_InheritOrientation)
        {
            TransformComponent* t = GetTransformComponent(P);
            if(t) Orientation *= t->GetOrientation(Relative);
            P->Destroy();
        }*/
        return Orientation;
    }

    Mat4 TransformComponent::GetTransform(bool Relative)
    {
        if(m_Updated)
        {
            /* To do: Optimize. */
            
            m_Transform = Mat4::Identity;
            if(m_Position.x + m_Position.y + m_Position.z != 0.0f) m_Transform = Translation(m_Position);
            if(!m_FirstPerson) {
                if(m_Orientation.w != 0.0f) m_Transform *= m_Orientation.ToMat();
            } else {
                Quat pitch = Quat(Vec3(1,0,0), m_FirstPersonYRot);
                Quat yaw = Quat(Vec3(0,1,0), m_FirstPersonXRot);
                m_Orientation = yaw * pitch;
                if(m_Orientation.w != 0.0f) m_Transform *= m_Orientation.ToMat();
            }
            m_Transform *= SandboxSimulator::Scale(m_Scale);
            m_NormalMatrix = m_Orientation.ToMat().Inverse().Transpose();
            m_Updated = false;
        }
        //Entity* Parent = m_Entity->GetParent();
        if(m_HasParent) {
            TransformComponent* t = (TransformComponent*)m_RelativeTo->GetComponentByType(CT_TRANSFORM);
            if(t)
            {
                Mat4 ret = m_Transform;
                ret = t->GetOrientation(false).ToMat() * ret;
                ret = SandboxSimulator::Translation(t->GetPosition(false)) * ret;
                ret = SandboxSimulator::Scale(t->GetScale()) * ret;
                return ret;
            }
        }
        /*if(Parent && !m_Simulated && !Relative && (m_InheritPosition || m_InheritOrientation || m_InheritScale))
        {
            TransformComponent* t = GetTransformComponent(Parent);
            if(t)
            {
                if(m_InheritPosition && m_InheritOrientation && m_InheritScale) return t->GetTransform(Relative) * m_Transform;
                Matrix4 ret = m_Transform;
                if(m_InheritOrientation) ret = t->GetOrientation(false).ToMatrix() * ret;
                if(m_InheritPosition   ) ret = Translation(t->GetPosition(false))  * ret;
                if(m_InheritScale      ) ret = Reality::Scale(t->GetScale())       * ret;
                return ret;
            }
        }*/
        return m_Transform;
    }
    Mat4 TransformComponent::GetNormalMatrix(bool Relative)
    {
        //Update stuff if necessary
        GetTransform();
        if(m_HasParent) {
            TransformComponent* t = (TransformComponent*)m_RelativeTo->GetComponentByType(CT_TRANSFORM);
            if(t)
            {
                return t->GetNormalMatrix(Relative) * m_NormalMatrix;
            }
        }
        /*Entity* Parent = m_Entity->GetParent();
        if(Parent && !m_Simulated && !Relative && m_InheritOrientation)
        {
            TransformComponent* t = GetTransformComponent(Parent);
            if(t)
            {
                return t->GetNormalMatrix(Relative) * m_NormalMatrix;
            }
        }*/
        return m_NormalMatrix;
    }
    void TransformComponent::UpdateData(const Vec3 &t,const Quat &r,const Vec3 &s)
    {
        if(t.x != m_Position.x || t.y != m_Position.y || t.z != m_Position.z)
        {
            m_Position = t;
            m_Updated = true;
            m_DidMove = true;
        }
            
        if(r.x != m_Orientation.x || r.y != m_Orientation.y || r.z != m_Orientation.z || r.w != m_Orientation.w)
        {
            m_Orientation = r;
            m_Updated = true;
            m_DidRotate = true;
        }
        
        if(s.x != m_Scale.x || s.y != m_Scale.y || s.z != m_Scale.z)
        {
            m_Scale = s;
            m_Updated = true;
            m_DidScale = true;
        }
    }
};