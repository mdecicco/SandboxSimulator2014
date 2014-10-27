/*
 *  GUIManager.h
 *
 *  Created by Michael DeCicco on 9/9/14.
 *
 */

#pragma once
#include "EngineMath.h"
#include <vector>
using namespace std;

namespace Reality
{
    class Entity;
    class RenderList;
    typedef struct _GUIElementData
    {
        Vector2 Dimensions;
        bool IsVisible;
        i32 ListPos;
    } GUIElementData;
    
    class GUIManager
    {
        public:
            GUIManager();
            ~GUIManager();
        
            void OnMouseLeftDown(const Vector2& Pos);
            void OnMouseLeftUp(const Vector2& Pos);
            void OnMouseRightDown(const Vector2& Pos);
            void OnMouseRightUp(const Vector2& Pos);
        
            void AddEntity(Entity* E);
            void RemoveEntity(Entity* E);
            void GetVisibleEntities(RenderList* List);
        
        protected:
            vector<Entity*> m_Entities;
    };
};