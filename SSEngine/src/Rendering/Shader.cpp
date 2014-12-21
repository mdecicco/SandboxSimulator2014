#include <Rendering/Shader.h>
#include <iostream>

namespace SandboxSimulator
{
    Shader::Shader() {}

    Shader::~Shader() {}

    bool Shader::Load(const char *Vert, const char* Frag)
    {
        //std::cout << Frag << std::endl;
        m_VS = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_VS, 1, &Vert, NULL);
        glCompileShader(m_VS);
        GLint status;
        glGetShaderiv(m_FS, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(m_VS, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar* strInfoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(m_VS, infoLogLength, NULL, strInfoLog);
            printf("Compilation error in vertex shader: %s\n", strInfoLog);
        }

        m_FS = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_FS, 1, &Frag, NULL);
        glCompileShader(m_FS);
        glGetShaderiv(m_FS, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(m_FS, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar* strInfoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(m_FS, infoLogLength, NULL, strInfoLog);
            printf("Compilation error in fragment shader: %s\n", strInfoLog);
        }

        m_Ptr = glCreateProgram();
        glAttachShader(m_Ptr, m_FS);
        glAttachShader(m_Ptr, m_VS);
        glLinkProgram(m_Ptr);
        return true;
    }

    void Shader::Enable()
    {
        glUseProgram(m_Ptr);
    }

    void Shader::Disable()
    {
        glUseProgram(0);
    }
}
