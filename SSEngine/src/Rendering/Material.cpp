#include <Rendering/Material.h>

namespace SandboxSimulator
{
    Material::Material() : m_NeedsUpdate(true), m_Shader(new Shader())
    {}

    void Material::GenerateShader()
    {
        if(m_NeedsUpdate) {
            string vertex_shader =
            "#version 330\n"
            "in vec3 vp;"
            "uniform mat4 Transform;"
            "uniform mat4 Projection;"
            "out vec3 NormalVert;"
            "void main () {"
            "  NormalVert = vec3(0,0,1);"
            "  gl_Position = Projection * Transform * vec4(vp, 1.0);"
            "}";

            string fragment_shader = 
            "#version 330\n"
            "in vec3 NormalVert;"
            "out vec4 frag_colour;"
            "void main () {"
            "    vec3 Albedo = vec3(0,0,1);"
            "    frag_colour = vec4(Albedo,1.0);"
            "}";
            m_Shader->Load(vertex_shader.c_str(), fragment_shader.c_str());

            m_NeedsUpdate = false;
        }
    }

    void Material::Bind()
    {
        GenerateShader();
        if(m_Shader)
            m_Shader->Enable();
    }

    void Material::Unbind()
    {
        if(m_Shader)
            m_Shader->Disable();
    }
}
