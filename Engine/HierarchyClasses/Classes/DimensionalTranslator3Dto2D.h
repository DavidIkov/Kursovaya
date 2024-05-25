#pragma once
#include"DLLStuff.h"
#include"../Modifiers/ClassesMap.h"
#include"../Modifiers/Instance.h"
#include"../Modifiers/Object2DData.h"
#include"../Modifiers/TranslatorFrom3D.h"
class DimensionalTranslator3Dto2D :public ClassesMap, public Instance, public Object2DData, public TranslatorFrom3D {

public:
	const std::string ClassName = "DimensionalTranslator3Dto2D";
	DLL_TREATMENT DimensionalTranslator3Dto2D() :ClassesMap(ClassesMapClassDeleteFunction, CreateMap<Instance, Object2DData, TranslatorFrom3D>(nullptr, Instance::gThis(), Object2DData::gThis(), TranslatorFrom3D::gThis())), Instance(this), Object2DData(this), TranslatorFrom3D(this) {

	}


};