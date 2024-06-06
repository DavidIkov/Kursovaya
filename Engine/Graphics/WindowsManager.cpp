#include"WindowsManager.h"
#include"HierarchyClasses/Classes/HierarchyRoot.h"
#include"Dependencies/GLEW/include/GL/glew.h"

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
GLFWmonitor* WindowsManager::gCurrentMonitor() {
    return glfwGetPrimaryMonitor();
}
const GLFWvidmode* WindowsManager::gMonitorData(GLFWmonitor* monitor) {
    return glfwGetVideoMode(monitor);
}
void WindowsManager::Uninitialize() {
    glfwTerminate();
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    for (unsigned int i = 0; i < WindowsManager::Windows.size(); i++)
        if (WindowsManager::Windows[i]->gWindowPtr() == window) WindowsManager::Windows[i]->Keyboard.KeyCallback(key, scancode, action, mods);
}


GLFWwindow* Window::gWindowPtr() const {
    return WindowPtr;
}
Window::~Window() {
    for (unsigned int i = 0; i < WindowsManager::Windows.size(); i++)
        if (WindowsManager::Windows[i]->WindowPtr == WindowPtr) {
            WindowsManager::Windows.erase(WindowsManager::Windows.begin() + i);
            return;
        }
}
Window::Window(int width, int height, const char* name, GLFWmonitor* monitor) {

    WindowsManager::Windows.push_back(this);


    GLFWwindow* window = glfwCreateWindow(width, height, name, monitor, NULL);
    if (!window)
    {
        __debugbreak();//window failed to spawn
        return;
    }


    WindowPtr = window;

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(WindowPtr, KeyCallback);

    if (glewInit() != GLEW_OK) {
        __debugbreak();//glew failed for some reason
        return;
    }

    Hierarchy = new HierarchyRoot(this, (unsigned int)width, (unsigned int)height);

    glSC(glEnable(GL_BLEND));
    glSC(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glSC(glClearColor(0.f, 0.f, 0.f, 1.f));

    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));//GL_MIRRORED_REPEAT
    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));//GL_MIRRORED_REPEAT

    glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//when downscaling
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
    Hierarchy->Delete();
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
void Window::SetCursorMode(CursorModes mode) {
    unsigned int glfwMode = GLFW_CURSOR_NORMAL;
    switch (mode) {
    case CursorModes::Free: glfwMode = GLFW_CURSOR_NORMAL; break;
    case CursorModes::LockedAndInvisible: glfwMode = GLFW_CURSOR_DISABLED; break;
    }
    glfwSetInputMode(WindowPtr, GLFW_CURSOR, glfwMode);
}
void Window::GetCursorMotionData(Vector2* pos, Vector2* delta) const {
    if (not MouseMotionAquirementHappenedInThisFrame) {
        unsigned int width, height;
        GetWindowSize(&width, &height);

        double tx, ty;
        glfwGetCursorPos(WindowPtr, &tx, &ty);

        Vector2 recordedPrevPos = LastMousePosition;

        LastMousePosition.sX((float)tx - (float)width / 2);
        LastMousePosition.sY((float)height / 2 - (float)ty);

        LastMouseDelta.sX(LastMousePosition.gX() - recordedPrevPos.gX());
        LastMouseDelta.sY(LastMousePosition.gY() - recordedPrevPos.gY());
    }

    if (pos!=nullptr)
        *pos = LastMousePosition;
    if (delta!=nullptr)
        *delta = LastMouseDelta;

    MouseMotionAquirementHappenedInThisFrame = true;
}
void Window::StartUpdatingWindow() {

    GetCursorMotionData(nullptr,nullptr);//just updates it

    //glSC(glClear(GL_COLOR_BUFFER_BIT));
    glSC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    if (UpdateWindowRenderingSizeAutomatically) {
        unsigned int width, height;
        GetWindowSize(&width, &height);
        UpdateWindowRenderingSize(width, height);
    }
}
void Window::EndUpdatingWindow() {
    glfwSwapBuffers(WindowPtr);
    glfwPollEvents();
    MouseMotionAquirementHappenedInThisFrame = false;
}