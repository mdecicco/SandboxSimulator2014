#include <System/OS/PlatformIncludes.h>
#include <System/DebugSingleton.h>
#include <System/InputManager.h>
#include <System/OS/GlfwBackend.h>

#ifdef SS_USE_GLFW_BACKEND
namespace SSEngine {

/*
 *Glfw Window
 */
BackendWindow::BackendWindow(WindowInitializer &Initializer)
{
    m_Initializer = Initializer;
}

BackendWindow::~BackendWindow()
{

}

void BackendWindow::Create()
{
    //I can has glfw?
    if(!glfwInit())
    {
        glfwTerminate();
        SS_ERROR("Glfw could not initialize!\n");
    }
    //Gl context version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_Initializer.MajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_Initializer.MinorVersion);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    //Set bits
    glfwWindowHint(GLFW_RED_BITS,       m_Initializer.rgbaBits[0]);
    glfwWindowHint(GLFW_GREEN_BITS,     m_Initializer.rgbaBits[1]);
    glfwWindowHint(GLFW_BLUE_BITS,      m_Initializer.rgbaBits[2]);
    glfwWindowHint(GLFW_ALPHA_BITS,     m_Initializer.rgbaBits[3]);
    glfwWindowHint(GLFW_DEPTH_BITS,     m_Initializer.DepthBits);
    glfwWindowHint(GLFW_STENCIL_BITS,   m_Initializer.StencilBits);

    //Samples
    glfwWindowHint(GLFW_SAMPLES,        m_Initializer.SamplesCount);

    //Resizable?
    glfwWindowHint(GLFW_RESIZABLE,      m_Initializer.Resizable);

    //FullScreen?
    if(!m_Initializer.FullScreen)
        m_Window = glfwCreateWindow(m_Initializer.Width, m_Initializer.Height, m_Initializer.Title, NULL, NULL);
    else
        m_Window = glfwCreateWindow(m_Initializer.Width, m_Initializer.Height, m_Initializer.Title, glfwGetPrimaryMonitor(), NULL);

    //All is good?
    if(!m_Window)
    {
        glfwTerminate();
        SS_ERROR("Creating the glfw window failed!\n");
    }

    // GLEW
    MakeCurrent();
    //glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        SS_LOG("Glew initialization failed: %s\n", glewGetErrorString(err));
    }
    glGetError();
	glfwSwapInterval(0);
}

void BackendWindow::Destroy()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

bool BackendWindow::GetCloseRequested()
{
    return glfwWindowShouldClose(m_Window);
}

void BackendWindow::MakeCurrent()
{
    glfwMakeContextCurrent(m_Window);
}

Vec2 BackendWindow::GetWindowSize()
{
    int width, height;
    glfwGetWindowSize(m_Window, &width, &height);
    return Vec2(width, height);
}

void BackendWindow::PollEvents()
{
    glfwPollEvents();
}

void BackendWindow::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

f64 GetTimeInMS()
{
    return glfwGetTime()*1000.0;
}

f64 GetTimeInSeconds()
{
    return glfwGetTime();
}

/*
 * Input
 */
struct GLFWKeyCode2BC
{
    u32 x;
    KeyCode ak;
};

