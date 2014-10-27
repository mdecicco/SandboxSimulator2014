/*
 *  Lights.cpp
 *
 *  Created by Michael DeCicco on 9/9/14.
 *
 */

#include "Lights.h"

namespace Reality
{
    LightComponent::LightComponent() : Component(CT_LIGHT), m_Type(LT_POINT), m_Intensity(1.0f),
                                       m_ConstantAttenuation(1.0f), m_LinearAttenuation(1.0f),
                                       m_QuadraticAttenuation(1.0f), m_CutoffAngle(30.0f),
                                       m_Exponent(1.0f)
    {
    }
};