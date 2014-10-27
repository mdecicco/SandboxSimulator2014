/*
 *  Shader.h
 *
 *  Created by Michael DeCicco on 7/7/14.
 *
 */

#pragma once
#include "AssetManager.h"
#include "glfw3.h"

namespace Reality
{
    enum VERTEX_ATTRIBUTE
    {
        VA_POSITION,
        VA_NORMAL,
        VA_TANGENT,
        VA_TEXCOORD,
        VA_BONEIDX,
        VA_WEIGHT
    };
    
    enum SHADER_UNIFORM
    {
        SU_MODEL_MATRIX,
        SU_NORMAL_MATRIX,
        SU_VIEW_MATRIX,
        SU_PROJECTION_MATRIX,
        SU_MVP_MATRIX,
        SU_BONE0,
        SU_BONE1,
        SU_BONE2,
        SU_BONE3,
        SU_BONE4,
        SU_BONE5,
        SU_BONE6,
        SU_BONE7,
        SU_BONE8,
        SU_BONE9,
        SU_BONE10,
        SU_BONE11,
        SU_BONE12,
        SU_BONE13,
        SU_BONE14,
        SU_BONE15,
        SU_BONE16,
        SU_BONE17,
        SU_BONE18,
        SU_BONE19,
        SU_BONE20,
        SU_BONE21,
        SU_BONE22,
        SU_BONE23,
        SU_BONE24,
        SU_BONE25,
        SU_BONE26,
        SU_BONE27,
        SU_BONE28,
        SU_BONE29,
        SU_BONE30,
        SU_BONE31,
        SU_BONE32,
        SU_BONE33,
        SU_BONE34,
        SU_BONE35,
        SU_BONE36,
        SU_BONE37,
        SU_BONE38,
        SU_BONE39,
        SU_BONE40,
        SU_BONE41,
        SU_BONE42,
        SU_BONE43,
        SU_BONE44,
        SU_BONE45,
        SU_BONE46,
        SU_BONE47,
        SU_BONE48,
        SU_BONE49,
        SU_BONE50,
        SU_EXPOSURE,
        SU_NEAR_PLANE,
        SU_FAR_PLANE,
        SU_TIME_OF_DAY,
        SU_SUN_POSITION,
        SU_TEXTURE0,
        SU_TEXTURE1,
        SU_TEXTURE2,
        SU_TEXTURE3,
        SU_TEXTURE4,
        SU_TEXTURE5,
        SU_TEXTURE6,
        SU_TEXTURE7,
        SU_TEXTURE8,
        SU_COUNT
    };
    
    class Shader : public Asset
    {
        public:
            Shader();
            ~Shader();
        
            virtual bool Reload();
        
            void Enable();
            void Disable();
        
            i32 GetListID() const { return m_ListID; }
            GLuint GetPID() const { return m_PID; }
        
            i32 GetUniformLoc(SHADER_UNIFORM u) const { return m_UniformLocs[u]; }
        
        protected:
            friend class RenderList;
            GLuint m_VS;
            GLuint m_PS;
            GLuint m_PID;
            i32 m_ListID;
            i32 m_UniformLocs[SU_COUNT];
    };
};