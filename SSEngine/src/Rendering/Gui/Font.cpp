#include <Rendering/Gui/Font.h>
#include <Rendering/RenderSystem.h>

namespace SandboxSimulator
{
    Font::Font() : m_LineHeight(5)
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
        i32 Size = 0;
        i32 DesiredSize = 48;
        Scalar Scale = 1.0;

        while(!AtEnd())
        {
            char c = GetC();
            if(string((CString)GetPtr(),4) == "file")
            {
                i32 iPos = GetPosition() + 6;
                i32 nPos = iPos;
                SetPosition(iPos);
                
                for(;!AtEnd() && GetC() != '\"';nPos++) { }
                SetPosition(iPos);
                string str((CString)GetPtr(),nPos - iPos);
                
                m_Texture = new Texture();//(Texture*)Engine::GetEngine()->GetAssetManager()->LoadAsset(str.c_str(),AT_TEXTURE);
                m_Texture->Load(str.c_str());
                SetPosition(nPos);
            }
            else if(string((CString)GetPtr(),4) == "size")
            {
                i32 iPos = GetPosition() + 5;
                SetPosition(iPos);
                i32 Size = ParseInt();
                Scale = ((f32)DesiredSize / (f32)Size);
            }
            else if(string((CString)GetPtr(),10) == "lineHeight")
            {
                i32 iPos = GetPosition() + 11;
                SetPosition(iPos);
                m_LineHeight = ParseInt();
            }
            else if(string((CString)GetPtr(),5) == "scaleW")
            {
                SetPosition(GetPosition() + 6);
                Size = ParseInt();
            }
            else if(string((CString)GetPtr(),2) == "id")
            {
                SetPosition(GetPosition() + 3);
                CharID = ParseInt();
                m_Glyphs[CharID].c = CharID;
                m_Glyphs[CharID].IsValid = true;
                m_Glyphs[CharID].uv0 = Vec2(0,0);
                m_Glyphs[CharID].uv1 = Vec2(0,0);
                m_Glyphs[CharID].Size = Vec2(0,0);
            }
            else if(string((CString)GetPtr(),2) == "x=")
            {
                //std::string val = string((CString)GetPtr(),4);
                SetPosition(GetPosition() + 2);
                //std::string val2 = string((CString)GetPtr(),4);
                m_Glyphs[CharID].uv0.x = ParseInt();
            }
            else if(string((CString)GetPtr(),2) == "y=")
            {
                SetPosition(GetPosition() + 2);
                m_Glyphs[CharID].uv0.y = ParseInt();
            }
            else if(string((CString)GetPtr(),5) == "width")
            {
                SetPosition(GetPosition() + 6);
                m_Glyphs[CharID].Size.x = ParseInt();
                m_Glyphs[CharID].uv1.x = m_Glyphs[CharID].uv0.x + m_Glyphs[CharID].Size.x;
            }
            else if(string((CString)GetPtr(),6) == "height")
            {
                SetPosition(GetPosition() + 7);
                i32 Height = ParseInt();
                //if(Height > m_LineHeight) m_LineHeight = Height;
                m_Glyphs[CharID].uv1.y = m_Glyphs[CharID].uv0.y + Height;

                m_Glyphs[CharID].uv0.x = m_Glyphs[CharID].uv0.x / Size;
                m_Glyphs[CharID].uv0.y = m_Glyphs[CharID].uv0.y / Size;
                m_Glyphs[CharID].uv1.x = m_Glyphs[CharID].uv1.x / Size;
                m_Glyphs[CharID].uv1.y = m_Glyphs[CharID].uv1.y / Size;

                m_Glyphs[CharID].Size.x *= Scale;
                m_Glyphs[CharID].Size.y = Height * Scale;
                m_LineHeight = m_LineHeight * Scale;
            }
            else if(string((CString)GetPtr(),7) == "xoffset")
            {
                SetPosition(GetPosition() + 8);
                m_Glyphs[CharID].Offset.x = ParseScalar() * Scale;
            }
            else if(string((CString)GetPtr(),7) == "yoffset")
            {
                SetPosition(GetPosition() + 8);
                m_Glyphs[CharID].Offset.y = -(ParseScalar() * Scale);
            }
            else if(string((CString)GetPtr(),8) == "xadvance")
            {
                SetPosition(GetPosition() + 9);
                m_Glyphs[CharID].xAdvance = ParseScalar() * Scale;
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
                    
                    r->AddVertex(Vec3(Offset.x + xPos + m_Glyphs[c].Offset.x,Offset.y + yPos + m_Glyphs[c].Offset.y + m_Glyphs[c].Size.y,Offset.z));
                    r->AddTexCoord(Vec2(m_Glyphs[c].uv0.x,m_Glyphs[c].uv1.y));

                    r->AddVertex(Vec3(Offset.x + xPos + m_Glyphs[c].Offset.x + m_Glyphs[c].Size.x,Offset.y + yPos + m_Glyphs[c].Offset.y,Offset.z));
                    r->AddTexCoord(Vec2(m_Glyphs[c].uv1.x,m_Glyphs[c].uv0.y));
                    
                    
                    r->AddVertex(Vec3(Offset.x + xPos + m_Glyphs[c].Offset.x + m_Glyphs[c].Size.x,Offset.y + yPos + m_Glyphs[c].Offset.y,Offset.z));
                    r->AddTexCoord(Vec2(m_Glyphs[c].uv1.x,m_Glyphs[c].uv0.y));
                    
                    r->AddVertex(Vec3(Offset.x + xPos + m_Glyphs[c].Offset.x,Offset.y + yPos + m_Glyphs[c].Offset.y + m_Glyphs[c].Size.y,Offset.z));
                    r->AddTexCoord(Vec2(m_Glyphs[c].uv0.x,m_Glyphs[c].uv1.y));

                    r->AddVertex(Vec3(Offset.x + xPos + m_Glyphs[c].Offset.x + m_Glyphs[c].Size.x,Offset.y + yPos + m_Glyphs[c].Offset.y + m_Glyphs[c].Size.y,Offset.z));
                    r->AddTexCoord(m_Glyphs[c].uv1);
                    if(c != '\n' && c != '\r')
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