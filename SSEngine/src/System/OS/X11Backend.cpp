#ifdef BC_USE_X11_BACKEND

#include <System/OS/PlatformIncludes.h>
#include <System/InputManager.h>
#include <System/Environment.h>
#include <System/OS/Window.h>
#include <X11/XKBlib.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <string>

namespace BearClaw {

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*,
    GLXFBConfig, GLXContext, Bool, const int*);

static struct timeval gstart;

BackendWindow::BackendWindow(WindowInitializer &Initializer)
{
    m_Initializer = Initializer;
}

void BackendWindow::CreateDisplay()
{
    m_Display = XOpenDisplay(0);
    if(!m_Display)
    {
        //TODO: Error
    }
}

void BackendWindow::CreateConfig()
{
    int attribs[256];
    uint_fast32_t at = 0;

    attribs[at++] = GLX_X_RENDERABLE;
    attribs[at++] = True;

    attribs[at++] = GLX_DRAWABLE_TYPE;
    attribs[at++] = GLX_WINDOW_BIT;

    attribs[at++] = GLX_RENDER_TYPE;
    attribs[at++] = GLX_RGBA_BIT;

    attribs[at++] = GLX_X_VISUAL_TYPE;
    attribs[at++] = GLX_TRUE_COLOR;

    attribs[at++] = GLX_RED_SIZE;
    attribs[at++] = m_Initializer.rgbaBits[0];

    attribs[at++] = GLX_GREEN_SIZE;
    attribs[at++] = m_Initializer.rgbaBits[1];

    attribs[at++] = GLX_GREEN_SIZE;
    attribs[at++] = m_Initializer.rgbaBits[2];

    attribs[at++] = GLX_ALPHA_SIZE;
    attribs[at++] = m_Initializer.rgbaBits[3];

    attribs[at++] = GLX_DEPTH_SIZE;
    attribs[at++] = m_Initializer.DepthBits;

    attribs[at++] = GLX_STENCIL_SIZE;
    attribs[at++] = m_Initializer.StencilBits;

    attribs[at++] = GLX_DOUBLEBUFFER;
    attribs[at++] = true;

    if(m_Initializer.SamplesCount)
    {
        attribs[at++] = GLX_SAMPLE_BUFFERS;
        attribs[at++] = 1;

        attribs[at++] = GLX_SAMPLES;
        attribs[at++] = m_Initializer.SamplesCount;
    }

    attribs[at++] = None;

    int fbcount;
    glxConfig = glXChooseFBConfig(m_Display, DefaultScreen(m_Display),
        &attribs[0], &fbcount);

    if(!glxConfig)
    {
        BC_LOG("Could not set up x configuration\n");
    }
}

