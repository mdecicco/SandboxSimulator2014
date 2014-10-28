#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <System/SSTypes.h>
#include <Utils/Math.h>
#include <System/OS/PlatformIncludes.h>
#include <vector>
#include <map>

#include <functional>

/*
 * TODO: Add callbacks with pressed/release action similair to default glfw callbacks for gui and other things that need it.
 */

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define InputMgr InputManager::GetInputManager()

namespace SSEngine {
enum KeyCode
{
    SS_KEY_UNKNOWN = 0,

    SS_KEY_RETURN = '\r',
    SS_KEY_ESCAPE = '\033',
    SS_KEY_BACKSPACE = '\b',
    SS_KEY_TAB = '\t',
    SS_KEY_SPACE = ' ',
    SS_KEY_EXCLAIM = '!',
    SS_KEY_QUOTEDBL = '"',
    SS_KEY_HASH = '#',
    SS_KEY_PERCENT = '%',
    SS_KEY_DOLLAR = '$',
    SS_KEY_AMPERSAND = '&',
    SS_KEY_QUOTE = '\'',
    SS_KEY_LPAREN = '(',
    SS_KEY_RPAREN = ')',
    SS_KEY_ASTERISK = '*',
    SS_KEY_PLUS = '+',
    SS_KEY_COMMA = ',',
    SS_KEY_MINUS = '-',
    SS_KEY_PERIOD = '.',
    SS_KEY_SLASH = '/',
    SS_KEY_0 = '0',
    SS_KEY_1 = '1',
    SS_KEY_2 = '2',
    SS_KEY_3 = '3',
    SS_KEY_4 = '4',
    SS_KEY_5 = '5',
    SS_KEY_6 = '6',
    SS_KEY_7 = '7',
    SS_KEY_8 = '8',
    SS_KEY_9 = '9',
    SS_KEY_COLON = ':',
    SS_KEY_SEMICOLON = ';',
    SS_KEY_LESS = '<',
    SS_KEY_EQUALS = '=',
    SS_KEY_GREATER = '>',
    SS_KEY_QUESTION = '?',
    SS_KEY_AT = '@',
    SS_KEY_LBRACKET = '[',
    SS_KEY_BACKSLASH = '\\',
    SS_KEY_RBRACKET = ']',
    SS_KEY_CARET = '^',
    SS_KEY_UNDERSCORE = '_',
    SS_KEY_BACKQUOTE = '`',

    SS_KEY_A = 'A',
    SS_KEY_B = 'B',
    SS_KEY_C = 'C',
    SS_KEY_D = 'D',
    SS_KEY_E = 'E',
    SS_KEY_F = 'F',
    SS_KEY_G = 'G',
    SS_KEY_H = 'H',
    SS_KEY_I = 'I',
    SS_KEY_J = 'J',
    SS_KEY_K = 'K',
    SS_KEY_L = 'L',
    SS_KEY_M = 'M',
    SS_KEY_N = 'N',
    SS_KEY_O = 'O',
    SS_KEY_P = 'P',
    SS_KEY_Q = 'Q',
    SS_KEY_R = 'R',
    SS_KEY_S = 'S',
    SS_KEY_T = 'T',
    SS_KEY_U = 'U',
    SS_KEY_V = 'V',
    SS_KEY_W = 'W',
    SS_KEY_X = 'X',
    SS_KEY_Y = 'Y',
    SS_KEY_Z = 'Z',

    SS_KEY_a = 'a',
    SS_KEY_b = 'b',
    SS_KEY_c = 'c',
    SS_KEY_d = 'd',
    SS_KEY_e = 'e',
    SS_KEY_f = 'f',
    SS_KEY_g = 'g',
    SS_KEY_h = 'h',
    SS_KEY_i = 'i',
    SS_KEY_j = 'j',
    SS_KEY_k = 'k',
    SS_KEY_l = 'l',
    SS_KEY_m = 'm',
    SS_KEY_n = 'n',
    SS_KEY_o = 'o',
    SS_KEY_p = 'p',
    SS_KEY_q = 'q',
    SS_KEY_r = 'r',
    SS_KEY_s = 's',
    SS_KEY_t = 't',
    SS_KEY_u = 'u',
    SS_KEY_v = 'v',
    SS_KEY_w = 'w',
    SS_KEY_x = 'x',
    SS_KEY_y = 'y',
    SS_KEY_z = 'z',
    SS_KEY_LSHIFT,

    SS_KEY_DELETE = '\177',
    SS_KEY_INSERT,
    SS_KEY_HOME,
    SS_KEY_END,
    SS_KEY_PAGEUP,
    SS_KEY_PAGEDOWN,

    SS_KEY_UP,
    SS_KEY_DOWN,
    SS_KEY_LEFT,
    SS_KEY_RIGHT,

