/*
 *  FileManager.h
 *
 *  Created by Michael DeCicco on 6/26/14.
 *
 */

#pragma once
#include "Hashmap.hpp"
#include "Log.h"
#include "EngineMath.h"
#include <stdio.h>

namespace Reality
{
    class File
    {
        public:
            void AddRef() { m_RefCount++; }
            void Destroy();
        
            template <typename T>
            void Read(T& OutValue) const
            {
                if(fread(&OutValue,sizeof(T),1,m_fp) != 1) Log("Unable to read %lu bytes from file <%s>.\n",sizeof(T),m_Name);
            }
            void* ReadBlock(size_t Sz);
        
            template <typename T>
            bool Write(const T& InValue)
            {
                if(fwrite(&InValue,sizeof(T),1,m_fp) != 1) { Log("Unable to write %lu bytes to file <%s>.\n",sizeof(T),m_Name); return false; }
                return true;
            }
            bool WriteBlock(void* Data,size_t Sz);
        
            char GetC();
            Scalar ParseScalar();
            i32 ParseInt();
            i64 ParseInt64();
            bool ParseBool();
            Vector2 ParseVector2();
            Vector3 ParseVector3();
            Vector4 ParseVector4();
            CString ReadAll() const;
            string ReadLn() const;
        
            size_t GetPosition() const;
            void SetPosition(size_t Pos);
            size_t GetSize() const;
        
            bool AtEnd() const;
        
        protected:
            friend class FileManager;
            friend class Engine;
            File(FILE* fp,Literal Filename);
            ~File();
        
            FILE* m_fp;
            Literal m_Name;
            size_t m_Size;
        
            i32 m_RefCount;
    };
    
    class FileManager
    {
        public:
            FileManager();
            ~FileManager();
        
            File* Open(Literal Filename,Literal Mode);
            void Close(File* fp);
        
            CString GetContents(Literal Filename);
            bool FileExists(Literal Filename);
            bool DirectoryExists(Literal DirectoryName);
            bool MakeDirectory(Literal DirectoryName);
        
        protected:
            Hashmap<File*> m_Files;
    };
};