void BackendWindow::CreateNativeWindow()
{
    XVisualInfo*            vi;
    XSetWindowAttributes    swa;
    Window                  root;

    /*if(m_Initializer.FullScreen)
    {
        Screen* pscr;
        pscr                    = DefaultScreenOfDisplay(m_Display);

        m_Initializer.Width     = pscr->width;
        m_Initializer.Height    = pscr->height;
    }*/

    vi = glXGetVisualFromFBConfig(m_Display, glxConfig[0]);

    if(!vi)
    {
        BC_LOG("Vi creation failed\n");
    }

    root = RootWindow(m_Display, vi->screen);

    //Colormap
    swa.colormap = m_Colormap   = XCreateColormap(m_Display, root, vi->visual, AllocNone);
    swa.background_pixmap       = None;
    swa.border_pixel            = 0;
    swa.event_mask              = StructureNotifyMask;
    //

    m_Window = XCreateWindow(m_Display, root, 0, 0, m_Initializer.Width, m_Initializer.Height, 0,
                             vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
    if(!m_Window)
    {
        XFree(vi);
        BC_LOG("Window creation failed");
    }


    //WM protocols
    int count = 0;
    Atom protocols[1];
    Atom SafeDelete = XInternAtom(m_Display,"WM_DELETE_WINDOW", False);
    if (SafeDelete != None) protocols[count++] = SafeDelete;

    Atom PingCheck = XInternAtom(m_Display, "_NET_WM_PING", True);
    //if (PingCheck != None) protocols[count++] = PingCheck;

    if (count > 0)
    {
        //XSetWMProtocols(m_Display, m_Window, protocols, count);
    }
    //

    //Hints for resizing
    XSizeHints* hints = XAllocSizeHints();
    hints->flags = 0;

    if (!m_Initializer.Resizable)
    {
        hints->flags |= (PMinSize | PMaxSize);
        hints->min_width  = hints->max_width  = m_Initializer.Width;
        hints->min_height = hints->max_height = m_Initializer.Height;
    }

    XSetWMNormalHints(m_Display, m_Window, hints);
    XFree(hints);
    //

    XStoreName(m_Display, m_Window, m_Initializer.Title);
    XMapWindow(m_Display, m_Window);

    if(m_Initializer.FullScreen)
    {
        XEvent xev;
        memset(&xev, 0, sizeof(xev));

        Atom wmState = XInternAtom(m_Display, "_NET_WM_STATE", False);
        Atom fullscreen = XInternAtom(m_Display, "_NET_WM_STATE_FULLSCREEN", False);

        xev.type = ClientMessage;
        xev.xclient.window = m_Window;
        xev.xclient.message_type = wmState;
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = 1;
        xev.xclient.data.l[1] = fullscreen;
        xev.xclient.data.l[2] = 0;

        XSendEvent(m_Display, RootWindow(m_Display, vi->screen), 0,
            SubstructureNotifyMask | SubstructureRedirectMask, &xev);
        }

    XFree(vi);
}

void BackendWindow::CreateContext()
{
    int ctxAttribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, (int)m_Initializer.MajorVersion,
        GLX_CONTEXT_MINOR_VERSION_ARB, (int)m_Initializer.MinorVersion,
        None, None,
        None};

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;

    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
        glXGetProcAddressARB((const GLubyte *) "glXCreateContextAttribsARB");

    if(!glXCreateContextAttribsARB)
    {
        BC_LOG("Cannot get addres of proc glXCreateContextAttribsARB");
    }

    glxContext = glXCreateContextAttribsARB(m_Display, *glxConfig, 0,
        True, ctxAttribs);

    XSync(m_Display, False);

    if(!glxContext)
    {
        BC_LOG("Context creation failed");
    }
}

void BackendWindow::CreateInternal()
{
    // Display
    CreateDisplay();

    // GLX init
    int major, minor;
    if(!glXQueryVersion(m_Display, &major, &minor))
    {
        BC_LOG("glXQueryVersion() failed");
    }

    if(((major == 1) && (minor < 3)) || (major < 1))
    {
        BC_LOG("Old GLX version");
    }

    // Config
    CreateConfig();

    // Window
    CreateNativeWindow();

    // Context
    CreateContext();

    // GLEW
    MakeCurrent();
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        BC_LOG("Glew initialization failed: %s\n", glewGetErrorString(err));
    }
    glGetError();
}

void BackendWindow::Create()
{
    gettimeofday(&gstart, NULL);
    try
    {
        CreateInternal();
    }
    catch(const std::exception& e)
    {
        Destroy();
        BC_LOG("Window creation failed");
    }
}

void BackendWindow::Destroy()
{
    CloseRequested = true;

    if(glxContext)
    {
        glXMakeCurrent(m_Display, 0, 0);
        glXDestroyContext(m_Display, glxContext);
        //delete glxContext;
    }

    XUnmapWindow(m_Display, m_Window);

    if(m_Window)
    {
        XWithdrawWindow(m_Display, m_Window, 0);
        XDestroyWindow(m_Display, m_Window);
        m_Window = 0;
    }

    if(m_Colormap)
    {
        XFreeColormap(m_Display, m_Colormap);
    }

    if(m_Display)
    {
        XCloseDisplay(m_Display);
        //delete m_Display;
    }
}

