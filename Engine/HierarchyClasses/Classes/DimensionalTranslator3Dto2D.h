#pragma once
#include"DLLStuff.h"
#include"../Modifiers/ClassesMap.h"
#include"../Modifiers/Instance.h"
#include"../Modifiers/Object2DData.h"
#include"../Modifiers/TranslatorFrom3D.h"
#include"../Modifiers/OffScreenRenderingData.h"
class DimensionalTranslator3Dto2D :public ClassesMap, public Instance, public OffScreenRenderingData, public Object2DData, public TranslatorFrom3D {

public:
	const std::string ClassName = "DimensionalTranslator3Dto2D";
	DLL_TREATMENT DimensionalTranslator3Dto2D(unsigned int resolutionWidth, unsigned int resolutionHeight)
		:ClassesMap(ClassesMapClassDeleteFunction, CreateMap<Instance, OffScreenRenderingData, Object2DData, TranslatorFrom3D>(nullptr, Instance::gThis(), OffScreenRenderingData::gThis(), Object2DData::gThis(), TranslatorFrom3D::gThis())),
		Instance(this), OffScreenRenderingData(this, resolutionWidth, resolutionHeight), Object2DData(this), TranslatorFrom3D(this) {

	}


};