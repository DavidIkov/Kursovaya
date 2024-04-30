#pragma once
#include"Debug.h"

#include <GLFW/glfw3.h>

namespace WindowsManager {
#if defined _DEBUG
    inline bool glfwInitialized = false;
#endif
    bool Initialize();
    GLFWmonitor* getCurrentMonitor();
    const GLFWvidmode* getMonitorData(GLFWmonitor* monitor);
    void Uninitialize();
}


//does not support multi monitors(maybe) for now
class Window {
    GLFWwindow* WindowPtr;

public:
    bool UpdateWindowRenderingSizeAutomatically = true;
    //does not support "share" for now
    Window(int width, int height, const char* name, GLFWmonitor* monitor = nullptr);
    void SetWindowSize(unsigned int width, unsigned int height);
    void SetWindowColor(float r, float g, float b);
    void SetSwapInterval(unsigned int interval);
    bool WindowWaitingToBeClosed() const;
    void Destroy();
    void GetWindowSize(unsigned int* width, unsigned int* height) const;
    void UpdateWindowRenderingSize(unsigned int width, unsigned int height);
    void GetCursorPosition(float* x, float* y) const;
    void StartUpdatingWindow();
    void EndUpdatingWindow();
};