#ifndef SandboxSimulator_TransformComponent_h
#define SandboxSimulator_TransformComponent_h

#include <Core/SceneGraph.h>
#include <Utils/Math.h>

namespace SandboxSimulator {
class TransformComponent : public Component
{
    public:
        TransformComponent();
        ~TransformComponent();

        //Setters
        void SetPosition(Vec3 Pos, bool UpdatePhysics = true);
        void SetVelocity(Vec3 Vel, bool RelativeToOrientation = false);
        void AddForce(Vec3 Force, bool RelativeToOrientation = false);
        void AddImpulse(Vec3 Force, bool RelativeToOrientation = false);
        void SetOrientation(Quat O, bool UpdatePhysics = true);
        void SetScale(Vec3 Scale);

        //Getters
        Mat4 GetMat4();
        Vec3 GetPosition() { return m_Position; }

        bool IsStatic() const { return m_IsStatic; }
        bool DidMove() { bool tmp = m_DidMove; m_DidMove = false; return tmp; }
        bool DidRotate() { bool tmp = m_DidRotate; m_DidRotate = false; return tmp; }
        bool DidScale() { bool tmp = m_DidScale; m_DidScale = false; return tmp; }
        
        void Identity();
        void Translate(const Vec3& t);
        void Translate(Scalar x,Scalar y,Scalar z);
        void Rotate(const Quat& q);
        void Rotate(const Vec3& Axis,Scalar Angle);
        void Rotate(Scalar Ax,Scalar Ay,Scalar Az,Scalar Angle);
        void Scale(Scalar s);
        void Scale(Scalar x,Scalar y,Scalar z);
        void Scale(const Vec3& s);
        Vec3 GetPosition(bool Relative = true) const;
        Vec3 GetScale(bool Relative = true) const;
        Quat GetOrientation(bool Relative = true) const;
        
        Mat4 GetTransform(bool Relative = true);
        Mat4 GetNormalMatrix(bool Relative = true);
        
        void UpdateData(const Vec3& t,const Quat& r,const Vec3& s);
        bool IsClickable() const { return m_Clickable; }

        void SetFirstPerson(bool fp) { m_FirstPerson = fp;m_Updated = true; }

        void SetRelativeTo(Entity* E) { m_RelativeTo = E; m_HasParent = true; }
        void SetStatic(bool Static) { m_IsStatic = Static; }

    protected:
        Vec3 m_Position;
        Vec3 m_Scale;
        Quat m_Orientation;
        bool m_Updated;
        bool m_IsStatic;
        bool m_DidMove;
        bool m_DidRotate;
        bool m_DidScale;
        bool m_Clickable;
        bool m_InheritOrientation;
        bool m_InheritPosition;
        bool m_InheritScale;
        bool m_Simulated;

        bool m_FirstPerson;
        f32 m_FirstPersonYRot;
        f32 m_FirstPersonXRot;

        Mat4 m_Transform;
        Mat4 m_NormalMatrix;

        bool m_HasParent;
        Entity* m_RelativeTo;
};
}

#endif