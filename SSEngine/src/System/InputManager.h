#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <System/BcTypes.h>
#include <Utils/Math.h>
#include <System/OS/PlatformIncludes.h>
#include <vector>
#include <map>

#include <functional>

/*
 * TODO: Add callbacks with pressed/release action similair to default glfw callbacks for gui and other things that need it.
 */

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

namespace BearClaw {
enum KeyCode
{
    BC_KEY_UNKNOWN = 0,

    BC_KEY_RETURN = '\r',
    BC_KEY_ESCAPE = '\033',
    BC_KEY_BACKSPACE = '\b',
    BC_KEY_TAB = '\t',
    BC_KEY_SPACE = ' ',
    BC_KEY_EXCLAIM = '!',
    BC_KEY_QUOTEDBL = '"',
    BC_KEY_HASH = '#',
    BC_KEY_PERCENT = '%',
    BC_KEY_DOLLAR = '$',
    BC_KEY_AMPERSAND = '&',
    BC_KEY_QUOTE = '\'',
    BC_KEY_LPAREN = '(',
    BC_KEY_RPAREN = ')',
    BC_KEY_ASTERISK = '*',
    BC_KEY_PLUS = '+',
    BC_KEY_COMMA = ',',
    BC_KEY_MINUS = '-',
    BC_KEY_PERIOD = '.',
    BC_KEY_SLASH = '/',
    BC_KEY_0 = '0',
    BC_KEY_1 = '1',
    BC_KEY_2 = '2',
    BC_KEY_3 = '3',
    BC_KEY_4 = '4',
    BC_KEY_5 = '5',
    BC_KEY_6 = '6',
    BC_KEY_7 = '7',
    BC_KEY_8 = '8',
    BC_KEY_9 = '9',
    BC_KEY_COLON = ':',
    BC_KEY_SEMICOLON = ';',
    BC_KEY_LESS = '<',
    BC_KEY_EQUALS = '=',
    BC_KEY_GREATER = '>',
    BC_KEY_QUESTION = '?',
    BC_KEY_AT = '@',
    BC_KEY_LBRACKET = '[',
    BC_KEY_BACKSLASH = '\\',
    BC_KEY_RBRACKET = ']',
    BC_KEY_CARET = '^',
    BC_KEY_UNDERSCORE = '_',
    BC_KEY_BACKQUOTE = '`',

    BC_KEY_A = 'A',
    BC_KEY_B = 'B',
    BC_KEY_C = 'C',
    BC_KEY_D = 'D',
    BC_KEY_E = 'E',
    BC_KEY_F = 'F',
    BC_KEY_G = 'G',
    BC_KEY_H = 'H',
    BC_KEY_I = 'I',
    BC_KEY_J = 'J',
    BC_KEY_K = 'K',
    BC_KEY_L = 'L',
    BC_KEY_M = 'M',
    BC_KEY_N = 'N',
    BC_KEY_O = 'O',
    BC_KEY_P = 'P',
    BC_KEY_Q = 'Q',
    BC_KEY_R = 'R',
    BC_KEY_S = 'S',
    BC_KEY_T = 'T',
    BC_KEY_U = 'U',
    BC_KEY_V = 'V',
    BC_KEY_W = 'W',
    BC_KEY_X = 'X',
    BC_KEY_Y = 'Y',
    BC_KEY_Z = 'Z',

    BC_KEY_a = 'a',
    BC_KEY_b = 'b',
    BC_KEY_c = 'c',
    BC_KEY_d = 'd',
    BC_KEY_e = 'e',
    BC_KEY_f = 'f',
    BC_KEY_g = 'g',
    BC_KEY_h = 'h',
    BC_KEY_i = 'i',
    BC_KEY_j = 'j',
    BC_KEY_k = 'k',
    BC_KEY_l = 'l',
    BC_KEY_m = 'm',
    BC_KEY_n = 'n',
    BC_KEY_o = 'o',
    BC_KEY_p = 'p',
    BC_KEY_q = 'q',
    BC_KEY_r = 'r',
    BC_KEY_s = 's',
    BC_KEY_t = 't',
    BC_KEY_u = 'u',
    BC_KEY_v = 'v',
    BC_KEY_w = 'w',
    BC_KEY_x = 'x',
    BC_KEY_y = 'y',
    BC_KEY_z = 'z',
    BC_KEY_LSHIFT,

    BC_KEY_DELETE = '\177',
    BC_KEY_INSERT,
    BC_KEY_HOME,
    BC_KEY_END,
    BC_KEY_PAGEUP,
    BC_KEY_PAGEDOWN,

    BC_KEY_UP,
    BC_KEY_DOWN,
    BC_KEY_LEFT,
    BC_KEY_RIGHT,

    BC_KEY_F1,
    BC_KEY_F2,
    BC_KEY_F3,
    BC_KEY_F4,
    BC_KEY_F5,
    BC_KEY_F6,
    BC_KEY_F7,
    BC_KEY_F8,
    BC_KEY_F9,
    BC_KEY_F10,
    BC_KEY_F11,
    BC_KEY_F12,

    BC_KEY_COUNT
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
    BC_RELEASED,
    BC_PRESSED,
    BC_HELD,
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
    u32                     Keys[BC_KEY_COUNT];
    u32                     MouseButtons[MOUSE_BUTTON_COUNT];
    Vec2                    MousePosition;
    InputImpl*              Impl;


private:
    BcWindow*               m_Window;
    u8                      Events[EVENTS_COUNT];
    bool                    ShowCursor;

    //Callback lists
    Keycallbacks            m_KeyCBs;
    MouseMoveCallbacks      m_MouseMoveCBs;
    MouseButtonCallbacks    m_MouseButtonCBs;
	KeyFuncList				m_KeyFunctions;
	MouseMoveFuncList		m_MouseMoveFunctions;
	MouseButtonFuncList		m_MouseButtonFunctions;

public:
    InputManager() { Reset(); }
    ~InputManager();

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

    void Init(BcWindow* Win);
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