    SS_KEY_F1,
    SS_KEY_F2,
    SS_KEY_F3,
    SS_KEY_F4,
    SS_KEY_F5,
    SS_KEY_F6,
    SS_KEY_F7,
    SS_KEY_F8,
    SS_KEY_F9,
    SS_KEY_F10,
    SS_KEY_F11,
    SS_KEY_F12,

    SS_KEY_COUNT
};

enum Mouse_Button
{
    MOUSE_BUTTON_1 = 0,
    MOUSE_BUTTON_2 = 1,
    MOUSE_BUTTON_3 = 2,
    MOUSE_BUTTON_4 = 3,
    MOUSE_BUTTON_5 = 4,
    MOUSE_BUTTON_6 = 5,
    MOUSE_BUTTON_7 = 6,
    MOUSE_BUTTON_8 = 7,
    MOUSE_BUTTON_COUNT
};

enum Action_Type
{
    SS_RELEASED,
    SS_PRESSED,
    SS_HELD,
    EVENT_TYPE_COUNT
};

typedef void (*KeyCB)(char,Action_Type);
typedef void (*MouseMoveCB)(double,double);
typedef void (*MouseButtonCB)(Mouse_Button,Action_Type);
typedef std::vector<KeyCB> Keycallbacks;
typedef std::vector<MouseMoveCB> MouseMoveCallbacks;
typedef std::vector<MouseButtonCB> MouseButtonCallbacks;


typedef std::function<void(char, Action_Type)> KeyFunc;
typedef std::function<void(f64, f64)> MouseMoveFunc;
typedef std::function<void(Mouse_Button, Action_Type)> MouseButtonFunc;
typedef std::vector<KeyFunc> KeyFuncList;
typedef std::vector<MouseMoveFunc> MouseMoveFuncList;
typedef std::vector<MouseButtonFunc> MouseButtonFuncList;

class InputManager
{
public:
    enum Win_Event
    {
        WINDOW_FOCUS_LOST,
        WINDOW_FOCUS_GAINED,
        WINDOW_CLOSED_EVENT,
        EVENTS_COUNT
    };

public:
    u32                     Keys[SS_KEY_COUNT];
    u32                     MouseButtons[MOUSE_BUTTON_COUNT];
    Vec2                    MousePosition;
    InputImpl*              Impl;


private:
    SSWindow*               m_Window;
    u8                      Events[EVENTS_COUNT];
    bool                    ShowCursor;

    //Callback lists
    Keycallbacks            m_KeyCBs;
    MouseMoveCallbacks      m_MouseMoveCBs;
    MouseButtonCallbacks    m_MouseButtonCBs;
    KeyFuncList                m_KeyFunctions;
    MouseMoveFuncList        m_MouseMoveFunctions;
    MouseButtonFuncList        m_MouseButtonFunctions;

	InputManager() { Reset(); }

public:
    ~InputManager();

	static InputManager* GetInputManager() {
		static InputManager* inp = new InputManager();
		return inp;
	}

    u32 GetKey(u32 i) const
    {
        return Keys[i];
    }

    u32 GetMouseButton(u32 i) const
    {
        return MouseButtons[i];
    }

    const Vec2& GetMousePosition() const
    {
        return MousePosition;
    }

    u32 GetEvent(Win_Event Event) const
    {
        return Events[Event];
    }

    void Init(SSWindow* Win);
    void Reset();
    void PollEvents();
    void SetMousePosition(const Vec2& Pos);
    void HideCursor(bool Hide);

    //Callbacks
    void AddKeyDownCB(KeyCB CB);
    void AddMouseMoveCB(MouseMoveCB CB);
    void AddMouseButtonCB(MouseButtonCB CB);

    void RemoveKeyDownCB(KeyCB CB);
    void RemoveMouseMoveCB(MouseMoveCB CB);
    void RemoveMouseButtonCB(MouseButtonCB CB);

    //Functions
    void AddKeyDownFunction(KeyFunc KF) { m_KeyFunctions.push_back(KF); }
    void AddMouseMoveFunction(MouseMoveFunc MMF) { m_MouseMoveFunctions.push_back(MMF); }
    void AddMouseButtonFunction(MouseButtonFunc MBF) { m_MouseButtonFunctions.push_back(MBF); }

    void RemoveKeyDownFunctions();
    void RemoveMouseMoveFunctions();
    void RemoveMouseButtconFunctions();

    //lower level calls, calls the cbs and funcs
    void OnMouseMove(double x, double y);
    void OnKeyDown(char Key, Action_Type Action);
    void OnMouseButton(Mouse_Button Button, Action_Type Action);



    void ClearCallbacks()
    {
        m_KeyCBs.clear();
        m_MouseMoveCBs.clear();
        m_MouseButtonCBs.clear();
    }

    void ClearFunctions()
    {
        m_KeyFunctions.clear();
        m_MouseMoveFunctions.clear();
        m_MouseButtonFunctions.clear();
    }

    //

    void AddEvent(Win_Event Event)
    {
        ++Events[Event];
    }
};
}

#endif
