#include"OffScreenRenderingData.h"

const std::type_index OffScreenRenderingData::TypeIndex = std::type_index(typeid(OffScreenRenderingData));

OffScreenRenderingData* OffScreenRenderingData::gThis() {
	return this;
}
OffScreenRenderingData::OffScreenRenderingData(ClassesMap* mapPtr, unsigned int width, unsigned int height)
:Resolution((float)width,(float)height),vFrameBuffer(width,height){
	InstancePtr = mapPtr;
}