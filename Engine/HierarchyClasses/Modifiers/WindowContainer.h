#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"

class Window;

class WindowContainer {
	ClassesMap* InstancePtr = nullptr;//"this" pointer from actual instance
	Window* WindowPtr = nullptr;

public:
	static const std::type_index TypeIndex;

	WindowContainer* gThis();
	DLL_TREATMENT WindowContainer(ClassesMap* mapPtr, Window* windPtr);

};