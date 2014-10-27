/*
 *  Event.h
 *
 *  Created by Michael DeCicco on 9/10/14.
 *
 */

#pragma once
#include "EngineMath.h"
#include <vector>

namespace Reality
{
    class Event
    {
        public:
            Event(i32 EventID) : m_EventID(EventID) { }
            Event(const Event& E);
            ~Event();
        
            i32 ID() const { return m_EventID; }
        
            void AddParami(i32 p);
            void AddParamf(Scalar p);
            void AddParamd(f64 p);
            void AddParamv2(const Vector2& p);
            void AddParamv3(const Vector3& p);
            void AddParamv4(const Vector4& p);
            void AddParamq(const Quaternion& p);
            void AddParamm(const Matrix4& p);
            void AddParams(const string& p);
            i32 GetParami(i32 Idx) const;
            Scalar GetParamf(i32 Idx) const;
            f64 GetParamd(i32 Idx) const;
            Vector2 GetParamv2(i32 Idx) const;
            Vector3 GetParamv3(i32 Idx) const;
            Vector4 GetParamv4(i32 Idx) const;
            Quaternion GetParamq(i32 Idx) const;
            Matrix4 GetParamm(i32 Idx) const;
            string GetParams(i32 Idx) const;
        
        protected:
            vector<void*> m_Params;
            vector<i32>   m_ParamTypes;
            i32 m_EventID;
    };
};