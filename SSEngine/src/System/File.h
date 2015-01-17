#ifndef SandboxSimulator_File_h
#define SandboxSimulator_File_h

#include <string>
#include <System/SSTypes.h>
#include <Utils/Math.h>
#include <stdio.h>

namespace SandboxSimulator {
class File
{
    public:
        File(FILE* fp,Literal Filename);
        void AddRef() { m_RefCount++; }
        void Destroy();
    
        template <typename T>
        bool Read(T& OutValue) const
        {
            if(fread(&OutValue,sizeof(T),1,m_fp) != 1) { printf("Unable to read %lu bytes from file <%s>.\n",sizeof(T),m_Name); return false; }
            return true;
        }
        void* ReadBlock(size_t Sz);
    
        template <typename T>
        bool Write(const T& InValue)
        {
            if(fwrite(&InValue,sizeof(T),1,m_fp) != 1) { printf("Unable to write %lu bytes to file <%s>.\n",sizeof(T),m_Name); return false; }
            return true;
        }
        bool WriteBlock(void* Data,size_t Sz);
    
        char GetC();
        Scalar ParseScalar();
        i32 ParseInt();
        i64 ParseInt64();
        bool ParseBool();
        Vec2 ParseVector2();
        Vec3 ParseVector3();
        Vec4 ParseVector4();
        CString ReadAll() const;
        std::string ReadLn() const;
        bool WriteStr(const std::string& str);
    
        size_t GetPosition() const;
        void SetPosition(size_t Pos);
        size_t GetSize() const;
    
        bool AtEnd() const;
    
        FILE* m_fp;
    protected:
        friend class FileManager;
        friend class Engine;
        ~File();

        Literal m_Name;
        size_t m_Size;
    
        i32 m_RefCount;
};

File* OpenFile(Literal Filename,Literal Mode);
void CloseFile(File* fp);

class Asset
{
    public:
        Asset() : m_ID(-1), m_aRefCount(1), m_Data(0), m_Updated(false) { }

        bool Load(Literal Path);
        virtual bool OnLoad() = 0;

        void* GetPtr() const { return (void*)(((intptr_t)m_Data) + m_Pos); }
        template <typename T>
        void Read(T& OutValue)
        {
            if(m_Pos + sizeof(T) > m_Size)
            {
                printf("Unable to read %lu bytes from asset.\n",sizeof(T));
            }
            memcpy(&OutValue,*(T*)((intptr_t)m_Data + m_Pos),sizeof(OutValue));
            m_Pos += sizeof(T);
        }
        void* Read(i32 Sz);

        char GetC() { return ((CString)m_Data)[m_Pos++]; }

        i32 GetPosition() const { return m_Pos; }
        void SetPosition(i32 Pos) { m_Pos = Pos; }
        i32 GetSize() const { return m_Size; }

        bool AtEnd() const { return m_Pos == m_Size; }

    protected: 
        virtual ~Asset() { if(m_Data) delete (Byte*)m_Data; }
      
        i32 m_ID;
        i32 m_aRefCount;
        
        i32 m_Pos;
        i32 m_Size;
        i32 m_MemSize;
        void* m_Data;
        
        bool m_Updated;
        std::string m_Path;
};
}

#endif