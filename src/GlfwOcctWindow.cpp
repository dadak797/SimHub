#include "common.hpp"
#include "GlfwOcctWindow.hpp"

#include <Aspect_WindowInputListener.hxx>
#include <Aspect_ScrollDelta.hxx>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
    #include <emscripten/key_codes.h>
#endif


GlfwOcctWindow::GlfwOcctWindow(const char* title, bool toScaleBacking)
    : m_Title(title), m_Size(WINDOW_WIDTH, WINDOW_HEIGHT), m_DevicePixelRatio(1.0), m_ToScaleBacking(toScaleBacking), m_IsMapped(true)
{
#ifdef __EMSCRIPTEN__
    m_DevicePixelRatio = emscripten_get_device_pixel_ratio();
    emscripten_get_canvas_element_size(m_Title, &m_Size.x(), &m_Size.y());
    if (m_ToScaleBacking)
    {
        m_DevicePixelRatio = emscripten_get_device_pixel_ratio();
        Graphic3d_Vec2d cssSize;
        emscripten_get_element_css_size(m_Title, &cssSize.x(), &cssSize.y());
        Graphic3d_Vec2i canvasSize = Graphic3d_Vec2i(cssSize * m_DevicePixelRatio);
        if (canvasSize != m_Size)
        {
            m_Size = canvasSize;
            emscripten_set_canvas_element_size(m_Title, canvasSize.x(), canvasSize.y());
            emscripten_set_element_css_size(m_Title, cssSize.x(), cssSize.y());
        }
    }  
#endif

    m_Window = glfwCreateWindow(m_Size.x(), m_Size.y(), m_Title, nullptr, nullptr);
    if (!m_Window)
    {
        SPDLOG_ERROR("Failed to create GLFW Window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(m_Window);  // Only for emscripten? Not sure
}

GlfwOcctWindow::~GlfwOcctWindow()
{
    if (m_Window) 
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
}

bool GlfwOcctWindow::IsMapped() const
{
#if __EMSCRIPTEN__
    return m_IsMapped;
#else
    return glfwGetWindowAttrib(m_Window, GLFW_VISIBLE);
#endif
}

void GlfwOcctWindow::Map() const
{
#if __EMSCRIPTEN__
    m_IsMapped = true;
#else
    glfwShowWindow(m_Window);
#endif
}

void GlfwOcctWindow::Unmap() const
{
#if __EMSCRIPTEN__
    m_IsMapped = false;
#else
    glfwHideWindow(m_Window);
#endif
}

Aspect_TypeOfResize GlfwOcctWindow::DoResize()
{
    if (IsVirtual())
    {
        return Aspect_TOR_UNKNOWN;
    }

#ifdef __EMSCRIPTEN__
    emscripten_get_canvas_element_size(m_Title, &m_Size.x(), &m_Size.y());
    if (m_ToScaleBacking)
    {
        m_DevicePixelRatio = emscripten_get_device_pixel_ratio();
        Graphic3d_Vec2d cssSize;
        emscripten_get_element_css_size(m_Title, &cssSize.x(), &cssSize.y());
        Graphic3d_Vec2i canvasSize = Graphic3d_Vec2i(cssSize * m_DevicePixelRatio);
        if (canvasSize != m_Size)
        {
            m_Size = canvasSize;
            emscripten_set_canvas_element_size(m_Title, canvasSize.x(), canvasSize.y());
            emscripten_set_element_css_size(m_Title, cssSize.x(), cssSize.y());
        }
    }
#else
    if (glfwGetWindowAttrib(m_Window, GLFW_VISIBLE) == 1)
    {
        glfwGetWindowSize(myGlfwWindow, &m_Size.x(), &m_Size.y());
    }
#endif

    return Aspect_TOR_UNKNOWN;  
}

double GlfwOcctWindow::Ratio() const
{
    Graphic3d_Vec2i canvasSize = m_Size;
    if (!IsVirtual())
    {
    #ifdef __EMSCRIPTEN__
        emscripten_get_canvas_element_size(m_Title, &canvasSize.x(), &canvasSize.y());
    #else
        glfwGetWindowSize(m_Window, &canvasSize.x(), &canvasSize.y());
    #endif
    }

    return (canvasSize.x() != 0 && canvasSize.y() != 0) ?
        double(canvasSize.x()) / double(canvasSize.y()) : 1.0;
}

void GlfwOcctWindow::Position(int& x1, int& y1, int& x2, int& y2) const
{
#ifdef __EMSCRIPTEN__
    x1 = 0; y1 = 0;
#else
    glfwGetWindowPos(m_Window, &x1, &y1);
#endif

    int width, height;
#ifdef __EMSCRIPTEN__
    emscripten_get_canvas_element_size(m_Title, &width, &height);
#else
    glfwGetWindowSize(m_Window, &width, &height);
#endif
    x2 = x1 + width;
    y2 = y1 + height;
}

void GlfwOcctWindow::Size(int& width, int& height) const
{
#ifdef __EMSCRIPTEN__
    emscripten_get_canvas_element_size(m_Title, &width, &height);
#else
    glfwGetWindowSize(m_Window, &width, &height);
#endif
}

void GlfwOcctWindow::SetSizeLogical(const Graphic3d_Vec2d& size)
{
    m_Size = Graphic3d_Vec2i(size * m_DevicePixelRatio);
    if (IsVirtual())
    {
        return;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_canvas_element_size(m_Title, m_Size.x(), m_Size.y());
    emscripten_set_element_css_size(m_Title, m_Size.x(), m_Size.y());
#endif
}

Aspect_Drawable GlfwOcctWindow::NativeHandle() const
{
#ifdef __EMSCRIPTEN__
    return 0;
#else  // For _WIN32
    return (Aspect_Drawable)glfwGetWin32Window(m_Window);
#endif
}

Aspect_RenderingContext GlfwOcctWindow::NativeGlContext() const
{
#ifdef __EMSCRIPTEN__
    return 0;
#else  // For _WIN32
    return glfwGetWGLContext(m_Window);
#endif
}

Graphic3d_Vec2i GlfwOcctWindow::CursorPosition() const
{
    Graphic3d_Vec2d pos;
    glfwGetCursorPos(m_Window, &pos.x(), &pos.y());
    return Graphic3d_Vec2i((int)pos.x(), (int)pos.y());
}

Aspect_VKeyMouse GlfwOcctWindow::MouseButtonsFromNative(unsigned short buttons)
{
    Aspect_VKeyMouse aButtons = Aspect_VKeyMouse_NONE;
    if ((buttons & 0x1) != 0)
    {
        aButtons |= Aspect_VKeyMouse_LeftButton;
    }
    if ((buttons & 0x2) != 0)
    {
        aButtons |= Aspect_VKeyMouse_RightButton;
    }
    if ((buttons & 0x4) != 0)
    {
        aButtons |= Aspect_VKeyMouse_MiddleButton;
    }
    return aButtons;
}

Aspect_VKey GlfwOcctWindow::VirtualKeyFromNative(int key)
{
#ifdef __EMSCRIPTEN__
    if (key >= DOM_VK_0 && key <= DOM_VK_9)
    {
        // numpad keys
        return Aspect_VKey((key - DOM_VK_0) + Aspect_VKey_0);
    }
    if (key >= DOM_VK_A && key <= DOM_VK_Z)
    {
        // main latin alphabet keys
        return Aspect_VKey((key - DOM_VK_A) + Aspect_VKey_A);
    }
    if (key >= DOM_VK_F1 && key <= DOM_VK_F24)
    {
        // special keys
        if (key <= DOM_VK_F12)
        {
          return Aspect_VKey((key - DOM_VK_F1) + Aspect_VKey_F1);
        }
        return Aspect_VKey_UNKNOWN;
    }
    if (key >= DOM_VK_NUMPAD0 && key <= DOM_VK_NUMPAD9)
    {
        // numpad keys
        return Aspect_VKey((key - DOM_VK_NUMPAD0) + Aspect_VKey_Numpad0);
    }

    switch (key)
    {
    case DOM_VK_CANCEL:
    case DOM_VK_HELP: return Aspect_VKey_UNKNOWN;
    case DOM_VK_BACK_SPACE: return Aspect_VKey_Backspace;
    case DOM_VK_TAB: return Aspect_VKey_Tab;
    case DOM_VK_CLEAR: return Aspect_VKey_UNKNOWN;
    case DOM_VK_RETURN:
    case DOM_VK_ENTER: return Aspect_VKey_Enter;
    case DOM_VK_SHIFT: return Aspect_VKey_Shift;
    case DOM_VK_CONTROL: return Aspect_VKey_Control;
    case DOM_VK_ALT: return Aspect_VKey_Alt;
    case DOM_VK_PAUSE:
    case DOM_VK_CAPS_LOCK:
    case DOM_VK_KANA:
    //case DOM_VK_HANGUL:
    case DOM_VK_EISU:
    case DOM_VK_JUNJA:
    case DOM_VK_FINAL:
    case DOM_VK_HANJA: return Aspect_VKey_UNKNOWN;
    //case DOM_VK_KANJI:        
    case DOM_VK_ESCAPE: return Aspect_VKey_Escape;
    case DOM_VK_CONVERT:
    case DOM_VK_NONCONVERT:
    case DOM_VK_ACCEPT:
    case DOM_VK_MODECHANGE: return Aspect_VKey_UNKNOWN;
    case DOM_VK_SPACE: return Aspect_VKey_Space;
    case DOM_VK_PAGE_UP: return Aspect_VKey_PageUp;
    case DOM_VK_PAGE_DOWN: return Aspect_VKey_PageDown;
    case DOM_VK_END: return Aspect_VKey_End;
    case DOM_VK_HOME: return Aspect_VKey_Home;
    case DOM_VK_LEFT: return Aspect_VKey_Left;
    case DOM_VK_UP: return Aspect_VKey_Up;
    case DOM_VK_RIGHT: return Aspect_VKey_Right;
    case DOM_VK_DOWN: return Aspect_VKey_Down;
    case DOM_VK_SELECT:
    case DOM_VK_PRINT:
    case DOM_VK_EXECUTE:
    case DOM_VK_PRINTSCREEN:
    case DOM_VK_INSERT: return Aspect_VKey_UNKNOWN;
    case DOM_VK_DELETE: return Aspect_VKey_Delete;
    case DOM_VK_COLON: return Aspect_VKey_Comma;
    case DOM_VK_SEMICOLON: return Aspect_VKey_Semicolon;
    case DOM_VK_LESS_THAN: return Aspect_VKey_UNKNOWN;
    case DOM_VK_EQUALS: return Aspect_VKey_Equal;
    case DOM_VK_GREATER_THAN: return Aspect_VKey_UNKNOWN;
    case DOM_VK_QUESTION_MARK: return Aspect_VKey_Slash;
    case DOM_VK_AT: return Aspect_VKey_UNKNOWN; // @ key
    case DOM_VK_WIN: return Aspect_VKey_Meta;
    case DOM_VK_CONTEXT_MENU:
    case DOM_VK_SLEEP: return Aspect_VKey_UNKNOWN;
    case DOM_VK_MULTIPLY: return Aspect_VKey_NumpadMultiply;
    case DOM_VK_ADD: return Aspect_VKey_NumpadAdd;
    case DOM_VK_SEPARATOR: return Aspect_VKey_UNKNOWN;
    case DOM_VK_SUBTRACT: return Aspect_VKey_NumpadSubtract;
    case DOM_VK_DECIMAL: return Aspect_VKey_UNKNOWN;
    case DOM_VK_DIVIDE: return Aspect_VKey_NumpadDivide;
    case DOM_VK_NUM_LOCK: return Aspect_VKey_Numlock;
    case DOM_VK_SCROLL_LOCK: return Aspect_VKey_Scroll;
    case DOM_VK_WIN_OEM_FJ_JISHO:
    case DOM_VK_WIN_OEM_FJ_MASSHOU:
    case DOM_VK_WIN_OEM_FJ_TOUROKU:
    case DOM_VK_WIN_OEM_FJ_LOYA:
    case DOM_VK_WIN_OEM_FJ_ROYA:
    case DOM_VK_CIRCUMFLEX: return Aspect_VKey_UNKNOWN;
    case DOM_VK_EXCLAMATION:
    case DOM_VK_DOUBLE_QUOTE:
    //case DOM_VK_HASH:
    case DOM_VK_DOLLAR:
    case DOM_VK_PERCENT:
    case DOM_VK_AMPERSAND:
    case DOM_VK_UNDERSCORE:
    case DOM_VK_OPEN_PAREN:
    case DOM_VK_CLOSE_PAREN:
    case DOM_VK_ASTERISK: return Aspect_VKey_UNKNOWN;
    case DOM_VK_PLUS: return Aspect_VKey_Plus;
    case DOM_VK_PIPE:
    case DOM_VK_HYPHEN_MINUS: return Aspect_VKey_UNKNOWN;
    case DOM_VK_OPEN_CURLY_BRACKET: return Aspect_VKey_BracketLeft;
    case DOM_VK_CLOSE_CURLY_BRACKET: return Aspect_VKey_BracketRight;
    case DOM_VK_TILDE: return Aspect_VKey_Tilde;
    case DOM_VK_VOLUME_MUTE: return Aspect_VKey_VolumeMute;
    case DOM_VK_VOLUME_DOWN: return Aspect_VKey_VolumeDown;
    case DOM_VK_VOLUME_UP: return Aspect_VKey_VolumeUp;
    case DOM_VK_COMMA: return Aspect_VKey_Comma;
    case DOM_VK_PERIOD: return Aspect_VKey_Period;
    case DOM_VK_SLASH: return Aspect_VKey_Slash;
    case DOM_VK_BACK_QUOTE: return Aspect_VKey_UNKNOWN;
    case DOM_VK_OPEN_BRACKET: return Aspect_VKey_BracketLeft;
    case DOM_VK_BACK_SLASH: return Aspect_VKey_Backslash;
    case DOM_VK_CLOSE_BRACKET: return Aspect_VKey_BracketRight;
    case DOM_VK_QUOTE: return Aspect_VKey_UNKNOWN;
    case DOM_VK_META: return Aspect_VKey_Meta;
    case DOM_VK_ALTGR: return Aspect_VKey_Alt;
    case DOM_VK_WIN_ICO_HELP:
    case DOM_VK_WIN_ICO_00:
    case DOM_VK_WIN_ICO_CLEAR:
    case DOM_VK_WIN_OEM_RESET:
    case DOM_VK_WIN_OEM_JUMP:
    case DOM_VK_WIN_OEM_PA1:
    case DOM_VK_WIN_OEM_PA2:
    case DOM_VK_WIN_OEM_PA3:
    case DOM_VK_WIN_OEM_WSCTRL:
    case DOM_VK_WIN_OEM_CUSEL:
    case DOM_VK_WIN_OEM_ATTN:
    case DOM_VK_WIN_OEM_FINISH:
    case DOM_VK_WIN_OEM_COPY:
    case DOM_VK_WIN_OEM_AUTO:
    case DOM_VK_WIN_OEM_ENLW:
    case DOM_VK_WIN_OEM_BACKTAB:
    case DOM_VK_ATTN:
    case DOM_VK_CRSEL:
    case DOM_VK_EXSEL:
    case DOM_VK_EREOF: return Aspect_VKey_UNKNOWN;
    case DOM_VK_PLAY: return Aspect_VKey_MediaPlayPause;
    case DOM_VK_ZOOM:
    case DOM_VK_PA1:
    case DOM_VK_WIN_OEM_CLEAR: return Aspect_VKey_UNKNOWN;
    }
#else
    (void)key;
#endif
    return Aspect_VKey_UNKNOWN;
}

bool GlfwOcctWindow::ProcessMessage(Aspect_WindowInputListener& listener,
                                    int eventType, const void* event)
{
#ifdef __EMSCRIPTEN__
    switch (eventType)
    {
    case EMSCRIPTEN_EVENT_MOUSEMOVE:
    case EMSCRIPTEN_EVENT_MOUSEDOWN:
    case EMSCRIPTEN_EVENT_MOUSEUP:
    case EMSCRIPTEN_EVENT_CLICK:
    case EMSCRIPTEN_EVENT_DBLCLICK:
    case EMSCRIPTEN_EVENT_MOUSEENTER:
    case EMSCRIPTEN_EVENT_MOUSELEAVE:
    {
        return ProcessMouseEvent(listener, eventType, (const EmscriptenMouseEvent*)event);
    }
    case EMSCRIPTEN_EVENT_TOUCHSTART:
    case EMSCRIPTEN_EVENT_TOUCHMOVE:
    case EMSCRIPTEN_EVENT_TOUCHEND:
    case EMSCRIPTEN_EVENT_TOUCHCANCEL:
    {
        return ProcessTouchEvent(listener, eventType, (const EmscriptenTouchEvent*)event);
    }
    case EMSCRIPTEN_EVENT_WHEEL:
    {
        return ProcessWheelEvent(listener, eventType, (const EmscriptenWheelEvent*)event);
    }
    case EMSCRIPTEN_EVENT_KEYDOWN:
    case EMSCRIPTEN_EVENT_KEYUP:
    case EMSCRIPTEN_EVENT_KEYPRESS:
    {
        return ProcessKeyEvent(listener, eventType, (const EmscriptenKeyboardEvent*)event);
    }
    case EMSCRIPTEN_EVENT_RESIZE:
    case EMSCRIPTEN_EVENT_CANVASRESIZED:
    {
        return ProcessUiEvent(listener, eventType, (const EmscriptenUiEvent*)event);
    }
    case EMSCRIPTEN_EVENT_FOCUS:
    case EMSCRIPTEN_EVENT_FOCUSIN:
    case EMSCRIPTEN_EVENT_FOCUSOUT:
    {
        return ProcessFocusEvent (listener, eventType, (const EmscriptenFocusEvent*)event);
    }
    }
    return false;
#else
    (void)listener;
    (void)eventType;
    (void)event;
    return false;
#endif
}

bool GlfwOcctWindow::ProcessMouseEvent(Aspect_WindowInputListener& listener,
                                       int eventType, const EmscriptenMouseEvent* event)
{
#ifdef __EMSCRIPTEN__
    const Graphic3d_Vec2d aNewPos2d = ConvertPointToBacking(Graphic3d_Vec2d(event->targetX, event->targetY));
    const Graphic3d_Vec2i aNewPos2i = Graphic3d_Vec2i(aNewPos2d + Graphic3d_Vec2d(0.5));
    Aspect_VKeyFlags aFlags = 0;
    if (event->ctrlKey == EM_TRUE) { aFlags |= Aspect_VKeyFlags_CTRL; }
    if (event->shiftKey == EM_TRUE) { aFlags |= Aspect_VKeyFlags_SHIFT; }
    if (event->altKey == EM_TRUE) { aFlags |= Aspect_VKeyFlags_ALT; }
    if (event->metaKey == EM_TRUE) { aFlags |= Aspect_VKeyFlags_META; }

    const bool isEmulated = false;
    const Aspect_VKeyMouse aButtons = GlfwOcctWindow::MouseButtonsFromNative(event->buttons);
    switch (eventType)
    {
    case EMSCRIPTEN_EVENT_MOUSEMOVE:
    {
        if ((aNewPos2i.x() < 0 || aNewPos2i.x() > m_Size.x() ||
             aNewPos2i.y() < 0 || aNewPos2i.y() > m_Size.y()) &&
             listener.PressedMouseButtons() == Aspect_VKeyMouse_NONE)
        {
            return false;
        }
        if (listener.UpdateMousePosition(aNewPos2i, aButtons, aFlags, isEmulated))
        {
            listener.ProcessInput();
        }
        break;
    }
    case EMSCRIPTEN_EVENT_MOUSEDOWN:
    case EMSCRIPTEN_EVENT_MOUSEUP:
    {
        if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
        {
            if (aNewPos2i.x() < 0 || aNewPos2i.x() > m_Size.x() ||
                aNewPos2i.y() < 0 || aNewPos2i.y() > m_Size.y())
            {
                return false;
            }
        }
        if (listener.UpdateMouseButtons(aNewPos2i, aButtons, aFlags, isEmulated))
        {
            listener.ProcessInput();
        }
        break;
    }
    case EMSCRIPTEN_EVENT_CLICK:
    case EMSCRIPTEN_EVENT_DBLCLICK:
    {
        if (aNewPos2i.x() < 0 || aNewPos2i.x() > m_Size.x() ||
            aNewPos2i.y() < 0 || aNewPos2i.y() > m_Size.y())
        {
            return false;
        }
        break;
    }
    case EMSCRIPTEN_EVENT_MOUSEENTER:
    {
        break;
    }
    case EMSCRIPTEN_EVENT_MOUSELEAVE:
    {
        // there is no SetCapture() support, so that mouse unclick events outside canvas will not arrive,
        // so we have to forget current state...
        if (listener.UpdateMouseButtons(aNewPos2i, Aspect_VKeyMouse_NONE, aFlags, isEmulated))
        {
            listener.ProcessInput();
        }
        break;
    }
    }
    return true;
#else
    (void)listener;
    (void)eventType;
    (void)event;
    return false;
#endif
}

bool GlfwOcctWindow::ProcessWheelEvent(Aspect_WindowInputListener& listener,
                                       int eventType, const EmscriptenWheelEvent* event)
{
#ifdef __EMSCRIPTEN__
    if (eventType != EMSCRIPTEN_EVENT_WHEEL)
    {
        return false;
    }

    const Graphic3d_Vec2d aNewPos2d = ConvertPointToBacking(Graphic3d_Vec2d (event->mouse.targetX, event->mouse.targetY));
    const Graphic3d_Vec2i aNewPos2i = Graphic3d_Vec2i(aNewPos2d + Graphic3d_Vec2d(0.5));
    if (aNewPos2i.x() < 0 || aNewPos2i.x() > m_Size.x() ||
        aNewPos2i.y() < 0 || aNewPos2i.y() > m_Size.y())
    {
        return false;
    }

    double aDelta = 0.0;
    switch (event->deltaMode)
    {
    case DOM_DELTA_PIXEL:
    {
        aDelta = event->deltaY / (5.0 * DevicePixelRatio());
        break;
    }
    case DOM_DELTA_LINE:
    {
        aDelta = event->deltaY * 8.0;
        break;
    }
    case DOM_DELTA_PAGE:
    {
        aDelta = event->deltaY >= 0.0 ? 24.0 : -24.0;
        break;
    }
    }
    aDelta /= 15.0;

    if (listener.UpdateMouseScroll(Aspect_ScrollDelta(aNewPos2i, -aDelta)))
    {
        listener.ProcessInput();
    }
    return true;
#else
    (void)listener;
    (void)eventType;
    (void)event;
    return false;
#endif
}

bool GlfwOcctWindow::ProcessTouchEvent(Aspect_WindowInputListener& listener,
                                       int eventType, const EmscriptenTouchEvent* event)
{
    bool hasUpdates = false;
#ifdef __EMSCRIPTEN__
    if (eventType != EMSCRIPTEN_EVENT_TOUCHSTART && 
        eventType != EMSCRIPTEN_EVENT_TOUCHMOVE && 
        eventType != EMSCRIPTEN_EVENT_TOUCHEND && 
        eventType != EMSCRIPTEN_EVENT_TOUCHCANCEL)
    {
        return false;
    }

    for (int aTouchIter = 0; aTouchIter < event->numTouches; ++aTouchIter)
    {
        const EmscriptenTouchPoint& aTouch = event->touches[aTouchIter];
        if (!aTouch.isChanged)
        {
            continue;
        }

        const Standard_Size aTouchId = (Standard_Size)aTouch.identifier;

        const Graphic3d_Vec2d aNewPos2d = ConvertPointToBacking(Graphic3d_Vec2d (aTouch.targetX, aTouch.targetY));
        const Graphic3d_Vec2i aNewPos2i = Graphic3d_Vec2i(aNewPos2d + Graphic3d_Vec2d(0.5));
        switch (eventType)
        {
        case EMSCRIPTEN_EVENT_TOUCHSTART:
        {
            if (aNewPos2i.x() >= 0 && aNewPos2i.x() < m_Size.x() &&
                aNewPos2i.y() >= 0 && aNewPos2i.y() < m_Size.y())
            {
                hasUpdates = true;
                listener.AddTouchPoint(aTouchId, aNewPos2d);
            }
            break;
        }
        case EMSCRIPTEN_EVENT_TOUCHMOVE:
        {
            const int anOldIndex = listener.TouchPoints().FindIndex(aTouchId);
            if (anOldIndex != 0)
            {
                hasUpdates = true;
                listener.UpdateTouchPoint(aTouchId, aNewPos2d);
            }
            break;
        }
        case EMSCRIPTEN_EVENT_TOUCHEND:
        case EMSCRIPTEN_EVENT_TOUCHCANCEL:
        {
            if (listener.RemoveTouchPoint(aTouchId))
            {
                hasUpdates = true;
            }
            break;
        }
        }
    }
    if (hasUpdates)
    {
        listener.ProcessInput();
    }
#else
    (void)eventType;
    (void)event;
#endif
    return hasUpdates || listener.HasTouchPoints();
}

bool GlfwOcctWindow::ProcessKeyEvent(Aspect_WindowInputListener& listener,
                                     int eventType, const EmscriptenKeyboardEvent* event)
{
#ifdef __EMSCRIPTEN__
    if (eventType != EMSCRIPTEN_EVENT_KEYDOWN &&
        eventType != EMSCRIPTEN_EVENT_KEYUP &&
        eventType != EMSCRIPTEN_EVENT_KEYPRESS)
    {
        return false;
    }

    const double aTimeStamp = listener.EventTime();
    const Aspect_VKey aVKey = GlfwOcctWindow::VirtualKeyFromNative(event->keyCode);
    if (aVKey == Aspect_VKey_UNKNOWN)
    {
        return false;
    }

    switch (eventType)
    {
    case EMSCRIPTEN_EVENT_KEYDOWN:
    {
        if (event->repeat == EM_TRUE)
        {
            return false;
        }

        listener.KeyDown(aVKey, aTimeStamp);
        listener.ProcessInput();
        return false;
    }
    case EMSCRIPTEN_EVENT_KEYUP:
    {
        listener.KeyUp(aVKey, aTimeStamp);
        listener.ProcessInput();
        return false;
    }
    }
#else
    (void)listener;
    (void)eventType;
    (void)event;
#endif
    return false;
}

bool GlfwOcctWindow::ProcessUiEvent(Aspect_WindowInputListener& listener,
                                    int eventType, const EmscriptenUiEvent*)
{
#if defined(__EMSCRIPTEN__)
    if (eventType != EMSCRIPTEN_EVENT_RESIZE &&
        eventType != EMSCRIPTEN_EVENT_CANVASRESIZED)
    {
        return false;
    }
#else
    (void)eventType;
#endif
    listener.ProcessConfigure(true);
    return true;
}

bool GlfwOcctWindow::ProcessFocusEvent(Aspect_WindowInputListener& listener,
                                       int eventType, const EmscriptenFocusEvent*)
{
    bool isActivated = false;
#ifdef __EMSCRIPTEN__
    if (eventType != EMSCRIPTEN_EVENT_FOCUS &&
        eventType != EMSCRIPTEN_EVENT_FOCUSIN && // about to receive focus
        eventType != EMSCRIPTEN_EVENT_FOCUSOUT)
    {
        return false;
    }
    isActivated = eventType == EMSCRIPTEN_EVENT_FOCUS;
#else
    (void)eventType;
#endif
    listener.ProcessFocus(isActivated);
    return true;
}