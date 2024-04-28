#pragma once
#include"Debug.h"
#include<string>
#include"BufferLayout.h"
class Shader {
	unsigned int ID;
public:
	//compile shader
	Shader(unsigned int type, const std::string& filepath);

	unsigned int GetID() const;

	//delete shader
	void Delete();
};