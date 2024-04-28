#pragma once
#include"../Modifiers/Instance.h"
#include"../Modifiers/Renderer2D.h"
#include"../Modifiers/ClassesMap.h"
class Program :public ClassesMap, public Instance,public Renderer2D {
	
public:
	const std::string ClassName = "Program";
	void DeleteInstance() { delete this; };
	Program() :ClassesMap(ClassesMapClassDeleteFunction, CreateMap<Instance, Renderer2D>(nullptr, Instance::gThis(), Renderer2D::gThis())), Instance(this), Renderer2D(this) {
		
	}
	

};