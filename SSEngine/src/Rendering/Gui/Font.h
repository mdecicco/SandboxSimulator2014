#ifndef SandboxSimulator_Font_h
#define SandboxSimulator_Font_h

#include <Utils/Math.h>
#include <System/File.h>
#include <Rendering/Texture.h>

namespace SandboxSimulator {
class Entity;
class Mesh;

typedef struct _Glyph
{
    char c;
    Vec2 uv0;
    Vec2 uv1;
    Vec2 Size;
    Vec2 Offset;
    int xAdvance;
    bool IsValid;
} Glyph;

class Font : public Asset
{
    public:
        Font();
        ~Font();
    
        virtual bool OnLoad();
        void AddToMesh(Mesh* m,const string& Text,const Vec3& Offset, i32 TextSize);
    
        i32 m_TextSize;
        i32 m_TextureSize;
        Texture* m_Texture;
        Glyph m_Glyphs[256];
        i32 m_LineHeight;
};
}

#endif