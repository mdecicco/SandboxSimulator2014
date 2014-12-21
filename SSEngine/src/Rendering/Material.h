#ifndef SandboxSimulator_Material_h
#define SandboxSimulator_Material_h

#include <vector>
#include <string>


/*
 * Fair Warninig: Procede with caution.
 *
 * I have been drinking, scotch, beer, some shots of... I don't remember.
 * This code is likely a hazard area, if you so choose to proceed, there are likely
 * hazards to your health, sanity, and longevity. Have a nice day.
 *
 * I need a template wizard to do this for me... or for me to be able to think straight
 *
 */

#include <Rendering/Shader.h>
#include <System/SSTypes.h>
#include <Utils/Math.h>
#include <string>

using namespace std;

namespace SandboxSimulator
{
    enum MATERIAL_INPUT
    {
        MI_ALBEDO,
        MI_NORMAL,
        MI_EMISSIVE,
        MI_METALLIC,
        MI_ROUGHNESS,
        MI_OPACITY,
        MI_SPECULAR,
        MI_COUNT
    };

    class MaterialNode;
    class MaterialNodeOutput
    {
        public:
            MaterialNodeOutput(MaterialNode* Par) : Parent(Par), VarName(""), VarType("") {}
            ~MaterialNodeOutput() {}

            MaterialNode* Parent;

            string VarName;
            string VarType;
    };

    class MaterialNodeInput
    {
        public:
            MaterialNodeInput() : m_VarType(""), m_Connected(false) {}
            ~MaterialNodeInput() {}

            void ConnectInput(MaterialNodeOutput* o)
            {
                if(m_VarType == o->VarType)
                {
                    Input = o;
                    m_Connected = true;
                }
            }

            bool IsConnected() {return m_Connected;}

        protected:
            friend class MaterialNode;
            friend class Material;
            string m_VarType;

        private:
            bool m_Connected;
            MaterialNodeOutput* Input;
    };

    class MaterialNode
    {
        public:
            MaterialNode() {}
            virtual ~MaterialNode() {}

            virtual void BindUniforms(Shader* shader) = 0;
            virtual void GetUniforms(Shader* shader) = 0;

            string GetFunctionDefinitions() {return m_FunctionDefinitions;} // force overide and nest this later
            string GetUniformDefinitions() {return m_UniformDefinitions;} // same here
            string GetMainLogic() {return m_MainLogic;} // and here

        protected:
            string m_UniformDefinitions;
            string m_FunctionDefinitions;
            string m_MainLogic;
    };

    /* The actual material information that gets passed to the shader */
    class Material
    {
        public:
            Material();
            ~Material();

            void Bind();
            void GenerateShader();
            void Unbind();

            void SetInput(MATERIAL_INPUT Type, MaterialNodeOutput* Out);

        protected:
            MaterialNodeInput* m_Inputs[MI_COUNT];
            Shader* m_Shader;
            bool m_NeedsUpdate;
    };

    //Some test nodes to work with
    class MatVec3Node : MaterialNode
    {
        public:
            MatVec3Node(string name, Vec3 Data = Vec3(0.0)) : m_Data(Data), m_Output(new MaterialNodeOutput(this))
            {
                m_Output->VarName = name;
                m_Output->VarType = "vec3";
                m_UniformDefinitions = "uniform vec3 "+name+";";
                m_FunctionDefinitions = "";
                m_MainLogic = "";
            }

            ~MatVec3Node()
            {
                delete m_Output;
            }

            virtual void GetUniforms(Shader* Shdr)
            {
                m_UniformLoc = glGetUniformLocation(Shdr->GetPointer(), m_Output->VarName.c_str());
            }

            virtual void BindUniforms(Shader* Shdr)
            {
                glProgramUniform3f(Shdr->GetPointer(), m_UniformLoc, m_Data.x, m_Data.y, m_Data.z);
            }

            MaterialNodeOutput* GetOutput()
            {
                return m_Output;
            }

        protected:
            MaterialNodeOutput* m_Output;
            GLint m_UniformLoc;
            Vec3 m_Data;
    };
}

#endif
