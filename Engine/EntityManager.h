/*
 *  EntityManager.h
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#pragma once
#include "EngineTypes.h"
#include "EngineMath.h"
#include "Event.h"
#include "Log.h"
#include <vector>
using namespace std;

namespace Reality
{
    class Engine;
    enum COMPONENT_TYPE
    {
        CT_SCRIPT,
        CT_RENDER,
        CT_TRANSFORM,
        CT_CAMERA,
        CT_LIGHT,
        CT_ANIMATION,
        CT_COUNT
    };
    
    class Component
    {
        public:
            Component(COMPONENT_TYPE Type);
            virtual ~Component();
        
            void AddRef();
            void Destroy();
        
            COMPONENT_TYPE GetType() const { return m_Type; }
        
        protected:
            Engine* m_Engine;
            i32 m_RefCount;
            COMPONENT_TYPE m_Type;
    };
    
    class Engine;
    class PropertyList;
    class Entity
    {
        public:
            Entity() : m_UID(-1), m_RefCount(1), m_Definition(0) { memset(m_Components,0,sizeof(Component*) * CT_COUNT); }
            ~Entity() { }
        
            void AddRef() { m_RefCount++; }
            i32 Destroy();
        
            /* Component add functions for script env */
            void AddScript(const string& ScriptPath,Scalar Rate = 0.0f);
        
            /* Functions for renderable entities only */
            void SetGUIFlag(bool Flag);
            void AddText(const string& Font,const Vector3& Offset,i32 TextureID,const string& Text);
            bool IsGUI() const;
            void ClearVertices();
            void AddVertex(const Vector3& v);
            void AddVertex_(Scalar x,Scalar y,Scalar z) { AddVertex(Vector3(x,y,z)); }
            void SetVertex(i32 Idx,const Vector3& v);
            Vector3 GetVertex(i32 Idx) const;
            void AddTexCoord(const Vector2& t);
            void AddTexCoord_(Scalar x,Scalar y) { AddTexCoord(Vector2(x,y)); }
            void SetTexCoord(i32 Idx,const Vector2& t);
            Vector2 GetTexCoord(i32 Idx) const;
            void AddNormal(const Vector3& n);
            void AddNormal_(Scalar x,Scalar y,Scalar z) { AddNormal(Vector3(x,y,z)); }
            void SetNormal(i32 Idx,const Vector3& n);
            Vector3 GetNormal(i32 Idx) const;
            void AddTangent(const Vector3& t);
            void AddTangent_(Scalar x,Scalar y,Scalar z) { AddTangent(Vector3(x,y,z)); }
            void SetTangent(i32 Idx,const Vector3& t);
            Vector3 GetTangent(i32 Idx);
            void AddVertexWeight1(f32 Weight);
            void AddVertexWeight2(f32 Weight0,f32 Weight1);
            void AddVertexWeight3(f32 Weight0,f32 Weight1,f32 Weight2);
            void AddVertexWeight4(f32 Weight0,f32 Weight1,f32 Weight2,f32 Weight3);
            void SetWeight0(i32 Idx,f32 Weight);
            void SetWeight1(i32 Idx,f32 Weight);
            void SetWeight2(i32 Idx,f32 Weight);
            void SetWeight3(i32 Idx,f32 Weight);
            f32 GetWeight0(i32 Idx);
            f32 GetWeight1(i32 Idx);
            f32 GetWeight2(i32 Idx);
            f32 GetWeight3(i32 Idx);
            void AddVertexBoneIndex1(i32 Idx);
            void AddVertexBoneIndex2(i32 Idx0,i32 Idx1);
            void AddVertexBoneIndex3(i32 Idx0,i32 Idx1,i32 Idx2);
            void AddVertexBoneIndex4(i32 Idx0,i32 Idx1,i32 Idx2,i32 Idx3);
            void SetBoneIndex0(i32 vIdx,i32 bIdx);
            void SetBoneIndex1(i32 vIdx,i32 bIdx);
            void SetBoneIndex2(i32 vIdx,i32 bIdx);
            void SetBoneIndex3(i32 vIdx,i32 bIdx);
            i32 GetBoneIndex0(i32 Idx);
            i32 GetBoneIndex1(i32 Idx);
            i32 GetBoneIndex2(i32 Idx);
            i32 GetBoneIndex3(i32 Idx);
            i32 GetVertexCount() const;
            void SetTexParam(i32 TexNum,i32 Param,i32 Val);
            void SetShader(const string& Path);
            void SetPrimType(i32 Type);
            i32 GetPrimType() const;
            void AcquireDimensions();
        
            /* Functions for lights only */
            void SetLightType(i32 Type);
            i32 GetLightType() const;
            void SetAmbient(const Vector4& Color);
            Vector4 GetAmbient() const;
            void SetDiffuse(const Vector4& Color);
            Vector4 GetDiffuse() const;
            void SetSpecular(const Vector4& Color);
            Vector4 GetSpecular() const;
            void SetConstantAttenuation(Scalar Att);
            Scalar GetConstantAttenuation() const;
            void SetLinearAttenuation(Scalar Att);
            Scalar GetLinearAttenuation() const;
            void SetQuadraticAttenuation(Scalar Att);
            Scalar GetQuadraticAttenuation() const;
            void SetAttenuation(Scalar C,Scalar L,Scalar Q);
            void SetExponent(Scalar Exp);
            Scalar GetExponent() const;
            void SetIntensity(Scalar Intensity);
            Scalar GetIntensity() const;
            void SetCutoffAngle(Scalar Cutoff);
            Scalar GetCutoffAngle() const;
        
            /* Functions for cameras only */
            void SetNear(Scalar Near);
            Scalar GetNear();
            void SetFar(Scalar Far);
            Scalar GetFar();
            void SetPlanes(Scalar Near,Scalar Far);
            void SetExposure(Scalar Exp);
            Scalar GetExposure();
            void SetFieldOfView(Scalar FOV);
            Scalar GetFieldOfView();
        
            /* Functions for transformable entities only */
            void Identity();
            void Translate(const Vector3& t);
            void Translate(Scalar x,Scalar y,Scalar z);
            void Rotate(const Quaternion& q);
            void Rotate(const Vector3& Axis,Scalar Angle);
            void Rotate(Scalar Ax,Scalar Ay,Scalar Az,Scalar Angle);
            void Scale(Scalar s);
            void Scale(Scalar x,Scalar y,Scalar z);
            void Scale(const Vector3& s);
            Vector3 GetPosition() const;
            Vector3 GetScale() const;
            Quaternion GetOrientation() const;
            const Matrix4& GetTransform();
            void SetGravity(const Vector3& Gravity);
            void ApplyImpulse(const Vector3& Impulse,const Vector3& Point);
            void ApplyTorque(const Vector3& Torque);
            void ApplyTorqueImpulse(const Vector3& Torque);
            void SetAngularDamping(Scalar Damping);
            Scalar GetAngularDamping() const;
            void SetLinearDamping(Scalar Damping);
            Scalar GetLinearDamping() const;
            Vector3 GetLinearVelocity() const;
            Vector3 GetAngularVelocity() const;
            void SetAngularFactor(const Vector3& Factor);
            Vector3 GetAngularFactor() const;
            void SetSleepWhenInactive(bool Flag);
        
            /* Misc functions */
            void SubscribeToInput();
            void Send(const Event& E);
        
            Component* GetComponentByType(COMPONENT_TYPE Type);
        
            UID GetID() const { return m_UID; }
            PropertyList* GetDef() const { return m_Definition; }
            
        protected:
            friend class EntityManager;
            friend class Engine;
            Component* m_Components[CT_COUNT];
            PropertyList* m_Definition;
            UID m_UID;
            i32 m_RefCount;
    };
    
    class EntityManager
    {
        public:
            EntityManager();
            ~EntityManager();
        
            Entity* CreateEntity();
            void DestroyEntity(Entity* E);
        
            void AddComponent(Entity* E,Component* Comp);
            void RemoveComponent(Entity* E,Component* Comp);
            void RemoveComponentByType(Entity* E,COMPONENT_TYPE Type);
        
            i32 GetEntityCount() const { return (i32)m_Entities.size(); }
            Entity* GetEntity(i32 Idx) const { return m_Entities[Idx]; }
        
        protected:
            vector<Entity*> m_Entities;
            Engine* m_Engine;
    };
};