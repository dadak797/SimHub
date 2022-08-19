#include "common.hpp"
#include "GlfwOcctView.hpp"


int main(int argc, const char** argv) 
{
    SPDLOG_INFO("Hello, SimHub!");
    
    GlfwOcctView& view = GlfwOcctView::Instance();
    try
    {
        view.run();
    }
    catch (const std::exception& error)
    {
        SPDLOG_ERROR(error.what());
        return EXIT_FAILURE;
    }

    SPDLOG_INFO("Bye, SimHub!");
    return EXIT_SUCCESS;
}