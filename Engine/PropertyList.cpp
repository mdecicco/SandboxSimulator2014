/*
 *  PropertyList.cpp
 *
 *  Created by Michael DeCicco on 7/3/14.
 *
 */

#include "PropertyList.h"
#include "Log.h"

#include <stdlib.h>

namespace Reality
{
    /* Unit long names */
    static Literal UnitLongNames[SUPPORTED_UNIT_NUM + 1] =
        {
            "Gigaparsec",
            "Megaparsec",
            "Kiloparsec",
            "Parsec",
            "Milliparsec",
            "Microparsec",
            "Nanoparsec",
            "Picoparsec",
            "Femtoparsec",
            "Attoparsec",
            "Gigameter",
            "Megameter",
            "Kilometer",
            "Meter",
            "Second",
            "Minute",
            "Hour",
            "Day",
            "Week",
            "Month",
            "Year",
            "Decade",
            "Century",
            "Millennium",
            "",
        };
    
    /* Plural unit names */
    static Literal UnitLongNames_p[SUPPORTED_UNIT_NUM + 1] =
        {
            "Gigaparsecs",
            "Megaparsecs",
            "Kiloparsecs",
            "Parsecs",
            "Milliparsecs",
            "Microparsecs",
            "Nanoparsecs",
            "Picoparsecs",
            "Femtoparsecs",
            "Attoparsecs",
            "Gigameters",
            "Megameters",
            "Kilometers",
            "Meters",
            "Seconds",
            "Minutes",
            "Hours",
            "Days",
            "Weeks",
            "Months",
            "Years",
            "Decades",
            "Centuries",
            "Millennia",
            "",
        };
    
