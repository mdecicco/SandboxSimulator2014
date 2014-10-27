/*
 *  PropertyList.h
 *
 *  Created by Michael DeCicco on 7/3/14.
 *
 */

#pragma once
#include "EngineTypes.h"
#include "AssetManager.h"
#include "EngineMath.h"

namespace Reality
{
    enum VALUE_TYPE
    {
        VT_INT,
        VT_FLOAT,
        VT_DOUBLE,
        VT_STRING,
        VT_BOOLEAN,
        VT_VECTOR2,
        VT_VECTOR3,
        VT_VECTOR4,
        VT_COUNT
    };
    
    /* Helper functions */
    Literal GetUnitLongName(UNIT Unit);
    Literal GetUnitLongName_p(UNIT Unit);
    Literal GetUnitShortName(UNIT Unit);
    UNIT GetUnitFromName(Literal Name);
    void* AllocVal(VALUE_TYPE Typ);
    void FreeVal(VALUE_TYPE Typ,void* Val);
    CString GetValueString(VALUE_TYPE Typ,UNIT uTyp,void* Val);
    void ParseVal(const string& ValStr,VALUE_TYPE* ovType,UNIT* ouType,void* oVal);
    void* ParseVal(const string& ValStr,VALUE_TYPE* ovType,UNIT* ouType);
    
    class PNode
    {
        public:
            PNode();
            PNode(const PNode& copy);
            PNode(const string& Name,i32 Value);
            PNode(const string& Name,f32 Value);
            PNode(const string& Name,f64 Value);
            PNode(const string& Name,const string& Value);
            PNode(const string& Name,bool Value);
            PNode(const string& Name,const Vector2& Value);
            PNode(const string& Name,const Vector3& Value);
            PNode(const string& Name,const Vector4& Value);
            ~PNode();
            
            PNode* Search(Literal Name) const;
        
            CString GetName() { return Name; }
            void SetName(CString Name,i32 NameLen);
        
            void SetValue(i32 Value);
            void SetValue(f32 Value);
            void SetValue(f64 Value);
            void SetValue(const string& Value);
            void SetValue(bool Value);
            void SetValue(const Vector2& Value);
            void SetValue(const Vector3& Value);
            void SetValue(const Vector4& Value);
            void SetUnit(const string& Unit);
            void SetUnit(UNIT Unit) { UnitType = Unit; }
            UNIT GetUnit() const { return UnitType; }
            VALUE_TYPE GetType() const { return Type; }
        
            bool IsValue() const { return ValuePtr != 0; }
        
            operator i32() const;
            operator f32() const;
            operator f64() const;
            operator CString() const;
            operator bool() const;
            operator Vector2() const;
            operator Vector3() const;
            operator Vector4() const;
        
            i32 GetChildCount() const { return ChildCount; }
            PNode* GetChild(i32 CID) { return &Children[CID]; }
        
        protected:
            friend class PropertyList;
            CString Name;
            i32 NameLen;
            void* ValuePtr;
            VALUE_TYPE Type;
            UNIT UnitType;
            PNode* Children;
            i32 ChildCount;
    };
    
    class File;
    class PropertyList : public Asset
    {
        public:
    
            virtual bool Reload();
        
            PNode* Search(Literal Name) const;
        
            void PrintNode(i32 l,PNode* N) const;
        protected:
            friend class AssetManager;
            PropertyList();
            ~PropertyList();
            
            bool ReadNode(PNode* N);
            bool WriteNode(PNode* N);
            
            PNode* m_Nodes;
            i32 m_NodeCount;
    };
};