/*
 *  AssetManager.h
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#pragma once
#include "EngineTypes.h"
#include "Log.h"
#include "Hashmap.hpp"
#include "Timer.h"
#include "EngineMath.h"

#include <vector>
using namespace std;

namespace Reality
{
    enum ASSET_TYPE
    {
        AT_PROPERTYLIST,
        AT_SCRIPT,
        AT_SHADER,
        AT_TEXTURE,
        AT_MESH,
        AT_FONT,
        AT_INVALID
    };
    
    class Asset
    {
        public:
            Asset() : m_aType(AT_INVALID), m_ID(-1), m_aRefCount(1), m_Data(0), m_Updated(false) { }
        
            i32 AddRef() { return m_aRefCount++; }
            i32 Destroy();
        
            bool Load(Literal Path);
            bool Reload(Literal Path);
            bool Save(Literal Path) const;
        
            virtual bool Reload() = 0;
        
            template <typename T>
            void Read(T& OutValue)
            {
                if(m_Pos + sizeof(T) > m_Size)
                {
                    Log("Unable to read %lu bytes from asset.\n",sizeof(T));
                }
                memcpy(&OutValue,*(T*)((intptr_t)m_Data + m_Pos),sizeof(OutValue));
                m_Pos += sizeof(T);
            }
            void* Read(i32 Sz);
        
            template <typename T>
            void Write(const T& InValue)
            {
                if(m_Pos + sizeof(T) > m_MemSize)
                {
                    char* tmp = (char*)m_Data;
                    i32 m_NewMemSz = m_MemSize + (m_MemSize * 0.2f);
                    m_Data = new char[m_NewMemSz];
                    memcpy(m_Data,tmp,m_MemSize);
                    delete tmp;
                    m_MemSize = m_NewMemSz;
                }
                if(m_Pos + sizeof(T) > m_Size) m_Size += (sizeof(T) - (m_Size - m_Pos));
                *(T*)((intptr_t)m_Data + m_Pos) = InValue;
                m_Pos += sizeof(T);
            }
            void Write(void* Data,i32 Sz);
            Scalar ParseScalar();
            i32 ParseInt();
            bool ParseBool();
            Vector2 ParseVector2();
            Vector3 ParseVector3();
            Vector4 ParseVector4();
        
            void* GetPtr() const { return (void*)(((intptr_t)m_Data) + m_Pos); }
        
            char GetC() { return ((CString)m_Data)[m_Pos++]; }
        
            CString ReadAll() const;
        
            i32 GetPosition() const { return m_Pos; }
            void SetPosition(i32 Pos) { m_Pos = Pos; }
            i32 GetSize() const { return m_Size; }
        
            bool AtEnd() const { return m_Pos == m_Size; }
        
            bool HasChanged();
            bool HasUpdated() const { return m_Updated; }
            void ResetUpdateFlag() { m_Updated = false; }
        
            const string& GetPath() const { return m_Path; }
        
        protected:
            friend class AssetManager;
            virtual ~Asset() { if(m_Data) delete (Byte*)m_Data; }
        
            ASSET_TYPE m_aType;
            i32 m_ID;
            i32 m_aRefCount;
        
            i32 m_Pos;
            i32 m_Size;
            i32 m_MemSize;
            void* m_Data;
        
            i32 m_Year;
            i32 m_Month;
            i32 m_Day;
            i32 m_Hour;
            i32 m_Minute;
            i32 m_Second;
        
            bool m_Updated;
            string m_Path;
    };

    class AssetManager
    {
        public:
            AssetManager() : m_AutoAssetReload(true), m_AssetReloadInterval(Scalar(1)) { m_AssetReloadTmr.Start(); }
            ~AssetManager();
        
            Asset* LoadAsset(const string& Path,ASSET_TYPE Type);
        
            void ReloadChangedAssets();
        
            void SetAutoAssetReloading(bool flag) { m_AutoAssetReload = flag; }
            bool IsAutoAssetReloadingEnabled() const { return m_AutoAssetReload; }
            void SetAssetCheckInterval(Scalar Interval) { m_AssetReloadInterval = Interval; }
            Scalar GetAssetCheckInterval() const { return m_AssetReloadInterval; }
        
        protected:
            friend class Asset;
            void UnloadAsset(Asset* A);
            vector<Asset*> m_Assets;
            unordered_map<string,i32,hash<string>> m_AssetRef;
            bool m_AutoAssetReload;
            Scalar m_AssetReloadInterval;
            Timer m_AssetReloadTmr;
    };
};