#include"Object3DData.h"

const std::type_index Object3DData::TypeIndex = std::type_index(typeid(Object3DData));

Object3DData* Object3DData::gThis() {
	return this;
}
Object3DData::Object3DData(ClassesMap* mapPtr) {
	instancePtr = mapPtr;
}