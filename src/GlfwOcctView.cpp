#include "common.hpp"
#include "GlfwOcctView.hpp"
#include "GlfwOcctWindow.hpp"

#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <OpenGl_Context.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>


void GlfwOcctView::onGlfwErrorCallback(int error, const char* description)
{
    SPDLOG_ERROR("Error: {}: {}", error, description);
}

GlfwOcctView& GlfwOcctView::Instance()
{
    static GlfwOcctView s_Instance;
    return s_Instance;
}

GlfwOcctView::GlfwOcctView()
{
}

void GlfwOcctView::run()
{
    initWindow();
    initViewer();
    initDemoScene();
    mainloop();
    cleanup();
}

void GlfwOcctView::initWindow()
{
    glfwSetErrorCallback(GlfwOcctView::onGlfwErrorCallback);
    if(!glfwInit()) {
    #ifdef __EMSCRIPTEN__
        SPDLOG_ERROR("Failed to initialize Glfw");;
    #else
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("Failed to initialize Glfw: {}", description);
    #endif
        exit(EXIT_FAILURE);
    }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __EMSCRIPTEN__    
    m_Window = new GlfwOcctWindow(CANVAS_ID, WINDOW_WIDTH, WINDOW_HEIGHT);
#else
    m_Window = new GlfwOcctWindow(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
#endif
	glfwSetWindowUserPointer(m_Window->GetGlfwWindow(), this);

// 	// // window callback
// 	// glfwSetWindowSizeCallback(window, GlfwOccView::s_onWindowResized);
// 	// // mouse callback
// 	// glfwSetMouseButtonCallback(window, GlfwOccView::s_onMouseButton);
// 	// glfwSetCursorPosCallback(window, GlfwOccView::s_onMouseMove);

// 	// glfwMakeContextCurrent(window);

//     // const EM_BOOL toUseCapture = EM_TRUE;
//     // emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, toUseCapture, onResizeCallback);

//     // emscripten_set_mousedown_callback(aTargetId, this, toUseCapture, onMouseCallback);
//     // // bind these events to window to track mouse movements outside of canvas
//     // //emscripten_set_mouseup_callback(aTargetId, this, toUseCapture, onMouseCallback);
//     // //emscripten_set_mousemove_callback(aTargetId, this, toUseCapture, onMouseCallback);
//     // //emscripten_set_mouseleave_callback(aTargetId, this, toUseCapture, onMouseCallback);
//     // emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, toUseCapture, onMouseCallback);
//     // emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, toUseCapture, onMouseCallback);

//     // emscripten_set_dblclick_callback(aTargetId, this, toUseCapture, onMouseCallback);
//     // emscripten_set_click_callback(aTargetId, this, toUseCapture, onMouseCallback);
//     // emscripten_set_mouseenter_callback(aTargetId, this, toUseCapture, onMouseCallback);
//     // emscripten_set_wheel_callback(aTargetId, this, toUseCapture, onWheelCallback);

//     // emscripten_set_touchstart_callback(aTargetId, this, toUseCapture, onTouchCallback);
//     // emscripten_set_touchend_callback(aTargetId, this, toUseCapture, onTouchCallback);
//     // emscripten_set_touchmove_callback(aTargetId, this, toUseCapture, onTouchCallback);
//     // emscripten_set_touchcancel_callback(aTargetId, this, toUseCapture, onTouchCallback);

//     // //emscripten_set_keypress_callback(aTargetId, this, toUseCapture, onKeyCallback);
//     // emscripten_set_keydown_callback(aTargetId, this, toUseCapture, onKeyDownCallback);
//     // emscripten_set_keyup_callback(aTargetId, this, toUseCapture, onKeyUpCallback);
//     // //emscripten_set_focus_callback(aTargetId, this, toUseCapture, onFocusCallback);
//     // //emscripten_set_focusin_callback(aTargetId, this, toUseCapture, onFocusCallback);
//     // emscripten_set_focusout_callback(aTargetId, this, toUseCapture, onFocusCallback);
}

bool GlfwOcctView::initViewer()
{
    // Font Setting
    // Add font setting code here...

    // OpenGL Graphic Driver
    Handle(Aspect_DisplayConnection) disp;
    Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(disp, false);
    driver->ChangeOptions().buffersNoSwap = true;  // swap has no effect in WebGL
    driver->ChangeOptions().buffersOpaqueAlpha = true;  // avoid unexpected blending of canvas with page background
    if (!driver->InitContext())
    {
        SPDLOG_ERROR("Error: EGL initialization failed");
        return false;
    }

    // V3d Viewer
    Handle(V3d_Viewer) viewer = new V3d_Viewer(driver);
    viewer->SetComputedMode(false);
    viewer->SetDefaultShadingModel(Graphic3d_TypeOfShadingModel_Phong);
    viewer->SetDefaultLights();
    viewer->SetLightOn();
    
    // For Shadow
    // for (V3d_ListOfLight::Iterator aLightIter(aViewer->ActiveLights()); aLightIter.More(); aLightIter.Next())
    // {
    //     const Handle(V3d_Light)& aLight = aLightIter.Value();
    //     if (aLight->Type() == Graphic3d_TypeOfLightSource_Directional)
    //     {
    //         aLight->SetCastShadows(true);
    //     }
    // }

    // Text Style
    // Add text style setting code here...

    //m_View = new V3d_View(viewer);
    m_View = viewer->CreateView();
    m_View->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Perspective);
    //myView->Camera()->SetProjectionType (Graphic3d_Camera::Projection_Orthographic);
    m_View->SetImmediateUpdate(false);
    // m_View->ChangeRenderingParams().IsShadowEnabled = false;
    // m_View->ChangeRenderingParams().Resolution = (unsigned int)(96.0 * m_Window->DevicePixelRatio() + 0.5);
    // m_View->ChangeRenderingParams().ToShowStats = true;
    // m_View->ChangeRenderingParams().StatsTextAspect = myTextStyle->Aspect();
    // m_View->ChangeRenderingParams().StatsTextHeight = (int)myTextStyle->Height();
#ifdef __EMSCRIPTEN__
    m_View->SetWindow(m_Window);
#else
    m_View->SetWindow(m_Window, m_Window->NativeGlContext());
#endif

    // OpenGL Context
    m_GLContext = driver->GetSharedContext();

    if (!m_Window->IsMapped()) m_Window->Map();
    m_View->MustBeResized();

    // AIS Context
    m_Context = new AIS_InteractiveContext(viewer);
    m_Context->SetDisplayMode(AIS_Shaded, true);

    return true;
}

void GlfwOcctView::mainloop()
{
	while (!glfwWindowShouldClose(m_Window->GetGlfwWindow())) {
        drawFrame();
        glfwPollEvents();
	}
}

void GlfwOcctView::initDemoScene()
{
    if (m_Context.IsNull())
    {
        return;
    }

    Handle(AIS_Shape) box = new AIS_Shape(BRepPrimAPI_MakeBox(50.0, 50.0, 50.0).Shape());
    m_View->FitAll();
    m_Context->Display(box, Standard_False);

    m_View->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_WIREFRAME);
}

void GlfwOcctView::drawFrame()
{
    if (!m_View.IsNull())
	{
		m_GLContext->MakeCurrent();
		m_View->Invalidate();

        m_GLContext->SwapBuffers();
		FlushViewEvents(m_Context, m_View, true);
	}    
}

void GlfwOcctView::cleanup()
{
    // Clean Up OCCT
	m_View->Remove();
}
