#ifndef SandboxSimulator_Shader_h
#define SandboxSimulator_Shader_h

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
    };

    enum SHADER_UNIFORM
    {
        SU_MODEL_MATRIX,
        SU_NORMAL_MATRIX,
        SU_VIEW_MATRIX,
        SU_PROJECTION_MATRIX,
        SU_MVP_MATRIX,
    };

    class Shader
    {
        public:
            Shader();
            ~Shader();

            bool Load(const char* Path);

            void Enable();
            void Disable();

        protected:
            friend class RenderSystem;
            GLuint m_VS;
            GLuint m_FS;
            GLuint m_Ptr;
    };
}

#endif
