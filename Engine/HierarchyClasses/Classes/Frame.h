#pragma once
#include"DLLStuff.h"
#include"../Modifiers/Instance.h"
#include"../Modifiers/Object2DData.h"
#include"../Modifiers/ClassesMap.h"
class Frame :public ClassesMap, public Instance, public Object2DData {
public:
	const std::string ClassName = "Frame";
	DLL_TREATMENT Frame():ClassesMap(ClassesMapClassDeleteFunction, CreateMap<Instance, Object2DData>(nullptr, Instance::gThis(), Object2DData::gThis())), Instance(this), Object2DData(this) {
		
	}

};