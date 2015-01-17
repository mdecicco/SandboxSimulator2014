#ifndef SandboxSimulator_Shader_h
#define SandboxSimulator_Shader_h

#include <System/File.h>
#include <System/PlatformIncludes.h>
#include <GLFW/glfw3.h>

namespace SandboxSimulator
{
    enum VERTEX_ATTRIBUTE
    {
        VA_POSITION,
        VA_NORMAL,
        VA_TANGENT,
        VA_TEXCOORD,
        VA_COUNT
    };

    enum SHADER_UNIFORM
    {
        SU_MODEL_MATRIX,
        SU_NORMAL_MATRIX,
        SU_VIEW_MATRIX,
        SU_PROJECTION_MATRIX,
        SU_MVP_MATRIX,
        SU_EXPOSURE,
        SU_NEAR_PLANE,
        SU_FAR_PLANE,
        SU_CAMERA_POSITION,
        SU_RESOLUTION,
        SU_RUNTIME,
        SU_TIME_OF_DAY,
        SU_SUN_POSITION,
        SU_OPACITY,
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
        
            virtual bool OnLoad();
            bool LoadShader();
        
            void Enable();
            void Disable();
        
            i32 GetListID() const { return m_ListID; }
            GLuint GetPID() const { return m_PID; }
        
            i32 GetUniformLoc(SHADER_UNIFORM u) const { return m_UniformLocs[u]; }
            i32 GetUniformLoc(std::string Name) const { if(!m_PID) { return -1; } return glGetUniformLocation(m_PID,Name.c_str()); }
        
        protected:
            friend class RenderList;
            GLuint m_VS;
            GLuint m_PS;
            GLuint m_PID;
            i32 m_ListID;
            i32 m_UniformLocs[SU_COUNT];

            bool m_Loaded;
    };
}

#endif
