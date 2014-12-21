#include <Rendering/Material.h>

namespace SandboxSimulator
{
    Material::Material() : m_Albedo(new MaterialInput<Vec3>()),
        m_Normal(new MaterialInput<Vec3>()),
        m_Emissive(new MaterialInput<Vec3>()),
        m_Metallic(new MaterialInput<f32>()),
        m_Roughness(new MaterialInput<f32>()),
        m_Specular(new MaterialInput<f32>()),
        m_Opacity(new MaterialInput<f32>())
    {

    }

    Material::~Material()
    {
        delete m_Albedo;
        delete m_Normal;
        delete m_Emissive;
        delete m_Metallic;
        delete m_Roughness;
        delete m_Specular;
        delete m_Opacity;
    }
}
