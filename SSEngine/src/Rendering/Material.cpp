#include <Rendering/Material.h>

namespace SandboxSimulator
{
    Material::Material()
    {}

    Material::~Material()
    {}

    void Material::AddComponent(MaterialComponent *Comp)
    {
        if(!HasComponentType(Comp->GetType()))
            m_Components.push_back(Comp);
    }

    bool Material::HasComponentType(i32 Type)
    {
        for(i32 i = 0; i < m_Components.size(); i++)
            if(m_Components[i]->GetType() == Type)
                return true;

        return false;
    }

    MaterialComponent* Material::GetComponentByType(i32 Type)
    {
        for(i32 i = 0; i < m_Components.size(); i++)
            if(m_Components[i]->GetType() == Type)
                return m_Components[i];

        return nullptr;
    }
}
