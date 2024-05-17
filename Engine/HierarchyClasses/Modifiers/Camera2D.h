#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
#include"Maths/SPCS.h"
class Camera2D {
	ClassesMap* instancePtr = nullptr;//"this" pointer from actual instance
public:
	static const std::type_index TypeIndex;
	Camera2D* gThis();
	DLL_TREATMENT Camera2D(ClassesMap* mapPtr);
	SPCS Position = SPCS(0, 0, 0, 0);
	SPCS Size = SPCS(1, 1, 0, 0);
	float Rotation = 0.f;
};