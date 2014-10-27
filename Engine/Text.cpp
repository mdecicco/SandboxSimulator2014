/*
 *  Text.cpp
 *
 *  Created by Michael DeCicco on 9/12/14.
 *
 */

#include "Text.h"
#include "Engine.h"

#include <string>
using namespace std;
namespace Reality
{
    Font::Font()
    {
    }
    Font::~Font()
    {
        m_Texture->Destroy();
    }

    bool Font::Reload()
    {
        for(i32 i = 0;i < 256;i++) m_Glyphs[i].IsValid = false;
        //#charId=32 #uvStart=(0.3438, 0.1146) #uvEnd=(0.3438, 0.1146) #size=(0.0, 0.0) #xOffset=0 #yOffset=46 #xAdvance=14
        i32 CharID = 0;
        while(!AtEnd())
        {
            char c = GetC();
            if(c == '#')
            {
                if(string((CString)GetPtr(),11) == "textureFile")
                {
                    i32 iPos = GetPosition() + 13;
                    i32 nPos = iPos;
                    SetPosition(iPos);
                    
                    for(;!AtEnd() && GetC() != '\"';nPos++) { }
                    SetPosition(iPos);
                    string str((CString)GetPtr(),nPos - iPos);
                    
                    m_Texture = (Texture*)Engine::GetEngine()->GetAssetManager()->LoadAsset(str.c_str(),AT_TEXTURE);
                    SetPosition(nPos);
                }
                if(string((CString)GetPtr(),6) == "charId")
                {
                    SetPosition(GetPosition() + 7);
                    CharID = ParseInt();
                    m_Glyphs[CharID].c = CharID;
                    m_Glyphs[CharID].IsValid = true;
                }
                else if(string((CString)GetPtr(),7) == "uvStart")
                {
                    SetPosition(GetPosition() + 9);
                    m_Glyphs[CharID].uv0 = ParseVector2();
                }
                else if(string((CString)GetPtr(),5) == "uvEnd")
                {
                    SetPosition(GetPosition() + 7);
                    m_Glyphs[CharID].uv1 = ParseVector2();
                }
                else if(string((CString)GetPtr(),4) == "size")
                {
                    SetPosition(GetPosition() + 6);
                    m_Glyphs[CharID].Size = ParseVector2();
                }
                else if(string((CString)GetPtr(),7) == "xOffset")
                {
                    SetPosition(GetPosition() + 8);
                    m_Glyphs[CharID].Offset.x = ParseInt();
                }
                else if(string((CString)GetPtr(),7) == "yOffset")
                {
                    SetPosition(GetPosition() + 8);
                    m_Glyphs[CharID].Offset.y = ParseInt();
                }
                else if(string((CString)GetPtr(),8) == "xAdvance")
                {
                    SetPosition(GetPosition() + 9);
                    m_Glyphs[CharID].xAdvance = ParseInt();
                }
            }
        }
        return true;
    }
    void Font::AddTextToEntity(Entity* E,const string& Text,const Vector3& Offset,i32 TextureID)
    {
        RenderComponent* r = GetRenderComponent(E);
        if(!r)
        {
            Log("Cannot add text to entity <%d>. Entity has no render component.\n",E->GetID());
            return;
        }
        
        i32 xPos = 0;
        for(i32 i = 0;i < Text.size();i++)
        {
            if(m_Glyphs[Text[i]].IsValid)
            {
                if(r->GetPrimType() == GL_TRIANGLES)
                {
                    r->AddVertex(Vector3(Offset.x + xPos + m_Glyphs[Text[i]].Offset.x,Offset.y + m_Glyphs[Text[i]].Offset.y,Offset.z));
                    r->AddTexCoord(Vector2(2 * m_Glyphs[Text[i]].uv0.x,1) - m_Glyphs[Text[i]].uv0);
                    
                    r->AddVertex(Vector3(Offset.x + xPos + m_Glyphs[Text[i]].Offset.x + m_Glyphs[Text[i]].Size.x,Offset.y + m_Glyphs[Text[i]].Offset.y,Offset.z));
                    r->AddTexCoord(Vector2(2 * m_Glyphs[Text[i]].uv1.x,1) - Vector2(m_Glyphs[Text[i]].uv1.x,m_Glyphs[Text[i]].uv0.y));
                    
                    r->AddVertex(Vector3(Offset.x + xPos + m_Glyphs[Text[i]].Offset.x,Offset.y + m_Glyphs[Text[i]].Offset.y + m_Glyphs[Text[i]].Size.y,Offset.z));
                    r->AddTexCoord(Vector2(2 * m_Glyphs[Text[i]].uv0.x,1) - Vector2(m_Glyphs[Text[i]].uv0.x,m_Glyphs[Text[i]].uv1.y));
                    
                    
                    r->AddVertex(Vector3(Offset.x + xPos + m_Glyphs[Text[i]].Offset.x + m_Glyphs[Text[i]].Size.x,Offset.y + m_Glyphs[Text[i]].Offset.y,Offset.z));
                    r->AddTexCoord(Vector2(2 * m_Glyphs[Text[i]].uv1.x,1) - Vector2(m_Glyphs[Text[i]].uv1.x,m_Glyphs[Text[i]].uv0.y));
                    
                    r->AddVertex(Vector3(Offset.x + xPos + m_Glyphs[Text[i]].Offset.x + m_Glyphs[Text[i]].Size.x,Offset.y + m_Glyphs[Text[i]].Offset.y + m_Glyphs[Text[i]].Size.y,Offset.z));
                    r->AddTexCoord(Vector2(2 * m_Glyphs[Text[i]].uv1.x,1) - m_Glyphs[Text[i]].uv1);
                    
                    r->AddVertex(Vector3(Offset.x + xPos + m_Glyphs[Text[i]].Offset.x,Offset.y + m_Glyphs[Text[i]].Offset.y + m_Glyphs[Text[i]].Size.y,Offset.z));
                    r->AddTexCoord(Vector2(2 * m_Glyphs[Text[i]].uv0.x,1) - Vector2(m_Glyphs[Text[i]].uv0.x,m_Glyphs[Text[i]].uv1.y));
                    xPos += m_Glyphs[Text[i]].xAdvance;
                }
                //xPos += default_char_width;
            }
            else
            {
                Log("Warning: Text <%s> contains invalid character <%d:%c>.\n",Text.c_str(),Text[i],Text[i]);
            }
        }
        
        if(r->GetMesh()->m_Textures[TextureID]) r->GetMesh()->m_Textures[TextureID]->Destroy();
        r->GetMesh()->m_Textures[TextureID] = m_Texture;
        m_Texture->AddRef();
    }
};