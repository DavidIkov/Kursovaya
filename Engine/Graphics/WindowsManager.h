#pragma once
#include"DLLStuff.h"
#include"Debug.h"

#include "Dependencies/GLFW/include/GLFW/glfw3.h"

class HierarchyRoot;
namespace WindowsManager {
#if defined _DEBUG
    inline bool glfwInitialized = false;
#endif
    DLL_TREATMENT bool Initialize();
    DLL_TREATMENT GLFWmonitor* getCurrentMonitor();
    DLL_TREATMENT const GLFWvidmode* getMonitorData(GLFWmonitor* monitor);
    DLL_TREATMENT void Uninitialize();
}


//does not support multi monitors(maybe) for now
class Window {
    GLFWwindow* WindowPtr;
public:
    HierarchyRoot* Hierarchy;
    bool UpdateWindowRenderingSizeAutomatically = true;
    //does not support "share" for now
    DLL_TREATMENT Window(int width, int height, const char* name, GLFWmonitor* monitor = nullptr);
    DLL_TREATMENT void SetWindowSize(unsigned int width, unsigned int height);
    DLL_TREATMENT void SetWindowColor(float r, float g, float b);
    DLL_TREATMENT void SetSwapInterval(unsigned int interval);
    DLL_TREATMENT bool WindowWaitingToBeClosed() const;
    DLL_TREATMENT void Destroy();
    DLL_TREATMENT void GetWindowSize(unsigned int* width, unsigned int* height) const;
    DLL_TREATMENT void UpdateWindowRenderingSize(unsigned int width, unsigned int height);
    DLL_TREATMENT void GetCursorPosition(float* x, float* y) const;
    DLL_TREATMENT void StartUpdatingWindow();
    DLL_TREATMENT void EndUpdatingWindow();
};