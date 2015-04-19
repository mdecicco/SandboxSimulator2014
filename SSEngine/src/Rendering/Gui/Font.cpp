#include <Rendering/Gui/Font.h>
#include <Rendering/RenderSystem.h>
#include <Rendering/Gui/Gui.h>

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
        //i32 Size = 0;
        //i32 DesiredSize = 128;
        //Scalar Scale = 1.0;

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
                m_TextSize = ParseInt();
                //Scale = ((f32)DesiredSize / (f32)Size);
            }
            else if(string((CString)GetPtr(),10) == "lineHeight")
            {
                i32 iPos = GetPosition() + 11;
                SetPosition(iPos);
                m_LineHeight = ParseInt();
            }
            else if(string((CString)GetPtr(),6) == "scaleW")
            {
                SetPosition(GetPosition() + 7);
                m_TextureSize = ParseInt();
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
                m_Glyphs[CharID].Size.y = Height;
                //if(Height > m_LineHeight) m_LineHeight = Height;
                m_Glyphs[CharID].uv1.y = m_Glyphs[CharID].uv0.y + Height;

                m_Glyphs[CharID].uv0.x = m_Glyphs[CharID].uv0.x / m_TextureSize;
                m_Glyphs[CharID].uv0.y = m_Glyphs[CharID].uv0.y / m_TextureSize;
                m_Glyphs[CharID].uv1.x = m_Glyphs[CharID].uv1.x / m_TextureSize;
                m_Glyphs[CharID].uv1.y = m_Glyphs[CharID].uv1.y / m_TextureSize;

                //m_Glyphs[CharID].Size.x *= Scale;
                m_Glyphs[CharID].Size.y = Height;
            }
            else if(string((CString)GetPtr(),7) == "xoffset")
            {
                SetPosition(GetPosition() + 8);
                m_Glyphs[CharID].Offset.x = ParseScalar();
            }
            else if(string((CString)GetPtr(),7) == "yoffset")
            {
                SetPosition(GetPosition() + 8);
                m_Glyphs[CharID].Offset.y = (ParseScalar());
            }
            else if(string((CString)GetPtr(),8) == "xadvance")
            {
                SetPosition(GetPosition() + 9);
                m_Glyphs[CharID].xAdvance = ParseScalar();
            }
        }
        return true;
    }
    void Font::AddToMesh(Mesh* m,const string& Text,const Vec3& Offset, i32 TextSize)
    {   
        f32 xPos = 0;
        f32 yPos = 0;
        f32 Scale = (f32)TextSize / (f32)m_TextSize;
        m->Clear();
        for(i32 i = 0;i < Text.size();i++)
        {
            char c = Text[i];
            if(c == '\n' || c == '\r')
            {
                xPos = 0;
                yPos += m_LineHeight * Scale;
                continue;
            }
            
            if(m_Glyphs[c].IsValid)
            {
                m->AddVertex(Vec3(Offset.x + xPos + (m_Glyphs[c].Offset.x * Scale),Offset.y + yPos + (m_Glyphs[c].Offset.y * Scale),Offset.z));
                m->AddTexCoord(m_Glyphs[c].uv0);

                m->AddVertex(Vec3(Offset.x + xPos + (m_Glyphs[c].Offset.x * Scale),Offset.y + yPos + (m_Glyphs[c].Offset.y * Scale) + (m_Glyphs[c].Size.y * Scale),Offset.z));
                m->AddTexCoord(Vec2(m_Glyphs[c].uv0.x,m_Glyphs[c].uv1.y));

                m->AddVertex(Vec3(Offset.x + xPos + (m_Glyphs[c].Offset.x * Scale) + (m_Glyphs[c].Size.x * Scale),Offset.y + yPos + (m_Glyphs[c].Offset.y * Scale),Offset.z));
                m->AddTexCoord(Vec2(m_Glyphs[c].uv1.x,m_Glyphs[c].uv0.y));


                m->AddVertex(Vec3(Offset.x + xPos + (m_Glyphs[c].Offset.x * Scale) + (m_Glyphs[c].Size.x * Scale),Offset.y + yPos + (m_Glyphs[c].Offset.y * Scale),Offset.z));
                m->AddTexCoord(Vec2(m_Glyphs[c].uv1.x,m_Glyphs[c].uv0.y));

                m->AddVertex(Vec3(Offset.x + xPos + (m_Glyphs[c].Offset.x * Scale),Offset.y + yPos + (m_Glyphs[c].Offset.y * Scale) + (m_Glyphs[c].Size.y * Scale),Offset.z));
                m->AddTexCoord(Vec2(m_Glyphs[c].uv0.x,m_Glyphs[c].uv1.y));

                m->AddVertex(Vec3(Offset.x + xPos + (m_Glyphs[c].Offset.x * Scale) + (m_Glyphs[c].Size.x * Scale),Offset.y + yPos + (m_Glyphs[c].Offset.y * Scale) + (m_Glyphs[c].Size.y * Scale),Offset.z));
                m->AddTexCoord(m_Glyphs[c].uv1);
                if(c != '\n' && c != '\r')
                    xPos += m_Glyphs[c].xAdvance * Scale;
            }
            else
            {
                printf("Warning: Text contains invalid character <%d:%c>.\n",c,c);
            }
        }
        
        if(m->m_Textures[1]) m->m_Textures[1]->Destroy();
        m->m_Textures[1] = m_Texture;
    }
}