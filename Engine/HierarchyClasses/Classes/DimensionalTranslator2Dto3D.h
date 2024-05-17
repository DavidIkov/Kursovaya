#pragma once
#include"DLLStuff.h"
#include"../Modifiers/ClassesMap.h"
#include"../Modifiers/Instance.h"
#include"../Modifiers/Object2DData.h"
#include"../Modifiers/Camera3D.h"
class DimensionalTranslator2Dto3D :public ClassesMap, public Instance, public Object2DData, public Camera3D {

public:
	const std::string ClassName = "DimensionalTranslator2Dto3D";
	DLL_TREATMENT DimensionalTranslator2Dto3D() :ClassesMap(ClassesMapClassDeleteFunction, CreateMap<Instance, Object2DData, Camera3D>(nullptr, Instance::gThis(), Object2DData::gThis(), Camera3D::gThis())), Instance(this), Object2DData(this), Camera3D(this) {

	}


};