#ifndef SandboxSimulator_Gui_h
#define SandboxSimulator_Gui_h

#include <Utils/Math.h>
#include <vector>
#include <Core/SceneGraph.h>
#include <Rendering/Gui/Font.h>
#include <Rendering/Shader.h>

namespace SandboxSimulator {
class Entity;
class RenderList;
class RenderComponent;
class GuiElement;
class GUIManager;



enum GUI_TEXTURES
{
    GUI_FONT_TEXTURE = SU_TEXTURE0,
    GUI_TEXTURE_COUNT
};

#define ElementPtr std::unique_ptr<GuiElement>
class GuiElement
{
    public:
        GuiElement(RenderComponent* rend) : m_Component(rend), m_Size(Vec2(0,0)), m_NeedsUpdate(true)
        {}

        virtual void OnKeyDown(i32 Key) {};
        virtual void OnKeyUp(i32 Key) {};
        virtual void OnMouseDown(i32 Button, Vec2 Pos) {};
        virtual void OnMouseUp(i32 Button, Vec2 Pos) {};
        virtual void OnMouseOver(Vec2 Pos) {};
        virtual void OnMouseOut(Vec2 Pos) {};

        virtual void GenerateMesh();

        Entity* GetOwner();
        Vec2 GetPosition();

        Vec2 GetSize() { return m_Size; }
        void SetSize(Vec2 Size) { m_Size = Size; }



    protected:
        friend class GuiElement;
        friend class GUIManager;
        friend class Font;

        RenderComponent* m_Component;
        std::vector<ElementPtr> m_Children;
        GuiElement* m_Parent;
        Vec2 m_Size;

        bool m_NeedsUpdate;
};

class GUIManager
{
    public:
        GUIManager(Font* F);
        ~GUIManager();
    
        virtual void OnKeyDown(i32 Key) {};
        virtual void OnKeyUp(i32 Key) {};
        virtual void OnMouseDown(i32 Button, Vec2 Pos);
        virtual void OnMouseUp(i32 Button, Vec2 Pos);

        GuiElement* MakeElement(RenderComponent* r);
        void AddElement(GuiElement* Ele);
        void RemoveElement(GuiElement* E);
        void GetVisibleEntities(RenderList* List);
    
    protected:
        std::vector<ElementPtr> m_Elements;
        Font* m_Font;
};
}

#endif