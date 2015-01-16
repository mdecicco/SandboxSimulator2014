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
    /* The actual material information that gets passed to the shader */
    class Material
    {
        public:
            Material();
            ~Material();

            void Bind();
            void GenerateShader();
            void Unbind();

            Shader* GetShader() {return m_Shader;}

        protected:
            Shader* m_Shader;
            bool m_NeedsUpdate;
    };
}

#endif
