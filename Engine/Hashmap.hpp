/*
 *  Hashmap.hpp
 *
 *  Created by Michael DeCicco on 6/25/14.
 *
 */

#pragma once
#include "EngineTypes.h"
#include "Log.h"

#include <unordered_map>
#include <string>
using namespace std;

namespace Reality
{
    template <typename DataType>
    class Hashmap
    {
        public:
            typedef typename unordered_map<string,DataType,hash<string> >::const_iterator Iterator;
            Hashmap()
            {
            }
            ~Hashmap()
            {
            }
            void Clear() { m_Map.clear(); }
            bool Contains(const string& Key) const { return m_Map.find(Key) != m_Map.end(); }
            Iterator Find(const string& Key) const { return m_Map.find(Key); }
            Iterator Begin() const { return m_Map.begin(); }
            Iterator End()   const { return m_Map.end  (); }
        
            void Insert(const DataType& Val,Literal Key)
            {
                auto i = m_Map.find(Key);
                m_Map.insert(i,pair<string,DataType>(Key,Val));
            }
            DataType& operator[](const string& Key)
            {
                auto i = m_Map.find(Key);
                if(i == m_Map.end())
                {
                    Log("Warning: Hash map does not contain key <%s>. Unexpected results will ensue.\n",Key.c_str());
                }
                return i->second;
            }
            void Remove(const string& Key)
            {
                m_Map.erase(Key);
            }
        
        protected:
            unordered_map<string,
                          DataType,
                          hash<string> > m_Map;
    };
};