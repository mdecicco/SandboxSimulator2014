/*
 *  FileManager.cpp
 *
 *  Created by Michael DeCicco on 6/26/14.
 *
 */

#include "FileManager.h"
#include "Engine.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace Reality
{
    File::File(FILE* fp,Literal Filename) : m_fp(fp), m_Name(Filename)
    {
        fseek(m_fp,0L,SEEK_END);
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
            Engine::GetEngine()->CloseFile(this);
            delete this;
        }
    }
    void* File::ReadBlock(size_t Sz)
    {
        Byte* Data = new Byte[Sz];
        if(fread(Data,Sz,1,m_fp) != 1) { Log("Unable to read %lu bytes from file <%s>.\n",Sz,m_Name); delete [] Data; return 0; }
        return Data;
    }
    bool File::WriteBlock(void* Data,size_t Sz)
    {
        if(fwrite(Data,Sz,1,m_fp) != 1) { Log("Unable to write %lu bytes to file <%s>.\n",Sz,m_Name); return false; }
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
        string s;
        char c = GetC();
        while(isnumber(c) || c == '.' || c == '-') { s += c; c = GetC(); }
        SetPosition(GetPosition() - 1);
        return stof(s);
    }
    i32 File::ParseInt()
    {
        string s;
        char c = GetC();
        while(isnumber(c) || c == '-') { s += c; c = GetC(); }
        SetPosition(GetPosition() - 1);
        return stoi(s);
    }
    bool File::ParseBool()
    {
        string s;
        char c = GetC();
        while(isalpha(c)) { s += c; c = GetC(); }
        SetPosition(GetPosition() - 1);
        bool v;
        ParseVal(s,0,0,&v);
        return v;
    }
    Vector2 File::ParseVector2()
    {
        char c = GetC();
        while((c == ',' || isspace(c)) && ftell(m_fp) != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v0 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && ftell(m_fp) != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v1 = ParseScalar();
        
        return Vector2(v0,v1);
    }
    Vector3 File::ParseVector3()
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
        
        return Vector3(v0,v1,v2);
    }
    Vector4 File::ParseVector4()
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
        
        return Vector4(v0,v1,v2,v3);
    }
    CString File::ReadAll() const
    {
        CString Scpt = MakeCString(m_Size + 1);
        i32 p = 0;
        while(ftell(m_fp) != m_Size) Read(Scpt[p++]);
        Scpt[p++] = 0;
        return Scpt;
    }
    string File::ReadLn() const
    {
        string s;
        while(ftell(m_fp) != m_Size)
        {
            char c;
            Read(c);
            if(c == '\n' || c == '\r') break;
            s += c;
        }
        return s;
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
    
    FileManager::FileManager()
    {
    }
    FileManager::~FileManager()
    {
    }

    File* FileManager::Open(Literal Filename,Literal Mode)
    {
        FILE* fp = fopen(Filename,Mode);
        if(!fp) { Log("Unable to open file <%s>.\n",Filename); return 0; }
        File* rFile = new File(fp,Filename);
        m_Files.Insert(rFile,Filename);
        return rFile;
    }
    void FileManager::Close(File* fp)
    {
        if(!fp->m_fp) return;
        m_Files.Remove(fp->m_Name);
        fclose(fp->m_fp);
        fp->m_fp = 0;
        fp->Destroy();
    }
    CString FileManager::GetContents(Literal Filename)
    {
        File* fp = Open(Filename,"r");
        if(!fp) return 0;
        CString Contents = fp->ReadAll();
        Close(fp);
        return Contents;
    }
    bool FileManager::FileExists(Literal Filename)
    {
        struct stat st;
        int result = stat(Filename, &st);
        return result == 0;
    }
    bool FileManager::DirectoryExists(Literal DirectoryName)
    {
        struct stat s;
        i32 err = stat(DirectoryName,&s);
        if(err == -1) return false;
        else return S_ISDIR(s.st_mode);
        return false;
    }
    bool FileManager::MakeDirectory(Literal DirectoryName)
    {
        return mkdir(DirectoryName,0777) == 0;
    }
};