/*
 *  GUIManager.cpp
 *
 *  Created by Michael DeCicco on 9/9/14.
 *
 */

#include "GUIManager.h"
#include "Engine.h"

namespace Reality
{
    GUIManager::GUIManager()
    {
    }
    GUIManager::~GUIManager()
    {
    }

    void GUIManager::OnMouseLeftDown(const Vector2& cPos)
    {
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_Entities.size();i++)
        {
            ScriptComponent* Scpt = GetScriptComponent(m_Entities[i]);
            if(!Scpt) continue;
            RenderComponent* r = GetRenderComponent(m_Entities[i]);
            Vector2 Pos = m_Entities[i]->GetPosition().xy();
            Vector2 Sz  = r->m_GUIData->Dimensions;
            if(cPos.x > Pos.x && cPos.x < Pos.x + Sz.x
            && cPos.y > Pos.y && cPos.y < Pos.y + Sz.y)
            {
                Sys->OnClickDown(Scpt,m_Entities[i],cPos);
            }
        }
    }
    void GUIManager::OnMouseLeftUp(const Vector2& cPos)
    {
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        for(i32 i = 0;i < m_Entities.size();i++)
        {
            RenderComponent* r = GetRenderComponent(m_Entities[i]);
            Vector2 Pos = m_Entities[i]->GetPosition().xy();
            Vector2 Sz  = r->m_GUIData->Dimensions;
            if(cPos.x > Pos.x && cPos.x < Pos.x + Sz.x
            && cPos.y > Pos.y && cPos.y < Pos.y + Sz.y)
            {
                ScriptComponent* Scpt = GetScriptComponent(m_Entities[i]);
                if(!Scpt) continue;
                Sys->OnClickUp(Scpt,m_Entities[i],cPos - Pos);
            }
        }
    }
    void GUIManager::OnMouseRightDown(const Vector2& Pos)
    {
    }
    void GUIManager::OnMouseRightUp(const Vector2& Pos)
    {
    }
    void GUIManager::AddEntity(Entity* E)
    {
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        if(!r) return;
        if(r->m_GUIData) return;
        r->m_GUIData = new GUIElementData();
        r->m_GUIData->ListPos = (i32)m_Entities.size();
        r->m_GUIData->IsVisible = true;
        Vector3 min,max;
        r->GetMesh()->GetMinMaxVertices(min,max);
        r->m_GUIData->Dimensions = (max - min).xy();
        m_Entities.push_back(E);
    }
    void GUIManager::RemoveEntity(Entity* E)
    {
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        if(!r) return;
        if(!r->m_GUIData) return;
        m_Entities.erase(m_Entities.begin() + r->m_GUIData->ListPos);
        for(i32 i = r->m_GUIData->ListPos;i < m_Entities.size();i++)
        {
            ((RenderComponent*)m_Entities[i]->GetComponentByType(CT_RENDER))->m_GUIData->ListPos--;
        }
    }
    void GUIManager::GetVisibleEntities(RenderList* List)
    {
        for(i32 i = 0;i < m_Entities.size();i++)
        {
            RenderComponent* r = GetRenderComponent(m_Entities[i]);
            if(r->GetMesh()->NeedsSync())
            {
                //Vertices adjusted?
                r->AcquireDimensions();
                r->m_GUIData->Dimensions = r->GetDimensions().xy();
            }
            List->AddEntity(m_Entities[i]);
        }
    }
};