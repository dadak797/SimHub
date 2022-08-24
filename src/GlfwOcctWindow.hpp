#ifndef __GlfwOcctWindow_H__
#define __GlfwOcctWindow_H__

#include <Aspect_Window.hxx>
#include <Aspect_VKey.hxx>
#include <Aspect_RenderingContext.hxx>
#include <Graphic3d_Vec2.hxx>

struct GLFWwindow;

class Aspect_WindowInputListener;

struct EmscriptenMouseEvent;
struct EmscriptenWheelEvent;
struct EmscriptenTouchEvent;
struct EmscriptenKeyboardEvent;
struct EmscriptenUiEvent;
struct EmscriptenFocusEvent;


class GlfwOcctWindow : public Aspect_Window
{
    DEFINE_STANDARD_RTTIEXT(GlfwOcctWindow, Aspect_Window)

public:
    // Convert Emscripten mouse buttons into Aspect_VKeyMouse.
    static Aspect_VKeyMouse MouseButtonsFromNative(unsigned short theButtons);

    // Convert DOM virtual key into Aspect_VKey.
    static Aspect_VKey VirtualKeyFromNative(int theKey);


public:
    // Constructor (Title: window id for win32, canvas id for emscripten)
    GlfwOcctWindow(const char* title, int width, int height, bool toScaleBacking = true);

    // Destructor
    virtual ~GlfwOcctWindow();

    // Return true if window is not hidden
    virtual Standard_Boolean IsMapped() const override;

    // Change window mapped flag to true
    virtual void Map() const override;

    // Change window mapped flag to false
    virtual void Unmap() const override;

    // Resize window
    virtual Aspect_TypeOfResize DoResize() override;

    // Apply the mapping change to the window
    virtual bool DoMapping() const override { return true; }

    // Returns window ratio equal to the physical width/height dimensions
    virtual double Ratio() const override;

    // Returns the window position
    virtual void Position(int& x1, int& y1, int& x2, int& y2) const override;

    // Return the window size
    virtual void Size(int& width, int& height) const override;

    // Set new window size in logical (density-independent units)
    // Backing store will be resized basing on DevicePixelRatio()
    void SetSizeLogical(const Graphic3d_Vec2d& size);

    // Set new window size in pixels.
    // Logical size of the element will be resized basing on DevicePixelRatio()
    Standard_EXPORT void SetSizeBacking (const Graphic3d_Vec2i& theSize);

    //! Returns canvas id.
    const char* GetTitle() const { return m_Title; }

    // Current EGL implementation in Emscripten accepts only 0 for native window id
    virtual Aspect_Drawable NativeHandle() const override;

    // Always returns 0 for this class.
    virtual Aspect_Drawable NativeParentHandle() const override { return 0; }

    // Always returns 0 for this class.
    virtual Aspect_FBConfig NativeFBConfig() const override { return 0; }

    // Return device pixel ratio (logical to backing store scale factor)
    virtual Standard_Real DevicePixelRatio() const override { return m_DevicePixelRatio; }

    // Sets device pixel ratio for a window with IsVirtual() flag
    void SetDevicePixelRatio(double devicePixelRatio) { m_DevicePixelRatio = devicePixelRatio; }

    // Invalidate entire window content through generation of Expose event.
    virtual void InvalidateContent(const Handle(Aspect_DisplayConnection)& theDisp) override {}

    // Get native OpenGL context.
    Aspect_RenderingContext NativeGlContext() const;

    // Return cursor position
    Graphic3d_Vec2i CursorPosition() const;

    // Get GLFW window
    GLFWwindow* GetGlfwWindow() { return m_Window; }

public:
    // Process Events and Messages
    virtual bool ProcessMessage(Aspect_WindowInputListener& listener, int eventType, const void* event);
    virtual bool ProcessMouseEvent(Aspect_WindowInputListener& listener, int eventType, const EmscriptenMouseEvent* event);
    virtual bool ProcessWheelEvent(Aspect_WindowInputListener& listener, int eventType, const EmscriptenWheelEvent* event);
    virtual bool ProcessTouchEvent(Aspect_WindowInputListener& listener, int eventType, const EmscriptenTouchEvent* event);
    virtual bool ProcessKeyEvent(Aspect_WindowInputListener& listener, int eventType, const EmscriptenKeyboardEvent* event);
    virtual bool ProcessUiEvent(Aspect_WindowInputListener& listener, int eventType, const EmscriptenUiEvent* event);
    virtual bool ProcessFocusEvent(Aspect_WindowInputListener& listener, int theEventType, const EmscriptenFocusEvent* event);

private:
    GLFWwindow* m_Window;
    const char* m_Title;
    Graphic3d_Vec2i m_Size;
    double m_DevicePixelRatio;
    bool m_ToScaleBacking;
    mutable bool m_IsMapped;  
};

#endif  // __GlfwOcctWindow_H__