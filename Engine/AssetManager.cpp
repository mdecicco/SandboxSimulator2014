/*
 *  AssetManager.cpp
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#include "AssetManager.h"
#include "PropertyList.h"
#include "ScriptSystem.h"
#include "Shader.h"
#include "Text.h"
#include "Engine.h"

#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
using namespace std;

namespace Reality
{
    i32 Asset::Destroy()
    {
        m_aRefCount--;
        if(m_aRefCount == 0)
        {
            Engine::GetEngine()->GetAssetManager()->UnloadAsset(this);
            delete this;
        }
        else return m_aRefCount;
        return 0;
    }
    bool Asset::Load(Literal Path)
    {
        File* fp = Engine::GetEngine()->GetFileManager()->Open(Path,"r");
        if(!fp)
        {
            Engine::Log("Unable to load asset <%s>\n",Path);
            return false;
        }
        
        if(m_Data) { delete [] (Byte*)m_Data; }
        m_Size    = (i32)fp->GetSize();
        m_Data    = fp->ReadBlock(m_Size);
        m_Pos     = 0;
        m_MemSize = m_Size;
        
        
        //Find out the last modification date, so the engine can reload it if this information changes
        struct tm* Clock;
        struct stat Attrib;
        stat(m_Path.c_str(),&Attrib);
        Clock = gmtime(&(Attrib.st_mtime));
        m_Year   = Clock->tm_year;
        m_Month  = Clock->tm_mon ;
        m_Day    = Clock->tm_mday;
        m_Hour   = Clock->tm_hour;
        m_Minute = Clock->tm_min ;
        m_Second = Clock->tm_sec ;
        
        Engine::GetEngine()->GetFileManager()->Close(fp);
        
        return Reload();
    }
    bool Asset::Reload(Literal Path)
    {
        File* fp = Engine::GetEngine()->GetFileManager()->Open(Path,"r");
        if(!fp)
        {
            Engine::Log("Unable to load asset <%s>\n",Path);
            return false;
        }
        
        if(m_Data) { delete [] (Byte*)m_Data; }
        m_Size    = (i32)fp->GetSize();
        m_Data    = fp->ReadBlock(m_Size);
        m_Pos     = 0;
        m_MemSize = m_Size;
        
        //Find out the last modification date, so the engine can reload it if this information changes
        struct tm* Clock;
        struct stat Attrib;
        stat(m_Path.c_str(),&Attrib);
        Clock = gmtime(&(Attrib.st_mtime));
        m_Year   = Clock->tm_year;
        m_Month  = Clock->tm_mon ;
        m_Day    = Clock->tm_mday;
        m_Hour   = Clock->tm_hour;
        m_Minute = Clock->tm_min ;
        m_Second = Clock->tm_sec ;
        
        Engine::GetEngine()->GetFileManager()->Close(fp);
        
        m_Updated = Reload();
        return m_Updated;
    }
    bool Asset::Save(Literal Path) const
    {
        File* fp = Engine::GetEngine()->GetFileManager()->Open(Path,"w");
        if(!fp)
        {
            Engine::Log("Unable to save asset <%s>\n",Path);
            return false;
        }
        
        bool r = fp->WriteBlock(m_Data,m_Size);
        
        Engine::GetEngine()->GetFileManager()->Close(fp);
        return r;
    }
    void* Asset::Read(i32 Sz)
    {
        if(m_Pos + sizeof(Sz) > m_Size)
        {
            Log("Unable to read %lu bytes from asset <%s>.\n",Sz,m_Path.c_str());
            return 0;
        }
        
        void* Ptr = new char[Sz];
        memcpy(Ptr,GetPtr(),Sz);
        m_Pos += Sz;
        return Ptr;
    }
    void Asset::Write(void* Data,i32 Sz)
    {
        if(m_Pos + Sz > m_MemSize)
        {
            char* tmp = (char*)m_Data;
            i32 m_NewMemSz = m_MemSize + (m_MemSize * 0.2f);
            m_Data = new char[m_NewMemSz];
            memcpy(m_Data,tmp,m_MemSize);
            delete tmp;
            m_MemSize = m_NewMemSz;
        }
        memcpy(GetPtr(),Data,Sz);
        m_Pos += Sz;
    }
    Scalar Asset::ParseScalar()
    {
        string s;
        char c = GetC();
        while(isnumber(c) || c == '.' || c == '-') { s += c; c = GetC(); }
        SetPosition(GetPosition() - 1);
        return stof(s);
    }
    i32 Asset::ParseInt()
    {
        string s;
        char c = GetC();
        while(isnumber(c) || c == '-') { s += c; c = GetC(); }
        SetPosition(GetPosition() - 1);
        return stoi(s);
    }
    bool Asset::ParseBool()
    {
        string s;
        char c = GetC();
        while(isalpha(c)) { s += c; c = GetC(); }
        SetPosition(GetPosition() - 1);
        bool v;
        ParseVal(s,0,0,&v);
        return v;
    }
    Vector2 Asset::ParseVector2()
    {
        char c = GetC();
        while((c == ',' || isspace(c)) && GetPosition() != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v0 = ParseScalar();
        
        c = GetC();
        while((c == ',' || isspace(c)) && GetPosition() != m_Size) c = GetC();
        SetPosition(GetPosition() - 1);
        Scalar v1 = ParseScalar();
        
        return Vector2(v0,v1);
    }
    Vector3 Asset::ParseVector3()
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
        
        return Vector3(v0,v1,v2);
    }
    Vector4 Asset::ParseVector4()
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
        
        return Vector4(v0,v1,v2,v3);
    }
    CString Asset::ReadAll() const
    {
        CString Scpt = MakeCString(m_Size);
        memcpy(Scpt,m_Data,m_Size);
        //Scpt[m_Size] = 0;
        return Scpt;
    }
    bool Asset::HasChanged()
    {
        struct tm* Clock;
        struct stat Attrib;
        stat(m_Path.c_str(),&Attrib);
        Clock = gmtime(&(Attrib.st_mtime));
        
        bool DidChange = false;
        
        if(m_Year < Clock->tm_year)
        {
            DidChange = true;
        }
        else if(m_Year == Clock->tm_year)
        {
            if(m_Month < Clock->tm_mon)
            {
                DidChange = true;
            }
            else if(m_Month == Clock->tm_mon)
            {
                if(m_Day < Clock->tm_mday)
                {
                    DidChange = true;
                }
                else if(m_Day == Clock->tm_mday)
                {
                    if(m_Hour < Clock->tm_hour)
                    {
                        DidChange = true;
                    }
                    else if(m_Hour == Clock->tm_hour)
                    {
                        if(m_Minute < Clock->tm_min)
                        {
                            DidChange = true;
                        }
                        else if(m_Minute == Clock->tm_min)
                        {
                            if(m_Second < Clock->tm_sec)
                            {
                                DidChange = true;
                            }
                        }
                    }
                }
            }
        }
        
        m_Year   = Clock->tm_year;
        m_Month  = Clock->tm_mon ;
        m_Day    = Clock->tm_mday;
        m_Hour   = Clock->tm_hour;
        m_Minute = Clock->tm_min ;
        m_Second = Clock->tm_sec ;
        return DidChange;
    }
    AssetManager::~AssetManager()
    {
    }
    Asset* AssetManager::LoadAsset(const string& Path,ASSET_TYPE Type)
    {
        auto it = m_AssetRef.find(Path);
        if(it != m_AssetRef.end())
        {
            Asset* a = m_Assets[it->second];
            a->AddRef();
            return a;
        }
        
        Asset* Ret = 0;
        switch(Type)
        {
            case AT_PROPERTYLIST:
            {
                Ret = new PropertyList();
                break;
            }
            case AT_SCRIPT:
            {
                Ret = new ScriptComponent();
                break;
            }
            case AT_SHADER:
            {
                Ret = new Shader();
                break;
            }
            case AT_MESH:
            {
                Ret = new Mesh();
                break;
            }
            case AT_TEXTURE:
            {
                Ret = new Texture();
                break;
            }
            case AT_FONT:
            {
                Ret = new Font();
                break;
            }
            default:
            {
                Engine::Log("Error loading asset <%s>. Invalid asset type.\n",Path.c_str());
                return 0;
                break;
            }
        }
        
        Ret->m_aType = Type;
        Ret->m_ID = ((i32)m_Assets.size());
        Ret->m_Path = Path;
        Log("Loading Asset[%d] <%s>.\n",Ret->m_ID,Ret->m_Path.c_str());
        
        /*
         * Push back new asset BEFORE loading, because if the asset type happens to
         * load another asset in its own load function, it will cause problems here
         * Specifically:
         * The asset being loaded from the first asset's load function will get the
         * same ID value because the asset list will not have increased in size yet
         *
         */
        
        m_Assets.push_back(Ret);
        bool s = Ret->Load(Path.c_str());
        if(!s)
        {
            Log("Unable to load asset[%d].\n",Ret->m_ID);
            delete Ret;
            m_Assets.pop_back();
            return 0;
        }
        m_AssetRef.insert(pair<string,i32>(Path,Ret->m_ID));
        return Ret;
    }
    void AssetManager::ReloadChangedAssets()
    {
        if(m_AutoAssetReload)
        {
            if(m_AssetReloadTmr >= m_AssetReloadInterval)
            {
                m_AssetReloadTmr.Stop();
                m_AssetReloadTmr.Start();
                
                for(i32 i = 0;i < m_Assets.size();i++)
                {
                    if(m_Assets[i]->HasChanged())
                    {
                        Log("Asset[%d] <%s> has changed. Attempting reload...\n",i,m_Assets[i]->m_Path.c_str());
                        m_Assets[i]->Reload(m_Assets[i]->m_Path.c_str());
                    }
                }
            }
        }
    }
    void AssetManager::UnloadAsset(Asset* A)
    {
        if(A->m_aRefCount > 0) { Log("Cannot unload asset <%s>. The asset is still in use.\n",A->m_Path.c_str()); return; }
        Log("Unloading Asset[%d] <%s>.\n",A->m_ID,A->m_Path.c_str());
        m_Assets.erase(m_Assets.begin() + A->m_ID);
        m_AssetRef.erase(A->m_Path.c_str());
        for(i32 i = A->m_ID;i < m_Assets.size();i++) m_Assets[i]->m_ID--;
    }
};