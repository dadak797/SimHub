#ifndef __COMMON_H__
#define __COMMON_H__


#include <iostream>

#ifdef __EMSCRIPTEN__
    #include <emscripten/emscripten.h>
    #define GLFW_INCLUDE_ES3
    #define GLFW_INCLUDE_GLEXT
#else  // __EMSCRIPTEN__
    #include <glad/glad.h>
#endif  // __EMSCRIPTEN__

#include <GLFW/glfw3.h>

#ifndef __EMSCRIPTEN__
    #include <GLFW/glfw3native.h>
#endif

#include <spdlog/spdlog.h>


#define CLASS_PTR(klassName) \
    class klassName; \
    using klassName ## UPtr = std::unique_ptr<klassName>; \
    using klassName ## Ptr = std::shared_ptr<klassName>; \
    using klassName ## WPtr = std::weak_ptr<klassName>;


#endif // __COMMON_H__