static const GLFWKeyCode2BC g2b[] = {
    {GLFW_KEY_ENTER,        SS_KEY_RETURN       },
    {GLFW_KEY_ESCAPE,       SS_KEY_ESCAPE       },
    {GLFW_KEY_BACKSPACE,    SS_KEY_BACKSPACE    },
    {GLFW_KEY_TAB,          SS_KEY_TAB          },
    {GLFW_KEY_SPACE,        SS_KEY_SPACE        },
    {GLFW_KEY_UNKNOWN,      SS_KEY_EXCLAIM      },
    {GLFW_KEY_APOSTROPHE,   SS_KEY_QUOTEDBL     },
    {GLFW_KEY_UNKNOWN,      SS_KEY_HASH         },
    {GLFW_KEY_UNKNOWN,      SS_KEY_PERCENT      },
    {GLFW_KEY_UNKNOWN,      SS_KEY_DOLLAR       },
    {GLFW_KEY_UNKNOWN,      SS_KEY_AMPERSAND    },
    {GLFW_KEY_UNKNOWN,      SS_KEY_QUOTE        },
    {GLFW_KEY_UNKNOWN,      SS_KEY_LPAREN       },
    {GLFW_KEY_UNKNOWN,      SS_KEY_RPAREN       },
    {GLFW_KEY_UNKNOWN,      SS_KEY_ASTERISK     },
    {GLFW_KEY_UNKNOWN,      SS_KEY_PLUS         },
    {GLFW_KEY_COMMA,        SS_KEY_COMMA        },
    {GLFW_KEY_MINUS,        SS_KEY_MINUS        },
    {GLFW_KEY_PERIOD,       SS_KEY_PERIOD       },
    {GLFW_KEY_SLASH,        SS_KEY_SLASH        },
    {GLFW_KEY_0,            SS_KEY_0            },
    {GLFW_KEY_1,            SS_KEY_1            },
    {GLFW_KEY_2,            SS_KEY_2            },
    {GLFW_KEY_3,            SS_KEY_3            },
    {GLFW_KEY_4,            SS_KEY_4            },
    {GLFW_KEY_5,            SS_KEY_5            },
    {GLFW_KEY_6,            SS_KEY_6            },
    {GLFW_KEY_7,            SS_KEY_7            },
    {GLFW_KEY_8,            SS_KEY_8            },
    {GLFW_KEY_9,            SS_KEY_9            },
    {GLFW_KEY_UNKNOWN,      SS_KEY_COLON        },
    {GLFW_KEY_SEMICOLON,    SS_KEY_SEMICOLON    },
    {GLFW_KEY_UNKNOWN,      SS_KEY_LESS         },
    {GLFW_KEY_EQUAL,        SS_KEY_EQUALS       },
    {GLFW_KEY_UNKNOWN,      SS_KEY_GREATER      },
    {GLFW_KEY_UNKNOWN,      SS_KEY_QUESTION     },
    {GLFW_KEY_UNKNOWN,      SS_KEY_AT           },
    {GLFW_KEY_LEFT_BRACKET, SS_KEY_LBRACKET     },
    {GLFW_KEY_BACKSLASH,    SS_KEY_BACKSLASH    },
    {GLFW_KEY_RIGHT_BRACKET,SS_KEY_RBRACKET     },
    //{XK_caret, BC_KEY_CARET,
    {GLFW_KEY_UNKNOWN,      SS_KEY_UNDERSCORE   },
    {GLFW_KEY_GRAVE_ACCENT, SS_KEY_BACKQUOTE    },
    {GLFW_KEY_A,            SS_KEY_A            },
    {GLFW_KEY_B,            SS_KEY_B            },
    {GLFW_KEY_C,            SS_KEY_C            },
    {GLFW_KEY_D,            SS_KEY_D            },
    {GLFW_KEY_E,            SS_KEY_E            },
    {GLFW_KEY_F,            SS_KEY_F            },
    {GLFW_KEY_G,            SS_KEY_G            },
    {GLFW_KEY_H,            SS_KEY_H            },
    {GLFW_KEY_I,            SS_KEY_I            },
    {GLFW_KEY_J,            SS_KEY_J            },
    {GLFW_KEY_K,            SS_KEY_K            },
    {GLFW_KEY_L,            SS_KEY_L            },
    {GLFW_KEY_M,            SS_KEY_M            },
    {GLFW_KEY_N,            SS_KEY_N            },
    {GLFW_KEY_O,            SS_KEY_O            },
    {GLFW_KEY_P,            SS_KEY_P            },
    {GLFW_KEY_Q,            SS_KEY_Q            },
    {GLFW_KEY_R,            SS_KEY_R            },
    {GLFW_KEY_S,            SS_KEY_S            },
    {GLFW_KEY_T,            SS_KEY_T            },
    {GLFW_KEY_U,            SS_KEY_U            },
    {GLFW_KEY_V,            SS_KEY_V            },
    {GLFW_KEY_W,            SS_KEY_W            },
    {GLFW_KEY_X,            SS_KEY_X            },
    {GLFW_KEY_Y,            SS_KEY_Y            },
    {GLFW_KEY_Z,            SS_KEY_Z            },
    {GLFW_KEY_DELETE,       SS_KEY_DELETE       },

    {GLFW_KEY_F1,           SS_KEY_F1           },
    {GLFW_KEY_F2,           SS_KEY_F2           },
    {GLFW_KEY_F3,           SS_KEY_F3           },
    {GLFW_KEY_F4,           SS_KEY_F4           },
    {GLFW_KEY_F5,           SS_KEY_F5           },
    {GLFW_KEY_F6,           SS_KEY_F6           },
    {GLFW_KEY_F7,           SS_KEY_F7           },
    {GLFW_KEY_F8,           SS_KEY_F8           },
    {GLFW_KEY_F9,           SS_KEY_F9           },
    {GLFW_KEY_F10,          SS_KEY_F10          },
    {GLFW_KEY_F11,          SS_KEY_F11          },
    {GLFW_KEY_F12,          SS_KEY_F12          },

    {GLFW_KEY_UP,           SS_KEY_UP           },
    {GLFW_KEY_DOWN,         SS_KEY_DOWN         },
    {GLFW_KEY_LEFT,         SS_KEY_LEFT         },
    {GLFW_KEY_RIGHT,        SS_KEY_RIGHT        }
};