    /* Unit short names */
    static Literal UnitShortNames[SUPPORTED_UNIT_NUM + 1] =
        {
            "Gpc",
            "Mpc",
            "kpc",
            "pc",
            "mpc",
            "upc",
            "npc",
            "ppc",
            "fpc",
            "apc",
            "Gm",
            "Mm",
            "km",
            "m",
            "sec",
            "min",
            "hr",
            "day",
            "wk",
            "mon",
            "yr",
            "dec",
            "cen",
            "mil",
            "",
        };
    Literal GetUnitLongName  (UNIT Unit) { return UnitLongNames  [Unit]; }
    Literal GetUnitLongName_p(UNIT Unit) { return UnitLongNames_p[Unit]; }
    Literal GetUnitShortName (UNIT Unit) { return UnitShortNames [Unit]; }
    UNIT GetUnitFromName(Literal Name)
    {
        if(isupper(Name[0]))
        {
            //Could either be Long name, long plural name or one of the 'G' or 'M' short names
            if(Name[0] == 'M')
            {
                switch(Name[1])
                {
                    case 'p': return MEGAPARSEC;
                    case 'm': return MEGAMETER;
                    case 'i':
                    {
                        if(Name[4] == 'i') return MILLIPARSEC;
                        else if(Name[4] == 'o') return MICROPARSEC;
                        else if(Name[4] == 't') return MINUTE;
                        else if(Name[4] == 'e') return MILLENNIUM;
                        return SUPPORTED_UNIT_NUM;
                    }
                    case 'e':
                    {
                        if(Name[4] == 'p') return MEGAPARSEC;
                        else if(Name[4] == 'm') return MEGAMETER;
                        else if(Name[2] == 't') return METER;
                        return SUPPORTED_UNIT_NUM;
                    }
                    case 'o': return MONTH;
                }
            }
            else if(Name[0] == 'G')
            {
                switch(Name[1])
                {
                    case 'p': return GIGAPARSEC;
                    case 'm': return GIGAMETER;
                    case 'i':
                    {
                        if(Name[4] == 'p') return GIGAPARSEC;
                        else if(Name[4] == 'm') return GIGAMETER;
                        else return SUPPORTED_UNIT_NUM;
                    }
                }
            }
            else if(Name[0] == 'K')
            {
                if(Name[4] == 'p') return KILOPARSEC;
                else if(Name[4] == 'm') return KILOMETER;
                return SUPPORTED_UNIT_NUM;
            }
            else if(Name[0] == 'P')
            {
                if(Name[5] == 'c') return PARSEC;
                else if(Name[5] == 'a') return PICOPARSEC;
                return SUPPORTED_UNIT_NUM;
            }
            else if(Name[0] == 'D')
            {
                if(Name[1] == 'a') return DAY;
                else if(Name[1] == 'e') return DECADE;
                return SUPPORTED_UNIT_NUM;
            }
            else if(Name[0] == 'N' && Name[8] == 'e') return NANOPARSEC;
            else if(Name[0] == 'S' && Name[4] == 'n') return SECOND;
            else if(Name[0] == 'H' && Name[4] == 's') return HOUR;
            else if(Name[0] == 'W' && Name[3] == 'k') return WEEK;
            else if(Name[0] == 'C' && Name[4] == 'u') return CENTURY;
            else if(Name[0] == 'Y' && Name[3] == 'r') return YEAR;
            else if(Name[0] == 'F' && Name[4] == 'o') return FEMTOPARSEC;
            else if(Name[0] == 'A' && Name[4] == 'p') return ATTOPARSEC;
        }
        else if(Name[0] == 'm')
        {
            if(Name[1] == 'i')
            {
                if(Name[2] == 'l') return MILLENNIUM;
                else if(Name[2] == 'n') return MINUTE;
                return SUPPORTED_UNIT_NUM;
            }
            else if(Name[1] == 'p') return MILLIPARSEC;
            else if(Name[1] == 'o') return MONTH;
            else if(!isalnum(Name[1])) return METER;
            return SUPPORTED_UNIT_NUM;
        }
        else if(Name[0] == 'k')
        {
            if(Name[1] == 'p') return KILOPARSEC;
            else if(Name[1] == 'm') return KILOMETER;
            return SUPPORTED_UNIT_NUM;
        }
        else if(Name[0] == 'p')
        {
            if(Name[1] == 'c') return PARSEC;
            else if(Name[1] == 'p') return PICOPARSEC;
            return SUPPORTED_UNIT_NUM;
        }
        else if(Name[0] == 'd')
        {
            if(Name[1] == 'e') return DECADE;
            else if(Name[1] == 'a') return DAY;
            return SUPPORTED_UNIT_NUM;
        }
        else if(Name[0] == 'h' && Name[1] == 'r') return HOUR;
        else if(Name[0] == 'y' && Name[1] == 'r') return YEAR;
        else if(Name[0] == 'w' && Name[1] == 'k') return WEEK;
        else if(Name[0] == 's' && Name[1] == 'e') return SECOND;
        else if(Name[0] == 'c' && Name[1] == 'e') return CENTURY;
        else if(Name[0] == 'u' && Name[1] == 'p') return MICROPARSEC;
        else if(Name[0] == 'f' && Name[1] == 'p') return FEMTOPARSEC;
        else if(Name[0] == 'n' && Name[1] == 'p') return NANOPARSEC;
        else if(Name[0] == 'a' && Name[1] == 'p') return ATTOPARSEC;
        return SUPPORTED_UNIT_NUM;
    }
    void* AllocVal(VALUE_TYPE Typ)
    {
        switch(Typ)
        {
            case VT_INT: return new i32(0);
            case VT_FLOAT: return new f32(0);
            case VT_DOUBLE: return new f64(0);
            case VT_STRING: return new string();
            case VT_BOOLEAN: return new bool(0);
            case VT_VECTOR2: return new Vector2();
            case VT_VECTOR3: return new Vector3();
            case VT_VECTOR4: return new Vector4();
            default: Log("Error allocating memory for variable. (Invalid value type)\n");
        }
       
        return 0;
    }
    void FreeVal(VALUE_TYPE Typ,void* Val)
    {
        switch(Typ)
        {
            case VT_INT    : { delete (i32*)Val; break; }
            case VT_FLOAT  : { delete (f32*)Val; break; }
            case VT_DOUBLE : { delete (f64*)Val; break; }
            case VT_STRING : { delete (string*)Val; break; }
            case VT_BOOLEAN: { delete (bool*)Val; break; }
            case VT_VECTOR2: { delete (Vector2*)Val; break; }
            case VT_VECTOR3: { delete (Vector3*)Val; break; }
            case VT_VECTOR4: { delete (Vector4*)Val; break; }
            default: Log("Error freeing memory for variable. (Invalid value type)\n");
        }
    }
    CString GetValueString(VALUE_TYPE Typ,UNIT uTyp,void* Val)
    {
        switch(Typ)
        {
            case VT_INT:
            {
                CString s = MakeCString(32);
                snprintf(s,32,"%d%s",*(i32*)Val,GetUnitShortName(uTyp));
                return s;
            }
            case VT_FLOAT:
            {
                CString s = MakeCString(32);
                snprintf(s,32,"%f%s",*(f32*)Val,GetUnitShortName(uTyp));
                return s;
            }
            case VT_DOUBLE:
            {
                CString s = MakeCString(32);
                snprintf(s,32,"%f%s",*(f64*)Val,GetUnitShortName(uTyp));
                return s;
            }
            case VT_STRING:
            {
                CString s = MakeCString(32);
                snprintf(s,32,"%s",((string*)Val)->c_str());
                return s;
            }
            case VT_BOOLEAN:
            {
                CString s = MakeCString(32);
                snprintf(s,32,"%s",(*(bool*)Val) ? "true" : "false");
                return s;
            }
            case VT_VECTOR2:
            {
                CString s = MakeCString(32);
                Literal UnitShortName = GetUnitShortName(uTyp);
                Vector2 *v = (Vector2*)Val;
                snprintf(s,32,"%f%s,%f%s",v->x,UnitShortName,v->y,UnitShortName);
                return s;
            }
            case VT_VECTOR3:
            {
                CString s = MakeCString(32);
                Literal UnitShortName = GetUnitShortName(uTyp);
                Vector3 *v = (Vector3*)Val;
                snprintf(s,32,"%f%s,%f%s,%f%s",v->x,UnitShortName,v->y,UnitShortName,v->z,UnitShortName);
                return s;
            }
            case VT_VECTOR4:
            {
                CString s = MakeCString(32);
                Literal UnitShortName = GetUnitShortName(uTyp);
                Vector4 *v = (Vector4*)Val;
                snprintf(s,32,"%f%s,%f%s,%f%s,%f%s",v->x,UnitShortName,v->y,UnitShortName,v->z,UnitShortName,v->w,UnitShortName);
                return s;
            }
            default:
            {
                return 0;
            }
        }
        return 0;
    }
    void ParseVal(const string& ValStr,VALUE_TYPE* ovType,UNIT* ouType,void* oVal)
    {
        if(ValStr.find('.') != ValStr.npos)
        {
            Scalar f = 0.0f;
            #ifdef USE_DOUBLE_SCALAR
                if(ovType) *ovType = VT_DOUBLE;
            #else
                if(ovType) *ovType = VT_FLOAT;
            #endif
            
            string ustr;
            u8 stage = 0;
            string v;
            for(i32 i = 0;i < ValStr.length();i++)
            {
                if(ValStr[i] == ' ') continue;
                if(isnumber(ValStr[i]) || ValStr[i] == '.') { stage = 1; v.push_back(ValStr[i]); }
                else if(stage == 1) { ustr = ValStr.substr(i,ValStr.length()); break; }
            }
            f = atof(v.c_str());
            #ifdef USE_DOUBLE_SCALAR
                (*(f64*)oVal) = f;
            #else
                (*(f32*)oVal) = f;
            #endif
            if(ouType) *ouType = GetUnitFromName(ustr.c_str());
        }
        else
        {
            if(ovType) *ovType = VT_INT;
            string ustr;
            u8 stage = 0;
            string v;
            for(i32 i = 0;i < ValStr.length();i++)
            {
                if(ValStr[i] == ' ') continue;
                if(isnumber(ValStr[i]) || ValStr[i] == '.' || (ValStr[i] == '-' && stage == 0)) { stage = 1; v.push_back(ValStr[i]); }
                else if(stage == 1) { ustr = ValStr.substr(i,ValStr.length()); break; }
            }
            
            #ifdef USE_DOUBLE_SCALAR
            if(!ovType) (*(f64*)oVal) = (f64)atoi(v.c_str());
            else (*(i32*)oVal) = atoi(v.c_str());
            #else
            if(!ovType) (*(f32*)oVal) = (f32)atoi(v.c_str());
            else (*(i32*)oVal) = atoi(v.c_str());
            #endif
            
            if(ouType) *ouType = GetUnitFromName(ustr.c_str());
        }
    }
    void* ParseVal(const string& ValStr,VALUE_TYPE* ovType,UNIT* ouType)
    {
        if(ValStr.find('.') != ValStr.npos)
        {
            Scalar f = 0.0f;
            #ifdef USE_DOUBLE_SCALAR
            if(ovType) *ovType = VT_DOUBLE;
            #else
            if(ovType) *ovType = VT_FLOAT;
            #endif
            
            string ustr;
            u8 stage = 0;
            string v;
            for(i32 i = 0;i < ValStr.length();i++)
            {
                if(ValStr[i] == ' ') continue;
                if(isnumber(ValStr[i]) || ValStr[i] == '.') { stage = 1; v.push_back(ValStr[i]); }
                else if(stage == 1) { ustr = ValStr.substr(i,ValStr.length()); break; }
            }
            f = atof(v.c_str());
            if(ouType) *ouType = GetUnitFromName(ustr.c_str());
            
            return new Scalar(f);
        }
        else
        {
            if(ovType) *ovType = VT_INT;
            string ustr;
            u8 stage = 0;
            string v;
            for(i32 i = 0;i < ValStr.length();i++)
            {
                if(ValStr[i] == ' ') continue;
                if(isnumber(ValStr[i]) || ValStr[i] == '.') { stage = 1; v.push_back(ValStr[i]); }
                else if(stage == 1) { ustr = ValStr.substr(i,ValStr.length()); break; }
            }
            return new int(atoi(v.c_str()));
        }
        return 0;
    }
    
