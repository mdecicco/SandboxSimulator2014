/*
 *  Texture.cpp
 *
 *  Created by Michael DeCicco on 8/4/14.
 *
 */

#include "Texture.h"
#include "FreeImage.h"

namespace Reality
{
    i32 GetNextPower2(i32 num)
    {
        i32 b = num;
        i32 n;
        for(n = 0;b != 0;n++) b >>= 1;
        b = 1 << n;
        if (b == 2 * num) b >>= 1;
        return b;
    }
    
    Texture::Texture() : m_TexID(-1), m_Data(0)
    {
    }
    Texture::~Texture()
    {
        if(m_TexID != -1) glDeleteTextures(1,&m_TexID);
    }

    bool Texture::Reload()
    {
        FREE_IMAGE_FORMAT fmt = FreeImage_GetFileType(GetPath().c_str(),0);
        FIBITMAP* Img = FreeImage_Load(fmt,GetPath().c_str());
        if(!Img) return false;
     
        FIBITMAP* temp = Img;
        Img = FreeImage_ConvertTo32Bits(Img);
        FreeImage_Unload(temp);
     
        i32 w = FreeImage_GetWidth(Img);
        i32 h = FreeImage_GetHeight(Img);
        
        m_Dimensions = Vector2(w,h);
     
        m_Data = new uByte[4 * w * h];
        Byte* ImgBits = (Byte*)FreeImage_GetBits(Img);
        for(i32 j = 0;j < w * h;j++)
        {
            m_Data[j * 4 + 0] = ImgBits[j * 4 + 2];
            m_Data[j * 4 + 1] = ImgBits[j * 4 + 1];
            m_Data[j * 4 + 2] = ImgBits[j * 4 + 0];
            m_Data[j * 4 + 3] = ImgBits[j * 4 + 3];
        }
        
        FreeImage_Unload(Img);
        return true;
    }
    void Texture::BeginRTT()
    {
        
    }
    void Texture::EndRTT()
    {
    }
    GLuint Texture::GetTexID()
    {
        if(m_TexID == -1 && m_Data)
        {
            glGenTextures(1,&m_TexID);
            glBindTexture(GL_TEXTURE_2D,m_TexID);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,m_Dimensions.x,m_Dimensions.y,0,GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)m_Data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            delete [] m_Data;
            m_Data = 0;
        }
        return m_TexID;
    }
};