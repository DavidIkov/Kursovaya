#pragma once
#include<iostream>
#include"Dependencies/GLEW/include/GL/glew.h"
#if defined _DEBUG
#define glSC(func)  while (glGetError() != GL_NO_ERROR);\
func;\
{int error = glGetError();\
if (error != GL_NO_ERROR) {\
    std::cout << "Error: " << error << " in file: " << __FILE__ << " in call: " << #func << " at " << __LINE__ << " line" << "\n";\
    __debugbreak();\
}}
#else 
#define glSC(func) func
#endif