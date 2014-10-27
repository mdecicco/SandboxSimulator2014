/*
 *  Event.cpp
 *
 *  Created by Michael DeCicco on 9/10/14.
 *
 */

#include "Event.h"

namespace Reality
{
    Event::Event(const Event& E)
    {
        for(i32 i = 0;i < E.m_Params.size();i++)
        {
            m_ParamTypes.push_back(E.m_ParamTypes[i]);
            if(m_ParamTypes[i] == 0) { m_Params.push_back(new i32(*(i32*)E.m_Params[i])); continue; }
            if(m_ParamTypes[i] == 1) { m_Params.push_back(new Scalar(*(Scalar*)E.m_Params[i])); continue; }
            if(m_ParamTypes[i] == 2) { m_Params.push_back(new f64(*(f64*)E.m_Params[i])); continue; }
            if(m_ParamTypes[i] == 3) { m_Params.push_back(new Vector2(*(Vector2*)E.m_Params[i])); continue; }
            if(m_ParamTypes[i] == 4) { m_Params.push_back(new Vector3(*(Vector3*)E.m_Params[i])); continue; }
            if(m_ParamTypes[i] == 5) { m_Params.push_back(new Vector4(*(Vector4*)E.m_Params[i])); continue; }
            if(m_ParamTypes[i] == 6) { m_Params.push_back(new Quaternion(*(Quaternion*)E.m_Params[i])); continue; }
            if(m_ParamTypes[i] == 7) { m_Params.push_back(new Matrix4(*(Matrix4*)E.m_Params[i])); continue; }
            if(m_ParamTypes[i] == 8) { m_Params.push_back(new string(*(string*)E.m_Params[i])); continue; }
        }
        m_EventID = E.m_EventID;
    }
    Event::~Event()
    {
        for(i32 i = 0;i < m_Params.size();i++)
        {
            if(m_ParamTypes[i] == 0) { delete (i32*)m_Params[i]; continue; }
            if(m_ParamTypes[i] == 1) { delete (Scalar*)m_Params[i]; continue; }
            if(m_ParamTypes[i] == 2) { delete (f64*)m_Params[i]; continue; }
            if(m_ParamTypes[i] == 3) { delete (Vector2*)m_Params[i]; continue; }
            if(m_ParamTypes[i] == 4) { delete (Vector3*)m_Params[i]; continue; }
            if(m_ParamTypes[i] == 5) { delete (Vector4*)m_Params[i]; continue; }
            if(m_ParamTypes[i] == 6) { delete (Quaternion*)m_Params[i]; continue; }
            if(m_ParamTypes[i] == 7) { delete (Matrix4*)m_Params[i]; continue; }
            if(m_ParamTypes[i] == 8) { delete (string*)m_Params[i]; continue; }
        }
    }

    void Event::AddParami(i32 p)
    {
        m_Params.push_back(new i32(p));
        m_ParamTypes.push_back(0);
    }
    void Event::AddParamf(Scalar p)
    {
        m_Params.push_back(new Scalar(p));
        m_ParamTypes.push_back(1);
    }
    void Event::AddParamd(f64 p)
    {
        m_Params.push_back(new f64(p));
        m_ParamTypes.push_back(2);
    }
    void Event::AddParamv2(const Vector2& p)
    {
        m_Params.push_back(new Vector2(p));
        m_ParamTypes.push_back(3);
    }
    void Event::AddParamv3(const Vector3& p)
    {
        m_Params.push_back(new Vector3(p));
        m_ParamTypes.push_back(4);
    }
    void Event::AddParamv4(const Vector4& p)
    {
        m_Params.push_back(new Vector4(p));
        m_ParamTypes.push_back(5);
    }
    void Event::AddParamq(const Quaternion& p)
    {
        m_Params.push_back(new Quaternion(p));
        m_ParamTypes.push_back(6);
    }
    void Event::AddParamm(const Matrix4& p)
    {
        m_Params.push_back(new Matrix4(p));
        m_ParamTypes.push_back(7);
    }
    void Event::AddParams(const string& p)
    {
        m_Params.push_back(new string(p));
        m_ParamTypes.push_back(8);
    }
    i32 Event::GetParami(i32 Idx) const
    {
        return *(i32*)m_Params[Idx];
    }
    Scalar Event::GetParamf(i32 Idx) const
    {
        return *(Scalar*)m_Params[Idx];
    }
    f64 Event::GetParamd(i32 Idx) const
    {
        return *(f64*)m_Params[Idx];
    }
    Vector2 Event::GetParamv2(i32 Idx) const
    {
        return *(Vector2*)m_Params[Idx];
    }
    Vector3 Event::GetParamv3(i32 Idx) const
    {
        return *(Vector3*)m_Params[Idx];
    }
    Vector4 Event::GetParamv4(i32 Idx) const
    {
        return *(Vector4*)m_Params[Idx];
    }
    Quaternion Event::GetParamq(i32 Idx) const
    {
        return *(Quaternion*)m_Params[Idx];
    }
    Matrix4 Event::GetParamm(i32 Idx) const
    {
        return *(Matrix4*)m_Params[Idx];
    }
    string Event::GetParams(i32 Idx) const
    {
        return *(string*)m_Params[Idx];
    }
};