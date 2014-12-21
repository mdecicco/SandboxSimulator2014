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

#include <System/SSTypes.h>
#include <Utils/Math.h>

namespace SandboxSimulator
{
    class MaterialComponent
    {
        public:
            MaterialComponent() {}
            virtual ~MaterialComponent() {}
            i32 GetType() {return m_Type;}

        protected:
            i32 m_Type;
    };

    #define MATERIAL_ALBEDO_TYPE 1
    class MaterialAlbedoComponent : public MaterialComponent
    {
        public:
            MaterialAlbedoComponent() : m_Val(Vec3())
            {
                m_Type = MATERIAL_ALBEDO_TYPE;
            }

            virtual ~MaterialAlbedoComponent() {}

            Vec3 GetValue() {return m_Val;}
            void SetValue(Vec3 Val) {m_Val = Val;}

        protected:
            Vec3 m_Val;
    };

    /* The actual material information that gets passed to the shader */
    class Material
    {
        public:
            Material();
            ~Material();

            void AddComponent(MaterialComponent* Comp);
            bool HasComponentType(i32 Type);
            MaterialComponent* GetComponentByType(i32 Type);

        protected:
            std::vector<MaterialComponent*> m_Components;
    };
}

#endif
