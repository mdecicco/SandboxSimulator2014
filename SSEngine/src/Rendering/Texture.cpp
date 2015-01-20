#include <Rendering/Texture.h>
#include <src/SOIL.h>
#include <Utils/PngLoader.h>

namespace SandboxSimulator
{
    Texture::Texture(i32 Width,i32 Height,bool Depth) : m_TexID(-1), m_Data(0), m_FBO(-1), m_DepthBuffer(-1), m_Target(0)
    {
        if(m_FBO != -1) glDeleteFramebuffers(1,&m_FBO);
        glGenFramebuffers(1,&m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER,m_FBO);
        m_Dimensions = Vec2(Width,Height);
        
        if(m_TexID != -1) glDeleteTextures(1,&m_TexID);
        glGenTextures(1,&m_TexID);
        glBindTexture(GL_TEXTURE_2D,m_TexID);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,Width,Height,0,GL_RGBA,GL_UNSIGNED_BYTE,0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        if(Depth)
        {
            if(m_DepthBuffer != -1) glDeleteRenderbuffers(1,&m_DepthBuffer);
            glGenRenderbuffers(1,&m_DepthBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER,m_DepthBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,Width,Height);
            
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,m_DepthBuffer);
        }
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,m_TexID,0);
        GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1,DrawBuffers);
        
        GLenum r = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(r != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Unable to create framebuffer for RTT (0x%X). Expect unexpected results.\n",r);
            if(m_DepthBuffer != -1) glDeleteRenderbuffers(1,&m_DepthBuffer);
            if(m_FBO != -1) glDeleteFramebuffers(1,&m_FBO);
            if(m_TexID != -1) glDeleteTextures(1,&m_TexID);
            m_DepthBuffer = m_FBO = m_TexID = -1;
        }
    }

    Texture::Texture() : m_TexID(-1), m_Data(0), m_FBO(-1), m_Target(0)
    {}

    Texture::~Texture()
    {
        if(m_TexID != -1) glDeleteTextures(1,&m_TexID);
        if(m_FBO != -1) glDeleteFramebuffers(1,&m_FBO);
        if(m_DepthBuffer != -1) glDeleteRenderbuffers(1,&m_DepthBuffer);
    }

    bool Texture::Load(Literal Path)
    {
        m_Path = Path;
        #ifdef WIN32
		std::vector<uByte> Image;
        unsigned Width, Height;
		unsigned error = lodepng::decode(Image, Width, Height, m_Path);
        
		if(error != 0)
		{
		  printf("Error %u : %s\n", error, lodepng_error_text(error));
		  return false;
		}

		size_t u2 = 1; while(u2 < Width) u2 *= 2;
		size_t v2 = 1; while(v2 < Height) v2 *= 2;
		double u3 = (double)Width / u2;
		double v3 = (double)Height / v2;

		std::vector<unsigned char> image2(u2 * v2 * 4);
		for(size_t y = 0; y < Height; y++)
		for(size_t x = 0; x < Width; x++)
		for(size_t c = 0; c < 4; c++)
		{
		    image2[4 * u2 * y + 4 * x + c] = Image[4 * Width * y + 4 * x + c];
		}

		GLfloat fLargest;
		glGetFloatv(GL_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glGenTextures(1, &m_TexID);
		glBindTexture(GL_TEXTURE_2D, m_TexID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);

		Width= u2;
		Height = v2;
        m_Dimensions.x = Width;
        m_Dimensions.y = Height;

        #else
        m_TexID = SOIL_load_OGL_texture
		(
			m_Path.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

		if( m_TexID == 0 )
		{
			printf("image loading error: '%s'\n", SOIL_last_result());
		}

        glBindTexture(GL_TEXTURE_2D, m_TexID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        #endif
        return true;
    }

    void Texture::BeginRTT()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,m_FBO);
        glViewport(0,0,m_Dimensions.x,m_Dimensions.y);
        glClear(m_DepthBuffer != -1 ? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT);
        GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1,DrawBuffers);
    }

    void Texture::EndRTT()
    {
        Vec2 r = Vec2(800,600);//Engine::GetEngine()->GetResolution();
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,r.x,r.y);
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
}
