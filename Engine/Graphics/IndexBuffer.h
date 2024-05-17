#pragma once
#include"DLLStuff.h"
#include<vector>
#include"Debug.h"
class IndexBuffer {
	unsigned int ID;
public:
	//create empty index buffer
	DLL_TREATMENT IndexBuffer();

	//create and fill index buffer
	DLL_TREATMENT IndexBuffer(const std::vector<unsigned int>* const data, unsigned int useType);

	//bind buffer
	DLL_TREATMENT void Bind();

	//unbind buffer
	DLL_TREATMENT static void Unbind();

	/*modifies buffer data, also binding buffer before changing its data so you can call it whenever you want, 
	uses GL_DYNAMIC_DRAW as buffer use type becouse if you changing buffer data then it should be dynamic by logic*/
	DLL_TREATMENT void ModifyData(const std::vector<unsigned int>* const data);
};