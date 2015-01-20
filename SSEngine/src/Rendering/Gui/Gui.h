#ifndef SandboxSimulator_Gui_h
#define SandboxSimulator_Gui_h

#include <Utils/Math.h>
#include <vector>

namespace SandboxSimulator {
class Entity;
class RenderList;
typedef struct _GUIElementData
{
    Vec2 Dimensions;
    bool IsVisible;
    i32 ListPos;
} GUIElementData;

class GUIManager
{
    public:
        GUIManager();
        ~GUIManager();
    
        void OnMouseLeftDown(const Vec2& Pos);
        void OnMouseLeftUp(const Vec2& Pos);
        void OnMouseRightDown(const Vec2& Pos);
        void OnMouseRightUp(const Vec2& Pos);
    
        void AddEntity(Entity* E);
        void RemoveEntity(Entity* E);
        void GetVisibleEntities(RenderList* List);
    
    protected:
        vector<Entity*> m_Entities;
};
}

#endif