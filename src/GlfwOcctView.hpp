#ifndef __GLFWOCCTVIEW_H__
#define __GLFWOCCTVIEW_H__


class GlfwOcctView
{
public:
    static GlfwOcctView& Instance();

    // Destructor
    ~GlfwOcctView() = default;

    void run();

private:
    // Constructor
    GlfwOcctView();
};


#endif  // __GLFWOCCTVIEW_H__