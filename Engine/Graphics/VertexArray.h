#pragma once
#include"DLLStuff.h"
#include"Debug.h"
class VertexArray {
	unsigned int ID;
public:

	//creates vertex array
	DLL_TREATMENT VertexArray();

	//binds vertex array
	DLL_TREATMENT void Bind();

	//unbinds vertex array
	DLL_TREATMENT static void Unbind();
};