#define GKEYCODE2BC(k_) InputMgr->Impl->KeyToBC[k_ & 0xFF]

void KeyCallback(GLFWwindow* Window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        InputMgr->Keys[GKEYCODE2BC(key)] = 1;
        InputMgr->OnKeyDown(GKEYCODE2BC(key), SS_PRESSED);
    }

    if(action == GLFW_RELEASE)
    {
        InputMgr->Keys[GKEYCODE2BC(key)] = 0;
        InputMgr->OnKeyDown(GKEYCODE2BC(key), SS_RELEASED);
    }

    if(action == GLFW_REPEAT)
    {
        InputMgr->OnKeyDown(GKEYCODE2BC(key), SS_HELD);
    }
}

void MouseMoveCallback(GLFWwindow* Window, double x, double y)
{
    InputMgr->MousePosition = Vec2(x, y);
    InputMgr->OnMouseMove(x,y);
}

void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods)
{
    //TODO
    //printf("button %d action %d", Button, Action);
    Action_Type Type;
    Mouse_Button Butt;
    if (Action == GLFW_PRESS) Type = SS_PRESSED;
    if (Action == GLFW_RELEASE) Type = SS_RELEASED;
    if (Action == GLFW_REPEAT) Type = SS_HELD;
    if (Button == GLFW_MOUSE_BUTTON_1) Butt = MOUSE_BUTTON_1;
    if (Button == GLFW_MOUSE_BUTTON_2) Butt = MOUSE_BUTTON_2;
    if (Button == GLFW_MOUSE_BUTTON_3) Butt = MOUSE_BUTTON_3;
    if (Button == GLFW_MOUSE_BUTTON_4) Butt = MOUSE_BUTTON_4;

    InputMgr->OnMouseButton(Butt, Type);
}

InputManager::~InputManager()
{
    delete Impl;
}

void InputManager::PollEvents()
{

}

void InputManager::Init(SSWindow* Win)
{
    m_Window = Win;
    BackendWindow* nwin = m_Window->GetBackendWindow();

    glfwSetKeyCallback(nwin->GetWindow(), KeyCallback);
    glfwSetCursorPosCallback(nwin->GetWindow(), MouseMoveCallback);
    glfwSetMouseButtonCallback(nwin->GetWindow(), MouseButtonCallback);

    // Init native
    Impl = new InputImpl;

    memset(&Impl->KeyToBC[0], sizeof(Impl->KeyToBC), SS_KEY_UNKNOWN);
    for(i32 i = 0; i < ARRAY_SIZE_IN_ELEMENTS(g2b); i++)
    {
        GLFWKeyCode2BC a = g2b[i];
        u32 somethingElse = a.x & 0xFF;

        Impl->KeyToBC[somethingElse] = a.ak;
    }

    Reset();
}

void InputManager::SetMousePosition(const Vec2& pos)
{
    //BC_LOG("Moving to: %f x %f", pos.x, pos.y);

    if(pos != MousePosition)
    {
        BackendWindow* nwin = m_Window->GetBackendWindow();

        glfwSetCursorPos(nwin->GetWindow(), pos.x, pos.y);
    }
}

void InputManager::HideCursor(bool Hide)
{
    if(Hide)
        glfwSetInputMode(m_Window->GetBackendWindow()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    else
        glfwSetInputMode(m_Window->GetBackendWindow()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

}
#endif
