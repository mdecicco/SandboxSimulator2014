#include <System/File.h>
#include <ctype.h>

namespace SandboxSimulator
{
    //Helper functions
    File* OpenFile(Literal Filename,Literal Mode)
    {
        FILE* fp = fopen(Filename,Mode);
        if(fp == NULL) { printf("Unable to open file <%s>.\n",Filename); return 0; }
        File* rFile = new File(fp,Filename);
        return rFile;
    }

    void CloseFile(File* fp)
    {
        if(!fp->m_fp) return;
        fclose(fp->m_fp);
        fp->m_fp = 0;
        fp->Destroy();
    }

    //Asset class
    bool Asset::Load(Literal Path)
    {
        File* fp = OpenFile(Path,"rb");
        if(!fp)
        {
            printf("Unable to load asset <%s>\n",Path);
            return false;
        }
        
        if(m_Data) { delete [] (Byte*)m_Data; }
        m_Size    = (i32)fp->GetSize();
        m_Data    = fp->ReadBlock(m_Size);
        m_Pos     = 0;
        m_MemSize = m_Size;
        
        CloseFile(fp);
        m_LoadPath = Path;
        
        return OnLoad();
    }

    i32 Asset::Destroy()
    {
        m_aRefCount--;
        if(m_aRefCount == 0)
        {
            //Engine::GetEngine()->GetAssetManager()->UnloadAsset(this);
            delete this;
        }
        else return m_aRefCount;
        return 0;
    }

    void* Asset::Read(i32 Sz)
    {
        if(m_Pos + sizeof(Sz) > m_Size)
        {
            printf("Unable to read %lu bytes from asset <%s>.\n",Sz,m_Path.c_str());
            return 0;
        }
        
        void* Ptr = new char[Sz];
        memcpy(Ptr,GetPtr(),Sz);
        m_Pos += Sz;
        return Ptr;
    }

    Scalar Asset::ParseScalar()
    {
        string s;
        char c = GetC();
        while(isdigit(c) || c == '.' || c == '-') { s += c; c = GetC(); }
        SetPosition(GetPosition() - 1);
        return stof(s);
    }

    i32 Asset::ParseInt()
    {
        string s;
        char c = GetC();
        while(isdigit(c) || c == '-') { s += c; c = GetC(); }
        SetPosition(GetPosition() - 1);
        return stoi(s);
    }

    Vec2 Asset::ParseVector2()
    {
        char c = GetC();
        while((c == ',' || isspace(c)) && GetPosition() != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v0 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && GetPosition() != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v1 = ParseScalar();
        
        return Vec2(v0,v1);
    }

    Vec3 Asset::ParseVector3()
    {
        char c = GetC();
        while((c == ',' || isspace(c)) && GetPosition() != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v0 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && GetPosition() != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v1 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && GetPosition() != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v2 = ParseScalar();
        
        return Vec3(v0,v1,v2);
    }

    Vec4 Asset::ParseVector4()
    {
        char c = GetC();
        while((c == ',' || isspace(c)) && GetPosition() != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v0 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && GetPosition() != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v1 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && GetPosition() != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v2 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && GetPosition() != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v3 = ParseScalar();
        
        return Vec4(v0,v1,v2,v3);
    }

    //File class
    File::File(FILE* fp,Literal Filename) : m_fp(fp), m_Name(Filename)
    {
        fseek(m_fp,0,SEEK_END);
        m_Size = ftell(m_fp);
        rewind(m_fp);
        m_RefCount = 1;
    }
    File::~File()
    {
    }
    void File::Destroy()
    {
        m_RefCount--;
        if(m_RefCount == 0)
        {
            CloseFile(this);
            delete this;
        }
    }
    void* File::ReadBlock(size_t Sz)
    {
        Byte* Data = new Byte[Sz];
        if(fread(Data,Sz,1,m_fp) != 1) { printf("Unable to read %lu bytes from file <%s>.\n",Sz,m_Name); delete [] Data; return 0; }
        return Data;
    }
    bool File::WriteBlock(void* Data,size_t Sz)
    {
        if(fwrite(Data,Sz,1,m_fp) != 1) { printf("Unable to write %lu bytes to file <%s>.\n",Sz,m_Name); return false; }
        return true;
    }
    
    char File::GetC()
    {
        char c;
        Read(c);
        return c;
    }

    Scalar File::ParseScalar()
    {
        std::string s;
        char c = GetC();
        while(isdigit(c) || c == '.' || c == '-') { s += c; c = GetC(); }
        SetPosition(GetPosition() - 1);
        return stof(s);
    }

    i32 File::ParseInt()
    {
        std::string s;
        char c = GetC();
        while(isdigit(c) || c == '-') { s += c; c = GetC(); }
        SetPosition(GetPosition() - 1);
        return stoi(s);
    }

    Vec2 File::ParseVector2()
    {
        char c = GetC();
        while((c == ',' || isspace(c)) && ftell(m_fp) != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v0 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && ftell(m_fp) != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v1 = ParseScalar();
        
        return Vec2(v0,v1);
    }

    Vec3 File::ParseVector3()
    {
        char c = GetC();
        while((c == ',' || isspace(c)) && ftell(m_fp) != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v0 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && ftell(m_fp) != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v1 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && ftell(m_fp) != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v2 = ParseScalar();
        
        return Vec3(v0,v1,v2);
    }

    Vec4 File::ParseVector4()
    {
        char c = GetC();
        while((c == ',' || isspace(c)) && ftell(m_fp) != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v0 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && ftell(m_fp) != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v1 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && ftell(m_fp) != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v2 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && ftell(m_fp) != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v3 = ParseScalar();
        
        return Vec4(v0,v1,v2,v3);
    }

    CString File::ReadAll() const
    {
        CString Scpt = MakeCString(m_Size + 1);
        i32 p = 0;
        while(ftell(m_fp) != m_Size) Read(Scpt[p++]);
        Scpt[p++] = 0;
        return Scpt;
    }

    std::string File::ReadLn() const
    {
        std::string s;
        while(ftell(m_fp) != m_Size)
        {
            char c;
            Read(c);
            if(c == '\n' || c == '\r') break;
            s += c;
        }
        return s;
    }

    bool File::WriteStr(const std::string& str)
    {
        return WriteBlock(const_cast<char*>(&str[0]),str.length() - 1);
    }

    size_t File::GetPosition() const
    {
        return ftell(m_fp);
    }

    void File::SetPosition(size_t Pos)
    {
        fseek(m_fp,Pos,SEEK_SET);
    }

    size_t File::GetSize() const
    {
        return m_Size;
    }

    bool File::AtEnd() const
    {
        return ftell(m_fp) == m_Size;
    }
}