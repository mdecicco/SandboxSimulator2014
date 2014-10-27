/*
 *  Lights.h
 *
 *  Created by Michael DeCicco on 9/9/14.
 *
 */

#pragma once
#include "EntityManager.h"
#include "EngineMath.h"

namespace Reality
{
    class LightComponent : public Component
    {
        public:
            enum LIGHT_TYPE
            {
                LT_POINT,
                LT_SPOT,
                LT_DIRECTIONAL,
            };
            LightComponent();
            ~LightComponent() { }
        
            LIGHT_TYPE m_Type;
        
            Scalar m_Intensity;
            Scalar m_ConstantAttenuation;
            Scalar m_LinearAttenuation;
            Scalar m_QuadraticAttenuation;
            Scalar m_CutoffAngle;
            Scalar m_Exponent;
        
            Vector4 m_Diffuse;
            Vector4 m_Ambient;
            Vector4 m_Specular;
    };
};