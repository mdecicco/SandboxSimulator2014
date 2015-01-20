#include <Rendering/Gui/Font.h>
#include <Rendering/RenderSystem.h>

namespace SandboxSimulator
{
    Font::Font()
    {
    }
    Font::~Font()
    {
        //m_Texture->Destroy();
        delete m_Texture;
    }

    bool Font::OnLoad()
    {
        for(i32 i = 0;i < 256;i++) m_Glyphs[i].IsValid = false;
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
                    
                    m_Texture = new Texture();//(Texture*)Engine::GetEngine()->GetAssetManager()->LoadAsset(str.c_str(),AT_TEXTURE);
                    m_Texture->Load(str.c_str());
                    SetPosition(nPos);
                }
                if(string((CString)GetPtr(),10) == "lineHeight")
                {
                    i32 iPos = GetPosition() + 11;
                    SetPosition(iPos);
                    m_LineHeight = ParseInt();
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
                    m_Glyphs[CharID].uv0.y = 1.0f - m_Glyphs[CharID].uv0.y;
                }
                else if(string((CString)GetPtr(),5) == "uvEnd")
                {
                    SetPosition(GetPosition() + 7);
                    m_Glyphs[CharID].uv1 = ParseVector2();
                    m_Glyphs[CharID].uv1.y = 1.0f - m_Glyphs[CharID].uv1.y;
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
    void Font::AddTextToEntity(Entity* E,const string& Text,const Vec3& Offset,i32 TextureID)
    {
        //if(!m_Texture) return;
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        if(!E->HasComponentType(CT_RENDER))
        {
            printf("Cannot add text to entity <%d>. Entity has no render component.\n",E->GetID());
            return;
        }
        
        f32 xPos = 0;
        f32 yPos = 0;
        for(i32 i = 0;i < Text.size();i++)
        {
            char c = Text[i];
            if(c == '\n' || c == '\r')
            {
                xPos = 0;
                yPos += m_LineHeight;
                continue;
            }
            
            if(m_Glyphs[c].IsValid)
            {
                if(r->GetPrimType() == GL_TRIANGLES)
                {
                    r->AddVertex(Vec3(Offset.x + xPos + m_Glyphs[c].Offset.x,Offset.y + yPos + m_Glyphs[c].Offset.y,Offset.z));
                    r->AddTexCoord(m_Glyphs[c].uv0);
                    
                    r->AddVertex(Vec3(Offset.x + xPos + m_Glyphs[c].Offset.x + m_Glyphs[c].Size.x,Offset.y + yPos + m_Glyphs[c].Offset.y,Offset.z));
                    r->AddTexCoord(Vec2(m_Glyphs[c].uv1.x,m_Glyphs[c].uv0.y));
                    
                    r->AddVertex(Vec3(Offset.x + xPos + m_Glyphs[c].Offset.x,Offset.y + yPos + m_Glyphs[c].Offset.y + m_Glyphs[c].Size.y,Offset.z));
                    r->AddTexCoord(Vec2(m_Glyphs[c].uv0.x,m_Glyphs[c].uv1.y));
                    
                    
                    r->AddVertex(Vec3(Offset.x + xPos + m_Glyphs[c].Offset.x + m_Glyphs[c].Size.x,Offset.y + yPos + m_Glyphs[c].Offset.y,Offset.z));
                    r->AddTexCoord(Vec2(m_Glyphs[c].uv1.x,m_Glyphs[c].uv0.y));
                    
                    r->AddVertex(Vec3(Offset.x + xPos + m_Glyphs[c].Offset.x + m_Glyphs[c].Size.x,Offset.y + yPos + m_Glyphs[c].Offset.y + m_Glyphs[c].Size.y,Offset.z));
                    r->AddTexCoord(m_Glyphs[c].uv1);
                    
                    r->AddVertex(Vec3(Offset.x + xPos + m_Glyphs[c].Offset.x,Offset.y + yPos + m_Glyphs[c].Offset.y + m_Glyphs[c].Size.y,Offset.z));
                    r->AddTexCoord(Vec2(m_Glyphs[c].uv0.x,m_Glyphs[c].uv1.y));
                    xPos += m_Glyphs[c].xAdvance;
                }
            }
            else
            {
                printf("Warning: Text contains invalid character <%d:%c>.\n",c,c);
            }
        }
        if(r->GetFont()) r->GetFont()->Destroy();
        r->SetFont(this);
        
        if(r->GetMesh()->m_Textures[TextureID]) r->GetMesh()->m_Textures[TextureID]->Destroy();
        r->GetMesh()->m_Textures[TextureID] = m_Texture;
    }
}