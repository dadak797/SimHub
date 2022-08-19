#include "GlfwOcctView.hpp"


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
}