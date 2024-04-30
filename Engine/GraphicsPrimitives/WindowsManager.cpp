#include"WindowsManager.h"


#include<GL/glew.h>

bool WindowsManager::Initialize() {
#if defined _DEBUG
    if (glfwInitialized) {
        __debugbreak();//glfw is already initialized, you probably got some logic problems in code
        return false;
    }
    glfwInitialized = true;
#endif

    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}
GLFWmonitor* WindowsManager::getCurrentMonitor() {
    return glfwGetPrimaryMonitor();
}
const GLFWvidmode* WindowsManager::getMonitorData(GLFWmonitor* monitor) {
    return glfwGetVideoMode(monitor);
}
void WindowsManager::Uninitialize() {
    glfwTerminate();
}

//temp
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        std::cout << "pressed e??\n";
}

Window::Window(int width, int height, const char* name, GLFWmonitor* monitor) {
    GLFWwindow* window = glfwCreateWindow(width, height, name, monitor, NULL);
    if (!window)
    {
        __debugbreak();//window failed to spawn
        return;
    }

    WindowPtr = window;

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(WindowPtr, key_callback);

    if (glewInit() != GLEW_OK) {
        __debugbreak();//glew failed for some reason
        return;
    }

    glSC(glEnable(GL_BLEND));
    glSC(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glSC(glClearColor(0.f, 0.f, 0.f, 1.f));

    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));//GL_MIRRORED_REPEAT
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));//GL_MIRRORED_REPEAT

    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));//when downscaling
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//when upscaling
}
void Window::SetWindowSize(unsigned int width, unsigned int height) {
    glfwSetWindowSize(WindowPtr, width, height);
}
void Window::SetWindowColor(float r, float g, float b) {
    glfwMakeContextCurrent(WindowPtr);
    glSC(glClearColor(r, g, b, 1.f));
}
void Window::SetSwapInterval(unsigned int interval) {
    glfwSwapInterval(interval);
}
bool Window::WindowWaitingToBeClosed() const {
    return glfwWindowShouldClose(WindowPtr);
}
void Window::Destroy() {
    glfwDestroyWindow(WindowPtr);
}
void Window::GetWindowSize(unsigned int* width, unsigned  int* height) const {
    int w, h;
    glfwGetWindowSize(WindowPtr, &w, &h);
    *width = w;
    *height = h;
}
void Window::UpdateWindowRenderingSize(unsigned int width, unsigned int height) {
    glSC(glViewport(0, 0, width, height));
}
void Window::GetCursorPosition(float* x, float* y) const {
    unsigned int width, height;
    GetWindowSize(&width, &height);

    double tx, ty;
    glfwGetCursorPos(WindowPtr, &tx, &ty);

    (*x) = (float)tx - (float)width / 2;
    (*y) = (float)height / 2 - (float)ty;
}
void Window::StartUpdatingWindow() {

    glSC(glClear(GL_COLOR_BUFFER_BIT));

    if (UpdateWindowRenderingSizeAutomatically) {
        unsigned int width, height;
        GetWindowSize(&width, &height);
        UpdateWindowRenderingSize(width, height);
    }
}
void Window::EndUpdatingWindow() {
    glfwSwapBuffers(WindowPtr);
    glfwPollEvents();
}