    PNode::PNode() : Name(0), NameLen(0), Children(0), ChildCount(0), ValuePtr(0), Type(VT_COUNT), UnitType(SUPPORTED_UNIT_NUM)
    {
    }
    PNode::PNode(const PNode& copy)
    {
        NameLen = copy.NameLen;
        if(NameLen > 0) { Name = MakeCString(NameLen); memcpy(Name,copy.Name,NameLen); }
        else Name = 0;
        ValuePtr = copy.ValuePtr;
        ChildCount = copy.ChildCount;
        Type = copy.Type;
        UnitType = copy.UnitType;
        if(ChildCount > 0) Children = new PNode[ChildCount];
        else Children = 0;
        for(i32 i = 0;i < ChildCount;i++) Children[i] = copy.Children[i];
    }
    PNode::PNode(const string& vName,i32 Value)
    {
        NameLen = (i32)vName.length();
        Name = MakeCString(NameLen);
        vName.copy(Name,NameLen);
        SetValue(Value);
    }
    PNode::PNode(const string& vName,f32 Value)
    {
        NameLen = (i32)vName.length();
        Name = MakeCString(NameLen);
        vName.copy(Name,NameLen);
        SetValue(Value);
    }
    PNode::PNode(const string& vName,f64 Value)
    {
        NameLen = (i32)vName.length();
        Name = MakeCString(NameLen);
        vName.copy(Name,NameLen);
        SetValue(Value);
    }
    PNode::PNode(const string& vName,const string& Value)
    {
        NameLen = (i32)vName.length();
        Name = MakeCString(NameLen);
        vName.copy(Name,NameLen);
        SetValue(Value);
    }
    PNode::PNode(const string& vName,bool Value)
    {
        NameLen = (i32)vName.length();
        Name = MakeCString(NameLen);
        vName.copy(Name,NameLen);
        SetValue(Value);
    }
    PNode::PNode(const string& vName,const Vector2& Value)
    {
        NameLen = (i32)vName.length();
        Name = MakeCString(NameLen);
        vName.copy(Name,NameLen);
        SetValue(Value);
    }
    PNode::PNode(const string& vName,const Vector3& Value)
    {
        NameLen = (i32)vName.length();
        Name = MakeCString(NameLen);
        vName.copy(Name,NameLen);
        SetValue(Value);
    }
    PNode::PNode(const string& vName,const Vector4& Value)
    {
        NameLen = (i32)vName.length();
        Name = MakeCString(NameLen);
        vName.copy(Name,NameLen);
        SetValue(Value);
    }
    PNode::~PNode()
    {
        if(Name) delete [] Name;
        if(ValuePtr) FreeVal(Type,ValuePtr);
        if(Children) delete [] Children;
    }
    PNode* PNode::Search(Literal Name) const
    {
        for(i32 i = 0;i < ChildCount;i++)
        {
            if(!strncmp(Children[i].Name,Name,Children[i].NameLen))
            {
                return &Children[i];
            }
        }
        return  0;
    }
    void PNode::SetName(CString name,i32 nameLen)
    {
        if(Name) delete [] Name;
        NameLen = nameLen + 1;
        Name = MakeCString(NameLen);
        memset(Name,0,NameLen);
        for(i32 i = 0;i < nameLen;i++) Name[i] = name[i];
    }
    void PNode::SetValue(i32 Value)
    {
        if(ValuePtr) FreeVal(Type,ValuePtr);
        Type = VT_INT;
        ValuePtr = AllocVal(Type);
        *((i32*)ValuePtr) = Value;
        printf("%d%s.\n",*(i32*)ValuePtr,GetUnitShortName(UnitType));
    }
    void PNode::SetValue(f32 Value)
    {
        if(ValuePtr) FreeVal(Type,ValuePtr);
        Type = VT_FLOAT;
        ValuePtr = AllocVal(Type);
        *((f32*)ValuePtr) = Value;
    }
    void PNode::SetValue(f64 Value)
    {
        if(ValuePtr) FreeVal(Type,ValuePtr);
        Type = VT_DOUBLE;
        ValuePtr = AllocVal(Type);
        *((f64*)ValuePtr) = Value;
    }
    void PNode::SetValue(const string& Value)
    {
        if(ValuePtr) FreeVal(Type,ValuePtr);
        Type = VT_STRING;
        ValuePtr = AllocVal(Type);
        *((string*)ValuePtr) = Value;
    }
    void PNode::SetValue(bool Value)
    {
        if(ValuePtr) FreeVal(Type,ValuePtr);
        Type = VT_BOOLEAN;
        ValuePtr = AllocVal(Type);
        *((bool*)ValuePtr) = Value;
    }
    void PNode::SetValue(const Vector2& Value)
    {
        if(ValuePtr) FreeVal(Type,ValuePtr);
        Type = VT_VECTOR2;
        ValuePtr = AllocVal(Type);
        *((Vector2*)ValuePtr) = Value;
    }
    void PNode::SetValue(const Vector3& Value)
    {
        if(ValuePtr) FreeVal(Type,ValuePtr);
        Type = VT_VECTOR3;
        ValuePtr = AllocVal(Type);
        *((Vector3*)ValuePtr) = Value;
    }
    void PNode::SetValue(const Vector4& Value)
    {
        if(ValuePtr) FreeVal(Type,ValuePtr);
        Type = VT_VECTOR4;
        ValuePtr = AllocVal(Type);
        *((Vector4*)ValuePtr) = Value;
    }
    PNode::operator i32() const
    {
        if(Type == VT_FLOAT) return (*(f32*)ValuePtr);
        else if(Type == VT_DOUBLE) return (*(f64*)ValuePtr);
        else if(Type != VT_INT) Log("Cannot cast node to int, incompatable type. Unexpected results will ensue.\n");
        return *(i32*)ValuePtr;
    }
    PNode::operator f32() const
    {
        if(Type == VT_INT) return (*(i32*)ValuePtr);
        else if(Type == VT_DOUBLE) return (*(f64*)ValuePtr);
        else if(Type != VT_FLOAT) Log("Cannot cast node to float, incompatable type. Unexpected results will ensue.\n");
        return *(f32*)ValuePtr;
    }
    PNode::operator f64() const
    {
        if(Type == VT_INT) return (*(i32*)ValuePtr);
        else if(Type == VT_FLOAT) return (*(f32*)ValuePtr);
        else if(Type != VT_DOUBLE) Log("Cannot cast node to double, incompatable type. Unexpected results will ensue.\n");
        return *(f64*)ValuePtr;
    }
    PNode::operator CString() const
    {
        return &(*((string*)ValuePtr))[0];
    }
    PNode::operator bool() const
    {
        return *(bool*)ValuePtr;
    }
    PNode::operator Vector2() const
    {
        return *(Vector2*)ValuePtr;
    }
    PNode::operator Vector3() const
    {
        return *(Vector3*)ValuePtr;
    }
    PNode::operator Vector4() const
    {
        return *(Vector4*)ValuePtr;
    }
    
