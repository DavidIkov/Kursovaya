#pragma once
#include"DLLStuff.h"
#include"Debug.h"
#include"InputController.h"
#include"vector"
#include"Maths/Vector2.h"

#include "Dependencies/GLFW/include/GLFW/glfw3.h"

class HierarchyRoot;


enum class CursorModes {
    Free,
    LockedAndInvisible,
};

//does not support multi monitors(maybe) for now
class Window {
    GLFWwindow* WindowPtr;

    mutable Vector2 LastMousePosition;
    mutable Vector2 LastMouseDelta;
    mutable bool MouseMotionAquirementHappenedInThisFrame = false;
public:
    DLL_TREATMENT GLFWwindow* gWindowPtr() const;

    InputController Keyboard;



    HierarchyRoot* Hierarchy;
    bool UpdateWindowRenderingSizeAutomatically = true;
    //does not support "share" for now
    DLL_TREATMENT Window(int width, int height, const char* name, GLFWmonitor* monitor = nullptr);
    DLL_TREATMENT ~Window();
    DLL_TREATMENT void SetWindowSize(unsigned int width, unsigned int height);
    DLL_TREATMENT void SetWindowColor(float r, float g, float b);
    DLL_TREATMENT void SetSwapInterval(unsigned int interval);
    DLL_TREATMENT bool WindowWaitingToBeClosed() const;
    DLL_TREATMENT void Destroy();
    DLL_TREATMENT void GetWindowSize(unsigned int* width, unsigned int* height) const;
    DLL_TREATMENT void UpdateWindowRenderingSize(unsigned int width, unsigned int height);
    DLL_TREATMENT void GetCursorMotionData(Vector2* pos, Vector2* delta) const;
    DLL_TREATMENT void SetCursorMode(CursorModes mode);
    DLL_TREATMENT void StartUpdatingWindow();
    DLL_TREATMENT void EndUpdatingWindow();
};

namespace WindowsManager {
    inline std::vector<Window*> Windows;
#if defined _DEBUG
    inline bool glfwInitialized = false;
#endif
    DLL_TREATMENT bool Initialize();
    DLL_TREATMENT GLFWmonitor* gCurrentMonitor();
    DLL_TREATMENT const GLFWvidmode* gMonitorData(GLFWmonitor* monitor);
    DLL_TREATMENT void Uninitialize();
}