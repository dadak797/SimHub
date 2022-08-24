#ifndef __GLFWOCCTVIEW_H__
#define __GLFWOCCTVIEW_H__

#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewController.hxx>


#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
    #include <emscripten/key_codes.h>
#endif

class GlfwOcctWindow;


class GlfwOcctView /*: protected AIS_ViewController*/
{
public:
    static GlfwOcctView& Instance();

    // Destructor
    ~GlfwOcctView() = default;

    void run();

private:
    // Constructor
    GlfwOcctView();

    // Create window
    void initWindow();

    // Create 3D Viewer
    bool initViewer();

//   //! Handle view redraw.
//   virtual void handleViewRedraw (const Handle(AIS_InteractiveContext)& theCtx,
//                                  const Handle(V3d_View)& theView) override {}

//   //! Schedule processing of window input events with the next repaint event.
//   virtual void ProcessInput() override {}

//   //! Handle key down event.
//   virtual void KeyDown (Aspect_VKey theKey,
//                         double theTime,
//                         double thePressure) override {}

//   //! Handle key up event.
//   virtual void KeyUp (Aspect_VKey theKey,
//                       double theTime) override {}


private:
    // Emscripten callbacks (Static functions)
    // static EM_BOOL onResizeCallback(int eventType, const EmscriptenUiEvent* event, void* view)
    // { return ((GlfwOcctView*)view)->onResizeEvent(eventType, event); }

    // static void onRedrawView(void* view)
    // { return ((GlfwOcctView*)view)->redrawView(); }

    // static EM_BOOL onMouseCallback(int eventType, const EmscriptenMouseEvent* event, void* view)
    // { return ((GlfwOcctView*)view)->onMouseEvent(eventType, event); }

    // static EM_BOOL onWheelCallback (int eventType, const EmscriptenWheelEvent* event, void* view)
    // { return ((GlfwOcctView*)view)->onWheelEvent(eventType, event); }

    // static EM_BOOL onTouchCallback (int eventType, const EmscriptenTouchEvent* event, void* view)
    // { return ((GlfwOcctView*)view)->onTouchEvent(eventType, event); }

    // static EM_BOOL onKeyDownCallback (int eventType, const EmscriptenKeyboardEvent* event, void* view)
    // { return ((GlfwOcctView*)view)->onKeyDownEvent(eventType, event); }

    // static EM_BOOL onKeyUpCallback (int eventType, const EmscriptenKeyboardEvent* event, void* view)
    // { return ((GlfwOcctView*)view)->onKeyUpEvent(eventType, event); }

    // static EM_BOOL onFocusCallback (int eventType, const EmscriptenFocusEvent* event, void* view)
    // { return ((GlfwOcctView*)view)->onFocusEvent(eventType, event); }

    static void onGlfwErrorCallback(int error, const char* description);

private:
    Handle(GlfwOcctWindow) m_Window;
    Handle(V3d_View) m_View;
    Handle(AIS_InteractiveContext) m_Context;
    //Graphic3d_Vec2i m_WinSizeOld;
    //double m_DevicePixelRatio;
    //unsigned int myUpdateRequests;
};

#endif  // __GLFWOCCTVIEW_H__