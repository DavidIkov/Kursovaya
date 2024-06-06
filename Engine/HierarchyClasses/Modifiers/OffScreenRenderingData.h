#pragma once
#include"DLLStuff.h"
#include"ClassesMap.h"
#include"Graphics/FrameBuffer.h"
#include"Maths/Vector2.h"
class OffScreenRenderingData {
	friend class TranslatorFrom2D;
	friend class TranslatorFrom3D;
	friend class Renderer;
	ClassesMap* InstancePtr = nullptr;//"this" pointer from actual instance
	Vector2 Resolution;
	FrameBuffer vFrameBuffer;
public:
	static const std::type_index TypeIndex;
	OffScreenRenderingData* gThis();
	DLL_TREATMENT OffScreenRenderingData(ClassesMap* mapPtr, unsigned int width, unsigned int height);
};