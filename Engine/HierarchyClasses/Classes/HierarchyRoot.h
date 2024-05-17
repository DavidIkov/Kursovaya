#pragma once
#include"DLLStuff.h"

#include"../Modifiers/Instance.h"
#include"../Modifiers/WindowContainer.h"
#include"../Modifiers/TranslatorFrom2D.h"
#include"../Modifiers/Camera2D.h"
#include"../Modifiers/Renderer.h"

#include"../Modifiers/ClassesMap.h"
class HierarchyRoot :public ClassesMap, public Instance, public WindowContainer, public TranslatorFrom2D, public Camera2D, public Renderer {

public:
	const std::string ClassName = "HierarchyRoot";
	DLL_TREATMENT HierarchyRoot(Window* windPtr) :ClassesMap(ClassesMapClassDeleteFunction, CreateMap<Instance, WindowContainer, TranslatorFrom2D, Camera2D, Renderer>(nullptr, Instance::gThis(), WindowContainer::gThis(), TranslatorFrom2D::gThis(), Camera2D::gThis(), Renderer::gThis())), Instance(this), WindowContainer(this, windPtr), TranslatorFrom2D(this), Camera2D(this), Renderer(this) {

	}


};