void BackendWindow::SwapBuffers()
{
    glXSwapBuffers(m_Display, m_Window);
}

Vec2 BackendWindow::GetWindowSize()
{
    XWindowAttributes attribs;
    XGetWindowAttributes(m_Display, m_Window, &attribs);

    return Vec2(attribs.width, attribs.height);
}

bool BackendWindow::GetCloseRequested()
{
    return CloseRequested;
}

void BackendWindow::ProcessEvent(XEvent* Event)
{
    Atom SafeDelete = XInternAtom(m_Display,"WM_DELETE_WINDOW", False);
    Atom PingCheck = XInternAtom(m_Display, "_NET_WM_PING", True);
    //

    if ((Atom) Event->xclient.data.l[0] == SafeDelete)
    {
        CloseRequested = true;
    }
    else if (PingCheck != None && (Atom) Event->xclient.data.l[0] == PingCheck)
    {
        Event->xclient.window = m_Window;
        XSendEvent(m_Display, Event->xclient.window, False,
                   SubstructureNotifyMask | SubstructureRedirectMask,
                   Event);
    }
}

void BackendWindow::MakeCurrent()
{
    if(!glXMakeCurrent(m_Display, m_Window, glxContext))
    {
        BC_LOG("Error making context curren\n");
    }
}

/*
 * Timing
 */
f64 GetTimeInMS()
{
    f64 ticks;

    struct timeval now;
    gettimeofday(&now, NULL);
    ticks = (f32)(now.tv_sec - gstart.tv_sec) * (f32)1000
        + (f32)(now.tv_usec - gstart.tv_usec) / (f32)1000;

    return ticks;
}

f64 GetTimeInSeconds()
{
    (f32)GetTimeInMS() * 0.001;
}

/*
 * Input
 */
struct X11KeyCodeToBC
{
    u32 x;
    KeyCode ak;
};

