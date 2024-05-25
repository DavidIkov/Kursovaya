#pragma once
#include"DLLStuff.h"
#include"../Modifiers/Instance.h"
#include"../Modifiers/Object3DData.h"
#include"../Modifiers/ClassesMap.h"
class Object3D :public ClassesMap, public Instance, public Object3DData {
public:
	const std::string ClassName = "Object3D";
	DLL_TREATMENT Object3D() :ClassesMap(ClassesMapClassDeleteFunction, CreateMap<Instance, Object3DData>(nullptr, Instance::gThis(), Object3DData::gThis())), Instance(this), Object3DData(this) {

	}

};