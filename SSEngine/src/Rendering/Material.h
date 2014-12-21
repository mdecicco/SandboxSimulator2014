#ifndef SanboxSimulator_Material_h
#define SanboxSimulator_Material_h

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

#include <System/SSTypes.h>
#include <Utils/Math.h>

namespace SandboxSimulator
{
    class MaterialNode
    {
        public:
            MaterialNode() {}
            virtual ~MaterialNode() {}

            virtual void Serialize() = 0;
            virtual void Deserialize() = 0;

            virtual void UpdateOuts() = 0;
    };

    template <class T>
    class MaterialOutput
    {
        public:
            MaterialOutput(T Data, MaterialNode* Node = nullptr) : m_Owner(Node), m_Data(Data) {}
            void SetData(T Data) {m_Data = Data;}
            T GetData() {if(m_Owner) m_Owner->UpdateOuts(); return m_Data;}
        protected:
            T m_Data;
            MaterialNode* m_Owner;
    };

    template <class T>
    class MaterialInput
    {
        public:
            MaterialInput() {}
            void SetInput(MaterialOutput<T>* In) {m_Input = In;}
            T GetData() {if(m_Input) return m_Input->GetData(); return (T)0;}

        protected:
            MaterialOutput<T>* m_Input;
    };

    /* The actual material information that gets passed to the shader */
    class Material : public MaterialNode
    {
        public:
            Material();
            ~Material();

            virtual void Serialize() {}
            virtual void Deserialize() {}
            virtual void UpdateOuts() {}

            MaterialInput<Vec3>* m_Albedo;
            MaterialInput<Vec3>* m_Normal;
            MaterialInput<Vec3>* m_Emissive;

            MaterialInput<f32>* m_Specular;
            MaterialInput<f32>* m_Roughness;
            MaterialInput<f32>* m_Opacity;
            MaterialInput<f32>* m_Metallic;
    };

    //Some Nodes for basic materials
    class ConstVec3Node : public MaterialNode
    {
        public:
            ConstVec3Node() : m_Output(new MaterialOutput<Vec3>(Vec3(), this)) {}
            virtual void Serialize() {}
            virtual void Deserialize() {}

            virtual void UpdateOuts() {}

            MaterialOutput<Vec3>* m_Output;
    };

    class MultiplyNode : public MaterialNode
    {
        public:
            MultiplyNode() : m_AInput(new MaterialInput<f32>()), m_BInput(new MaterialInput<f32>()), m_Output(new MaterialOutput<f32>(0.0f, this))
            {

            }

            virtual void Serialize() {}
            virtual void Deserialize() {}

            virtual void UpdateOuts()
            {
                m_Output->SetData(m_AInput->GetData() * m_BInput->GetData());
            }

            MaterialInput<f32>* m_AInput;
            MaterialInput<f32>* m_BInput;
            MaterialOutput<f32>* m_Output;
    };
}

#endif