static const X11KeyCodeToBC x2a[] = {
    {XK_Return, BC_KEY_RETURN},
    {XK_Escape, BC_KEY_ESCAPE},
    {XK_BackSpace, BC_KEY_BACKSPACE},
    {XK_Tab, BC_KEY_TAB},
    {XK_space, BC_KEY_SPACE},
    {XK_exclam, BC_KEY_EXCLAIM},
    {XK_quotedbl, BC_KEY_QUOTEDBL},
    {XK_numbersign, BC_KEY_HASH},
    {XK_percent, BC_KEY_PERCENT},
    {XK_dollar, BC_KEY_DOLLAR},
    {XK_ampersand, BC_KEY_AMPERSAND},
    {XK_apostrophe, BC_KEY_QUOTE},
    {XK_parenleft, BC_KEY_LPAREN},
    {XK_parenright, BC_KEY_RPAREN},
    {XK_asterisk, BC_KEY_ASTERISK},
    {XK_plus, BC_KEY_PLUS},
    {XK_comma, BC_KEY_COMMA},
    {XK_minus, BC_KEY_MINUS},
    {XK_period, BC_KEY_PERIOD},
    {XK_slash, BC_KEY_SLASH},
    {XK_0, BC_KEY_0},
    {XK_1, BC_KEY_1},
    {XK_2, BC_KEY_2},
    {XK_3, BC_KEY_3},
    {XK_4, BC_KEY_4},
    {XK_5, BC_KEY_5},
    {XK_6, BC_KEY_6},
    {XK_7, BC_KEY_7},
    {XK_8, BC_KEY_8},
    {XK_9, BC_KEY_9},
    {XK_colon, BC_KEY_COLON},
    {XK_semicolon, BC_KEY_SEMICOLON},
    {XK_less, BC_KEY_LESS},
    {XK_equal, BC_KEY_EQUALS},
    {XK_greater, BC_KEY_GREATER},
    {XK_question, BC_KEY_QUESTION},
    {XK_at, BC_KEY_AT},
    {XK_bracketleft, BC_KEY_LBRACKET},
    {XK_backslash, BC_KEY_BACKSLASH},
    {XK_bracketright, BC_KEY_RBRACKET},
    //{XK_caret, BC_KEY_CARET},
    {XK_underscore, BC_KEY_UNDERSCORE},
    {XK_grave, BC_KEY_BACKQUOTE},
    {XK_a, BC_KEY_A},
    {XK_b, BC_KEY_B},
    {XK_c, BC_KEY_C},
    {XK_d, BC_KEY_D},
    {XK_e, BC_KEY_E},
    {XK_f, BC_KEY_F},
    {XK_g, BC_KEY_G},
    {XK_h, BC_KEY_H},
    {XK_i, BC_KEY_I},
    {XK_j, BC_KEY_J},
    {XK_k, BC_KEY_K},
    {XK_l, BC_KEY_L},
    {XK_m, BC_KEY_M},
    {XK_n, BC_KEY_N},
    {XK_o, BC_KEY_O},
    {XK_p, BC_KEY_P},
    {XK_q, BC_KEY_Q},
    {XK_r, BC_KEY_R},
    {XK_s, BC_KEY_S},
    {XK_t, BC_KEY_T},
    {XK_u, BC_KEY_U},
    {XK_v, BC_KEY_V},
    {XK_w, BC_KEY_W},
    {XK_x, BC_KEY_X},
    {XK_y, BC_KEY_Y},
    {XK_z, BC_KEY_Z},
    {XK_Delete, BC_KEY_DELETE},

    {XK_F1, BC_KEY_F1},
    {XK_F2, BC_KEY_F2},
    {XK_F3, BC_KEY_F3},
    {XK_F4, BC_KEY_F4},
    {XK_F5, BC_KEY_F5},
    {XK_F6, BC_KEY_F6},
    {XK_F7, BC_KEY_F7},
    {XK_F8, BC_KEY_F8},
    {XK_F9, BC_KEY_F9},
    {XK_F10, BC_KEY_F10},
    {XK_F11, BC_KEY_F11},
    {XK_F12, BC_KEY_F12},

    {XK_Up,  BC_KEY_UP},
    {XK_Down, BC_KEY_DOWN},
    {XK_Left, BC_KEY_LEFT},
    {XK_Right, BC_KEY_RIGHT}
};

#define XKEYCODE2BC(k_) Impl->KeyToBC[k_ & 0xFF]

static bool EventsPending(Display* display)
{
    XFlush(display);
    if(XEventsQueued(display, QueuedAlready))
    {
        return true;
    }

    static struct timeval zero_time;
    int x11_fd;
    fd_set fdset;

    x11_fd = ConnectionNumber(display);
    FD_ZERO(&fdset);
    FD_SET(x11_fd, &fdset);
    if(select(x11_fd + 1, &fdset, NULL, NULL, &zero_time) == 1)
    {
        return XPending(display);
    }

    return false;
}

InputManager::~InputManager()
{}

void InputManager::Init(BcWindow* Win)
{
    m_Window = Win;
    BackendWindow* nwin = m_Window->GetBackendWindow();

    XSelectInput(nwin->m_Display, nwin->m_Window,
        ExposureMask | ButtonPressMask | KeyPressMask | KeyReleaseMask
        | PointerMotionMask | FocusChangeMask | EnterWindowMask
        | LeaveWindowMask);

    // Init native
    Impl = new InputImpl;

    memset(
        &Impl->KeyToBC[0], sizeof(Impl->KeyToBC), BC_KEY_UNKNOWN);
    for(i32 i = 0; i < ARRAY_SIZE_IN_ELEMENTS(x2a); i++)
    {
        X11KeyCodeToBC a = x2a[i];
        u32 somethingElse = a.x & 0xFF;

        Impl->KeyToBC[somethingElse] = a.ak;
    }

    Reset();
}