    PropertyList::PropertyList() : m_Nodes(0), m_NodeCount(0)
    {
    }
    PropertyList::~PropertyList()
    {
        if(m_Nodes) delete [] m_Nodes;
    }
    void PropertyList::PrintNode(i32 l,PNode* N) const
    {
        for(i32 i = 0;i < l;i++) printf("\t");
        printf("<%s>",N->Name);
        if(N->ChildCount > 0)
        {
            printf("\n");
            for(i32 i = 0;i < N->ChildCount;i++) PrintNode(l + 1,&N->Children[i]);
            for(i32 i = 0;i < l;i++) printf("\t");
        }
        else
        {
            CString Vstr = GetValueString(N->Type,N->UnitType,N->ValuePtr);
            printf("%s",Vstr);
            FreeCString(Vstr);
        }
        printf("</%s>\n",N->Name);
    }
    bool PropertyList::Reload()
    {
        if(m_Nodes) delete [] m_Nodes;
        m_Nodes = 0;
    
        vector<PNode*> Nodes;
        while(!AtEnd())
        {
            //GetC until '<'
            while(GetC() != '<')
            {
                if(AtEnd()) { Log("Unable to read property list. (No nodes found)\n"); return false; }
            }
            SetPosition(GetPosition() - 1);
            PNode* Node = new PNode();
            if(!ReadNode(Node))
            {
                Log("Unable to load property list. (invalid node)\n");
                delete Node;
                for(i32 i = 0;i < Nodes.size();i++) delete Nodes[i];
                return false;
            }
            Nodes.push_back(Node);
        }
        m_NodeCount = (i32)Nodes.size();
        if(m_NodeCount > 0)
        {
            m_Nodes = new PNode[m_NodeCount];
            for(i32 i = 0;i < m_NodeCount;i++)
            {
                m_Nodes[i] = *Nodes[i];
                
                //So that they don't get deallocated
                Nodes[i]->Children = 0;
                Nodes[i]->Name = 0;
                Nodes[i]->ValuePtr = 0;
                
                delete Nodes[i];
            }
        }
        return true;
    }
    PNode* PropertyList::Search(Literal Name) const
    {
        for(i32 i = 0;i < m_NodeCount;i++)
        {
            if(!strncmp(m_Nodes[i].Name,Name,m_Nodes[i].NameLen))
            {
                return &m_Nodes[i];
            }
        }
        return  0;
    }
    bool PropertyList::ReadNode(PNode* N)
    {
        //GetC until '<'
        while(GetC() != '<')
        {
            if(AtEnd()) { Log("Unable to read node. (No \'<\' character)\n"); return false; }
        }
        vector<char>Name;
        char c = GetC();
        while(c != '>')
        {
            if(AtEnd()) { Log("Unable to read node. (Expected \'>\' after \'<\')\n"); return false; }
            Name.push_back(c);
            c = GetC();
        }
        SetPosition(GetPosition() - 1);
        
        if(Name.size() == 0) { Log("Unable to read node. (Expected identifier between \'<\' and \'>\')\n"); return false; }
        N->SetName(&Name[0],(i32)Name.size());
        
        if(GetC() != '>') { Log("Unable to read node. (Expected \'>\' after identifier <%s>)\n",&Name[0]); return false; }
        vector<PNode*> Nodes;
        bool IsVal = false;
        while(!AtEnd())
        {
            c = GetC();
            //Child node or end of node
            if(c == '<')
            {
                SetPosition(GetPosition() - 1);
                if(IsVal || Nodes.size() > 0)
                {
                    GetC();
                    i32 Pos = GetPosition();
                    char c0 = GetC();
                    vector<char> Name0;
                    while(c0 != '>')
                    {
                        if(AtEnd()) { Log("Unable to read node. (Expected \'>\' after \'<\')\n"); return false; }
                        Name0.push_back(c0);
                        c0 = GetC();
                    }
                    if(Name0[0] == '/')
                    {
                        if(!strncmp(N->Name,&Name0[1],Name.size()))
                        {
                            break;
                        }
                        else
                        {
                            Log("Unable to read node. (Expected \"</%s>\")\n",N->Name);
                            return false;
                        }
                    }
                    SetPosition(Pos - 1);
                }
                
                PNode* Node = new PNode();
                if(!ReadNode(Node))
                {
                    delete Node;
                    for(i32 i = 0;i < Nodes.size();i++) delete Nodes[i];
                    return false;
                }
                Nodes.push_back(Node);
                continue;
            }
            else if(c == '\n' || c == '\r')
            {
                //Loop until whitespace ends
                char c0 = GetC();
                while(c0 == ' ') { c0 = GetC(); if(AtEnd()) { Log("Unable to read node. (Expected value or identifier after \'>\')\n"); return false; } }
                SetPosition(GetPosition() - 1);
                continue;
            }
            else //Value
            {
                SetPosition(GetPosition() - 1);
                //GetC until '<', adding chars to the value string
                string Val;
                char c0 = GetC();
                while(c0 != '<')
                {
                    if(AtEnd()) { Log("Unable to read node. (Expected \'<\' after value)\n"); return false; }
                    Val.push_back(c0);
                    c0 = GetC();
                }
                SetPosition(GetPosition() - 1);
                if(Val.length() == 0) { Log("Unable to read node. (Expected value between \'>\' and \'<\')\n"); return false; }
                
                i32 ccount = 0;
                string v1,v2,v3,v4;
                i32 idx[3];
                if(!isalpha(Val[0]))
                {
                    for(i32 i = 0;i < Val.length();i++)
                    {
                        if(Val[i] == ',') idx[ccount++] = i;
                    }
                    if(ccount == 0)
                    {
                        N->ValuePtr = ParseVal(Val,&N->Type,&N->UnitType);
                    }
                    else if(ccount == 1)
                    {
                        #ifdef USE_DOUBLE_SCALAR
                        N->Type = VT_DOUBLE;
                        #else
                        N->Type = VT_FLOAT;
                        #endif
                        
                        Vector2 v;
                        v1 = Val.substr(0,idx[0]);
                        ParseVal(v1,0,&N->UnitType,&v.x);
                        v2 = Val.substr(idx[0] + 1,Val.length());
                        ParseVal(v2,0,0,&v.y);
                        
                        N->SetValue(v);
                    }
                    else if(ccount == 2)
                    {
                        #ifdef USE_DOUBLE_SCALAR
                        N->Type = VT_DOUBLE;
                        #else
                        N->Type = VT_FLOAT;
                        #endif
                        
                        Vector3 v;
                        for(i32 i = 0;i < idx[0];i++) v1.push_back(Val[i]);
                        ParseVal(v1,0,&N->UnitType,&v.x);
                        for(i32 i = idx[0] + 1;i < idx[1];i++) v2.push_back(Val[i]);
                        ParseVal(v2,0,0,&v.y);
                        for(i32 i = idx[1] + 1;i < Val.size();i++) v3.push_back(Val[i]);
                        ParseVal(v3,0,0,&v.z);
                        
                        N->SetValue(v);
                    }
                    else if(ccount == 3)
                    {
                        #ifdef USE_DOUBLE_SCALAR
                        N->Type = VT_DOUBLE;
                        #else
                        N->Type = VT_FLOAT;
                        #endif
                        
                        Vector4 v;
                        v1 = Val.substr(0,idx[0] - 1);
                        ParseVal(v1,0,&N->UnitType,&v.x);
                        v2 = Val.substr(idx[0] + 1,idx[1]);
                        ParseVal(v2,0,0,&v.y);
                        v3 = Val.substr(idx[1] + 1,idx[2]);
                        ParseVal(v3,0,0,&v.z);
                        v3 = Val.substr(idx[2] + 1,Val.length());
                        ParseVal(v3,0,0,&v.w);
                        
                        N->SetValue(v);
                    }
                }
                else
                {
                    //It must be a string or a bool
                    if(Val == "true") N->SetValue(true);
                    else if(Val == "false") N->SetValue(false);
                    else N->SetValue(Val);
                }
                
                IsVal = true;
                continue;
            }
        }
        
        N->ChildCount = (i32)Nodes.size();
        if(N->ChildCount > 0)
        {
            N->Children = new PNode[N->ChildCount];
            for(i32 i = 0;i < N->ChildCount;i++)
            {
                N->Children[i] = *Nodes[i];
                
                //So that they don't get deallocated
                Nodes[i]->Children = 0;
                Nodes[i]->Name = 0;
                Nodes[i]->ValuePtr = 0;
                
                delete Nodes[i];
            }
        }
        return true;
    }
    bool PropertyList::WriteNode(PNode* N)
    {
        return false;
    }
};