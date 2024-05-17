#pragma once
#include"DLLStuff.h"
#include"Debug.h"
#include<string>
#include"BufferLayout.h"
class Shader {
	unsigned int ID;
public:
	//compile shader
	DLL_TREATMENT Shader(unsigned int type, const std::string& filepath);

	DLL_TREATMENT unsigned int GetID() const;

	//delete shader
	DLL_TREATMENT void Delete();
};