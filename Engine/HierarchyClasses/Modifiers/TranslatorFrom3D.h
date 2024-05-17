#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
class TranslatorFrom3D {
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance
public:
	static const std::type_index TypeIndex;
	TranslatorFrom3D* gThis();
	DLL_TREATMENT TranslatorFrom3D(ClassesMap* mapPtr);
};