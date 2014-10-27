/*
 *  Text.h
 *
 *  Created by Michael DeCicco on 9/12/14.
 *
 */

#pragma once
#include "Texture.h"
#include "AssetManager.h"

#include <string>
using namespace std;

namespace Reality
{
    class Entity;
    typedef struct _Glyph
    {
        char c;
        Vector2 uv0;
        Vector2 uv1;
        Vector2 Size;
        Vector2 Offset;
        int xAdvance;
        bool IsValid;
    } Glyph;
    
    class Font : public Asset
    {
        public:
            Font();
            ~Font();
        
            virtual bool Reload();
            void AddTextToEntity(Entity* E,const string& Text,const Vector3& Offset,i32 TextureID);
        
            Texture* m_Texture;
            Glyph m_Glyphs[256];
    };
};