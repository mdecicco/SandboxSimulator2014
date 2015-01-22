#include <Rendering/Gui/Gui.h>
#include <Rendering/RenderSystem.h>
#include <Core/SceneGraph.h>

namespace SandboxSimulator
{
    Entity* GuiElement::GetOwner()
    {
        return m_Component->GetParent();
    }

    Vec2 GuiElement::GetPosition()
    {
        if(m_Component->GetParent()->HasComponentType(CT_TRANSFORM))
            return ((TransformComponent*)m_Component->GetParent()->GetComponentByType(CT_TRANSFORM))->GetPosition().xy();
        return Vec2(0,0);
    }

    void GuiElement::GenerateMesh()
    {
        if(m_NeedsUpdate) {
            Mesh* mesh = new Mesh();
            Vec2 Pos = Vec2(0,0);
            mesh->AddVertex(Vec3(Pos.x,Pos.y, 0));
            mesh->AddTexCoord(Vec2(0,0));

            mesh->AddVertex(Vec3(Pos.x,Pos.y + m_Size.y,0));
            mesh->AddTexCoord(Vec2(0,1));

            mesh->AddVertex(Vec3(Pos.x + m_Size.x,Pos.y,0));
            mesh->AddTexCoord(Vec2(1,0));


            mesh->AddVertex(Vec3(Pos.x + m_Size.x,Pos.y,0));
            mesh->AddTexCoord(Vec2(1,0));

            mesh->AddVertex(Vec3(Pos.x,Pos.y + m_Size.y,0));
            mesh->AddTexCoord(Vec2(0,1));

            mesh->AddVertex(Vec3(Pos + m_Size,0));
            mesh->AddTexCoord(Vec2(1,1));

            //if(m_Font)
                //m_Font->AddToMesh(mesh, InnerText_,Vec3(0,0,0),(i32)m_Size.x, FontSize_);

            m_Component->SetMesh(mesh);
            m_NeedsUpdate = false;
        }
    }

    GUIManager::GUIManager(Font* F) : m_Font(F)
    {}

    GUIManager::~GUIManager()
    {}

    GuiElement* GUIManager::MakeElement(RenderComponent* r)
    {
        GuiElement* Ele = new GuiElement(r);
        m_Elements.push_back(std::move(ElementPtr(Ele)));
        return Ele;
    }

    void GUIManager::AddElement(GuiElement* Ele)
    {
        m_Elements.push_back(std::move(ElementPtr(Ele)));
    }

    void GUIManager::RemoveElement(GuiElement* E)
    {

    }

    void GUIManager::OnMouseDown(i32 Button, Vec2 cPos)
    {
        for(i32 i = (i32)m_Elements.size() - 1;i >= 0;i--)
        {
            RenderComponent* r = (RenderComponent*)m_Elements[i].get()->GetOwner()->GetComponentByType(CT_RENDER);
            if(r->IsHidden()) continue;
            Vec2 Pos = m_Elements[i].get()->GetPosition();
            Vec2 Sz  = m_Elements[i].get()->GetSize();
            if(cPos.x > Pos.x && cPos.x < Pos.x + Sz.x
            && cPos.y > Pos.y && cPos.y < Pos.y + Sz.y)
            {
                m_Elements[i].get()->OnMouseDown(Button, cPos);
                break;
            }
        }
    }

    void GUIManager::OnMouseUp(i32 Button, Vec2 cPos)
    {
        for(i32 i = (i32)m_Elements.size() - 1;i >= 0;i--)
        {
            RenderComponent* r = (RenderComponent*)m_Elements[i].get()->GetOwner()->GetComponentByType(CT_RENDER);
            if(r->IsHidden()) continue;
            Vec2 Pos = m_Elements[i].get()->GetPosition();
            Vec2 Sz  = m_Elements[i].get()->GetSize();
            if(cPos.x > Pos.x && cPos.x < Pos.x + Sz.x
            && cPos.y > Pos.y && cPos.y < Pos.y + Sz.y)
            {
                m_Elements[i].get()->OnMouseUp(Button, cPos);
                break;
            }
        }
    }

    void GUIManager::GetVisibleEntities(RenderList* List)
    {
        for(i32 i = 0;i < m_Elements.size();i++)
        {
            RenderComponent* r = (RenderComponent*)m_Elements[i].get()->GetOwner()->GetComponentByType(CT_RENDER);
            if(!m_Elements[i]->GetOwner()->HasComponentType(CT_RENDER)) continue;
            if(!r->GetMesh()) continue;
            if(r->IsHidden()) continue;
            if(r->GetMesh()->NeedsSync())
            if(r->GetVertexCount() == 0) continue;
            List->AddEntity(m_Elements[i].get()->GetOwner());
        }
    }
}