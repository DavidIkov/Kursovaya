#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
class Camera3D {
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance
public:
	static const std::type_index TypeIndex;
	Camera3D* gThis();
	DLL_TREATMENT Camera3D(ClassesMap* mapPtr);
};