void BackendWindow::PollEvents()
{}

void InputManager::PollEvents()
{
    for(int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(Keys); i++)
    {
        u32 k = Keys[i];
        if(k)
        {
            ++k;
        }
    }
    for(int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(MouseButtons); i++)
    {
        u32 k = MouseButtons[i];
        if(k)
        {
            ++k;
        }
    }

    BackendWindow* win = m_Window->GetBackendWindow();
    Display* disp = win->m_Display;
    while(EventsPending(disp))
    {
        XEvent event;
        ::KeyCode keycode = 0;
        KeySym keysym;
        XNextEvent(disp, &event);
skipXNextEvent:

        switch(event.type)
        {
        case KeyPress:
            keysym = XLookupKeysym(&event.xkey, 0);
            keycode = event.xkey.keycode;
            Keys[XKEYCODE2BC(keysym)] = 1;
            OnKeyDown(XKEYCODE2BC(keysym), BC_PRESSED);
            break;
        case KeyRelease:
            keycode = event.xkey.keycode;
            keysym = XLookupKeysym(&event.xkey, 0);
            if(EventsPending(disp))
            {
                XEvent event1;
                XNextEvent(disp, &event1);

                if(event1.type == KeyPress && event1.xkey.keycode == keycode)
                {
                    // Repeat
                    //++keys[XKEYCODE2BC(keysym)];
                    OnKeyDown(XKEYCODE2BC(keysym), BC_HELD);
                }
                else
                {
                    OnKeyDown(XKEYCODE2BC(keysym), BC_RELEASED);
                    Keys[XKEYCODE2BC(keysym)] = 0;
                    event = event1;
                    goto skipXNextEvent;
                }
            }
            else
            {
                OnKeyDown(XKEYCODE2BC(keysym), BC_RELEASED);
                Keys[XKEYCODE2BC(keysym)] = 0;
            }
            break;
        case MotionNotify:
            MousePosition.x = (f32)event.xmotion.x;
            MousePosition.y = (f32)event.xmotion.y;
            OnMouseMove((f64)event.xmotion.x, (f64)event.xmotion.y);

            break;
        case EnterNotify:
            //BC_LOG("EnterNotify: \n");
            break;
        case LeaveNotify:
            //BC_LOG("LeaveNotify: \n");
            break;
        case ClientMessage:
            m_Window->GetBackendWindow()->ProcessEvent(&event);
            break;
        default:
            //BC_LOG("Unknown X event\n");
            break;
        }
    }
}

void InputManager::SetMousePosition(const Vec2& pos)
{
    BC_LOG("Moving to: %f x %f", pos.x, pos.y);

    if(pos != MousePosition)
    {
        BackendWindow* nwi = m_Window->GetBackendWindow();

        XWarpPointer(nwi->m_Display, None, nwi->m_Window, 0, 0, 0, 0, pos.x, pos.y);

        XSync(nwi->m_Display, False);
    }
}

void InputManager::HideCursor(bool Hide)
{
    Display* dis = m_Window->GetBackendWindow()->m_Display;
    Window win = m_Window->GetBackendWindow()->m_Window;

    if(Impl->EmptyCursor == None)
    {
        char data[1] = {0};
        XColor color;
        Pixmap pixmap;

        color.red = color.green = color.blue = 0;
        pixmap = XCreateBitmapFromData(dis, DefaultRootWindow(dis), data, 1, 1);
        if(pixmap)
        {
            Impl->EmptyCursor = XCreatePixmapCursor(dis, pixmap, pixmap,
                &color, &color, 0, 0);
            XFreePixmap(dis, pixmap);
        }
    }

    XDefineCursor(dis, win, Impl->EmptyCursor);
    XFlush(dis);
}

}
#endif
