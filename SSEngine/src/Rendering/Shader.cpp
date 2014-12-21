#include <Rendering/Shader.h>

namespace SandboxSimulator
{
    Shader::Shader() {}

    Shader::~Shader() {}

    bool Shader::Load(const char *Path)
    {
        const char* vertex_shader =
        "#version 400\n"
        "in vec3 vp;"
        "void main () {"
        "  gl_Position = vec4 (vp, 1.0);"
        "}";

        const char* fragment_shader =
        "#version 400\n"
        "uniform vec3 Albedo;"
        "uniform float Opacity;"
        "out vec4 frag_colour;"
        "void main () {"
        "  vec3 outc = Albedo * Opacity;"
        "  frag_colour = vec4 (outc, 1.0);"
        "}";

        m_VS = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_VS, 1, &vertex_shader, NULL);
        glCompileShader(m_VS);
        m_FS = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_FS, 1, &fragment_shader, NULL);
        glCompileShader(m_FS);
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
