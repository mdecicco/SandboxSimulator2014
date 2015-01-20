#include <Rendering/Gui/Gui.h>
#include <Rendering/RenderSystem.h>

namespace SandboxSimulator
{
    GUIManager::GUIManager()
    {
    }
    GUIManager::~GUIManager()
    {
    }

    void GUIManager::OnMouseLeftDown(const Vec2& cPos)
    {
        //ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = (i32)m_Entities.size() - 1;i >= 0;i--)
        {
            //if(!m_Entities[i]->ReceivesInput()) continue;
            //ScriptComponent* Scpt = GetScriptComponent(m_Entities[i]);
            TransformComponent* t = (TransformComponent*)m_Entities[i]->GetComponentByType(CT_TRANSFORM);
            if(!m_Entities[i]->HasComponentType(CT_TRANSFORM)) continue;
            RenderComponent* r = (RenderComponent*)m_Entities[i]->GetComponentByType(CT_RENDER);
            if(!m_Entities[i]->HasComponentType(CT_RENDER)) continue;
            if(r->IsHidden()) continue;
            Vec2 Pos = t->GetPosition(false).xy();
            Vec2 Sz  = r->GetGuiData()->Dimensions;
            if(cPos.x > Pos.x && cPos.x < Pos.x + Sz.x
            && cPos.y > Pos.y && cPos.y < Pos.y + Sz.y)
            {
                //Onclick
                //Sys->OnClickDown(Scpt,m_Entities[i],cPos - Pos);
                break;
            }
        }
    }
    void GUIManager::OnMouseLeftUp(const Vec2& cPos)
    {
        //ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = (i32)m_Entities.size() - 1;i >= 0;i--)
        {
            //if(!m_Entities[i]->ReceivesInput()) continue;
            //ScriptComponent* Scpt = GetScriptComponent(m_Entities[i]);
            //if(!Scpt) continue;
            TransformComponent* t = (TransformComponent*)m_Entities[i]->GetComponentByType(CT_TRANSFORM);
            if(!m_Entities[i]->HasComponentType(CT_TRANSFORM)) continue;
            RenderComponent* r = (RenderComponent*)m_Entities[i]->GetComponentByType(CT_RENDER);
            if(!m_Entities[i]->HasComponentType(CT_RENDER)) continue;
            if(r->IsHidden()) continue;
            Vec2 Pos = t->GetPosition(false).xy();
            Vec2 Sz  = r->GetGuiData()->Dimensions;
            if(cPos.x > Pos.x && cPos.x < Pos.x + Sz.x
            && cPos.y > Pos.y && cPos.y < Pos.y + Sz.y)
            {
                //On...unclick?
                //Sys->OnClickUp(Scpt,m_Entities[i],cPos - Pos);
                break;
            }
        }
    }
    void GUIManager::OnMouseRightDown(const Vec2& Pos)
    {
    }
    void GUIManager::OnMouseRightUp(const Vec2& Pos)
    {
    }
    void GUIManager::AddEntity(Entity* E)
    {
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        if(!r) return;
        if(r->GetGuiData()) return;
        r->SetGuiData(new GUIElementData());
        r->GetGuiData()->ListPos = (i32)m_Entities.size();
        r->GetGuiData()->IsVisible = true;
        Vec3 min,max;
        r->GetMesh()->GetMinMaxVertices(min,max);
        r->GetGuiData()->Dimensions = (max - min).xy();
        m_Entities.push_back(E);
    }
    void GUIManager::RemoveEntity(Entity* E)
    {
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        if(!r) return;
        if(!r->GetGuiData()) return;
        m_Entities.erase(m_Entities.begin() + r->GetGuiData()->ListPos);
        for(i32 i = r->GetGuiData()->ListPos;i < m_Entities.size();i++)
        {
            ((RenderComponent*)m_Entities[i]->GetComponentByType(CT_RENDER))->GetGuiData()->ListPos--;
        }
    }
    void GUIManager::GetVisibleEntities(RenderList* List)
    {
        for(i32 i = 0;i < m_Entities.size();i++)
        {
            RenderComponent* r = (RenderComponent*)m_Entities[i]->GetComponentByType(CT_RENDER);
            if(!m_Entities[i]->HasComponentType(CT_RENDER)) continue;
            if(!r->GetMesh()) continue;
            if(r->IsHidden()) continue;
            if(r->GetMesh()->NeedsSync())
            {
                //Vertices adjusted?
                r->AcquireDimensions();
                r->GetGuiData()->Dimensions = r->GetDimensions().xy();
            }
            if(r->GetVertexCount() == 0) continue;
            List->AddEntity(m_Entities[i]);
        }
    }
}