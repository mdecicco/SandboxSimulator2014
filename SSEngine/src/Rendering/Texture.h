#ifndef SandboxSimulator_Texture_h
#define SandboxSimulator_Texture_h

#include <System/File.h>
#include <System/PlatformIncludes.h>
#include <GLFW/glfw3.h>

#include <string>

namespace SandboxSimulator
{
class Texture
{
    public:
        Texture();
        Texture(i32 Width,i32 Height,bool Depth);
        //Texture(const Vec2& D,bool Depth);
        ~Texture();
        
        virtual bool Load(std::string Path);
    
        void Bind(GLenum Target = GL_TEXTURE_2D) { m_Target = Target; glBindTexture(m_Target,m_TexID); }
        void Unbind() { glBindTexture(m_Target,m_TexID); }
        Vec2 GetDimensions() const { return m_Dimensions; }
    
        void BeginRTT();
        void EndRTT();
    
        GLuint GetTexID();
    protected:
        friend class AssetManager;

        std::string m_Path;
        uByte* m_Data;
        Vec2 m_Dimensions;
        GLuint m_TexID;
        GLenum m_Target;
        GLuint m_DepthBuffer;
        GLuint m_FBO;
};
}

#endif
