#pragma once
#include"DLLStuff.h"

#include"../Modifiers/ClassesMap.h"
#include"../Modifiers/Instance.h"
#include"../Modifiers/WindowContainer.h"
#include"../Modifiers/TranslatorFrom2D.h"
#include"../Modifiers/Renderer.h"
#include"../Modifiers/OffScreenRenderingData.h"

class HierarchyRoot :public ClassesMap, public Instance, public WindowContainer, public OffScreenRenderingData, public TranslatorFrom2D, public Renderer {

public:
	const std::string ClassName = "HierarchyRoot";
	DLL_TREATMENT HierarchyRoot(Window* windPtr, unsigned int resolutionWidth, unsigned int resolutionHeight)
		:ClassesMap(ClassesMapClassDeleteFunction, CreateMap<Instance, WindowContainer, OffScreenRenderingData, TranslatorFrom2D, Renderer>(nullptr, Instance::gThis(), WindowContainer::gThis(), OffScreenRenderingData::gThis(), TranslatorFrom2D::gThis(), Renderer::gThis())),
		Instance(this), WindowContainer(this, windPtr), OffScreenRenderingData(this, resolutionWidth, resolutionHeight), TranslatorFrom2D(this), Renderer(this){

	}


};