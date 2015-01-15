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
            "#version 330\n"
            "in vec3 vp;"
            "uniform mat4 Transform;"
            "uniform mat4 Projection;"
            "out vec3 NormalVert;"
            "void main () {"
            "  NormalVert = vec3(0,0,1);"
            "  gl_Position = Projection * Transform * vec4(vp, 1.0);"
            "}";

            string fragment_header = "";
            for(i32 i = 0; i < MI_COUNT; i++)
            {
				if(m_Inputs[i]->IsConnected()) 
				{
                    fragment_header += m_Inputs[i]->Input->Parent->GetUniformDefinitions();
                    fragment_header += m_Inputs[i]->Input->Parent->GetFunctionDefinitions();
				}
            }

            string fragment_body = "";
            for(i32 i = 0; i < MI_COUNT; i++)
            {
                if(m_Inputs[i]->IsConnected()) 
				{
                    switch(i)
                    {
                        case MI_ALBEDO:
                        {
                            fragment_body += "Albedo = "+m_Inputs[i]->Input->Getter+";";
                            break;
                        }
                        case MI_NORMAL:
                        {
                            fragment_body += "Normal = "+m_Inputs[i]->Input->Getter+";";
                            break;
                        }
                        case MI_EMISSIVE:
                        {
                            fragment_body += "Emissive = "+m_Inputs[i]->Input->Getter+";";
                            break;
                        }
                        case MI_ROUGHNESS:
                        {
                            fragment_body += "Roughness = "+m_Inputs[i]->Input->Getter+";";
                            break;
                        }
                        case MI_SPECULAR:
                        {
                            fragment_body += "Specular = "+m_Inputs[i]->Input->Getter+";";
                            break;
                        }
                        case MI_METALLIC:
                        {
                            fragment_body += "Metallic = "+m_Inputs[i]->Input->Getter+";";
                            break;
                        }
                        case MI_OPACITY:
                        {
                            fragment_body += "Opacity = "+m_Inputs[i]->Input->Getter+";";
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                }
            }

            //Shader "Template" with that information injected
            string fragment_shader = "#version 330\n";
            fragment_shader +=
                    fragment_header +
                    "in vec3 NormalVert;"
                    "out vec4 frag_colour;"
                    "void main () {"
                    "    vec3 Albedo = vec3(1,1,1);"
                    "    vec3 Normal = NormalVert;"
                    "    vec3 Emissive = vec3(1,1,1);"
                    "    float Metallic  = 0.0;"
                    "    float Roughness = 0.5;"
                    "    float Specular  = 0.5;"
                    "    float Opacity   = 1.0;"
                    +    fragment_body +
                    "    frag_colour = vec4(Albedo,Opacity);"
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
