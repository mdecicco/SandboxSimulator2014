/*
 *  Texture.h
 *
 *  Created by Michael DeCicco on 8/4/14.
 *
 */

#pragma once
#include "EngineMath.h"
#include "AssetManager.h"
#include "glfw3.h"

namespace Reality
{
    class Texture : public Asset
    {
        public:
            virtual bool Reload();
        
            void Bind(GLenum Target = GL_TEXTURE_2D) { m_Target = Target; glBindTexture(m_Target,m_TexID); }
            void Unbind() { glBindTexture(m_Target,m_TexID); }
        
            void BeginRTT();
            void EndRTT();
        
            GLuint GetTexID();
        protected:
            friend class AssetManager;
            Texture();
            ~Texture();
        
            uByte* m_Data;
            Vector2 m_Dimensions;
            GLuint m_TexID;
            GLenum m_Target;
    };
};