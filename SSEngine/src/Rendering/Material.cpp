#include <Rendering/Material.h>

namespace SandboxSimulator
{
    Material::Material() : m_NeedsUpdate(true), m_Shader(new Shader())
    {
		for(i32 i = 0; i < MI_COUNT; i++)
		{
			m_Inputs[i] = new MaterialNodeInput();
			switch(i)
			{
				case MI_ALBEDO:
				{
					m_Inputs[i]->m_VarType = "vec3";
					break;
				}
				case MI_NORMAL:
				{
					m_Inputs[i]->m_VarType = "vec3";
					break;
				}
				case MI_EMISSIVE:
				{
					m_Inputs[i]->m_VarType = "vec3";
					break;
				}
				default:
				{
					m_Inputs[i]->m_VarType = "float";
					break;
				}
			}
		}
    }

    void Material::SetInput(MATERIAL_INPUT Type, MaterialNodeOutput *Out)
    {
        m_Inputs[Type]->ConnectInput(Out);
    }

    void Material::GenerateShader()
    {
        if(m_NeedsUpdate) {
            string vertex_shader =
            "#version 400\n"
            "in vec3 vp;"
            "void main () {"
            "  gl_Position = vec4 (vp, 1.0);"
            "}";

            string fragment_shader = "#version 400\n";
            //inject uniforms and functions
            for(i32 i = 0; i < MI_COUNT; i++)
            {
				if(m_Inputs[i]->IsConnected()) 
				{
					fragment_shader += m_Inputs[i]->Input->Parent->GetUniformDefinitions();
					fragment_shader += m_Inputs[i]->Input->Parent->GetFunctionDefinitions();
				}
            }

            fragment_shader += "out vec4 frag_colour;"
                               "void main () {"
                               "vec3 Albedo = vec3(1,1,1);"
                               "float Opacity = 1.0;";

            //inject into main function
            for(i32 i = 0; i < MI_COUNT; i++)
            {
                if(m_Inputs[i]->IsConnected()) 
				{
                    switch(i)
                    {
                        case MI_ALBEDO:
                        {
                            fragment_shader += "Albedo = "+m_Inputs[i]->Input->VarName+";";
                            break;
                        }
                        case MI_OPACITY:
                        {
                            fragment_shader += "Opacity = "+m_Inputs[i]->Input->VarName+";";
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                }
            }

            //Rest of the shader (Where things like filling deferred buffers will happen)
            fragment_shader += "frag_colour = vec4(Albedo,Opacity);"
                                "}";
            m_Shader->Load(vertex_shader.c_str(), fragment_shader.c_str());

            m_Shader->Enable();
            for(i32 i = 0; i < MI_COUNT; i++)
            {
                if(m_Inputs[i]->IsConnected())
                    m_Inputs[i]->Input->Parent->GetUniforms(m_Shader);
            }
            m_Shader->Disable();

            m_NeedsUpdate = false;
        }
    }

    void Material::Bind()
    {
        GenerateShader();
        if(m_Shader)
            m_Shader->Enable();

        for(i32 i = 0; i < MI_COUNT; i++)
        {
            if(m_Inputs[i]->IsConnected())
                m_Inputs[i]->Input->Parent->BindUniforms(m_Shader);
        }
    }

    void Material::Unbind()
    {
        if(m_Shader)
            m_Shader->Disable();
    }
}
