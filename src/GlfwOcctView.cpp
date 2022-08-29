#include "common.hpp"
#include "GlfwOcctView.hpp"
#include "GlfwOcctWindow.hpp"


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
    //initWindow();
    //initViewer();
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

    return true;
}

