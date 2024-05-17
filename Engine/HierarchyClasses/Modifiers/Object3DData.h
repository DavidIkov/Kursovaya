#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
class Object3DData {
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance
public:
	static const std::type_index TypeIndex;
	Object3DData* gThis();
	DLL_TREATMENT Object3DData(ClassesMap* mapPtr);
};