#pragma once
#include"../Modifiers/Instance.h"
#include"../Modifiers/Instance2DData.h"
#include"../Modifiers/ClassesMap.h"
class Frame :public ClassesMap, public Instance, public Instance2DData {
public:
	const std::string ClassName = "Frame";
	void DeleteInstance() { delete this; };
	Frame():ClassesMap(ClassesMapClassDeleteFunction, CreateMap<Instance, Instance2DData>(nullptr, Instance::gThis(), Instance2DData::gThis())), Instance(this), Instance2DData(this) {
		
	}

};