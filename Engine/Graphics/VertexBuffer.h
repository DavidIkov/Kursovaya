#pragma once
#include"DLLStuff.h"
#include<vector>
#include"Debug.h"
class VertexBuffer {
	unsigned int ID;
public:
	
	//creates empty vertex buffer
	DLL_TREATMENT VertexBuffer();

	//create vertex buffer and fills its data
	DLL_TREATMENT VertexBuffer(const std::vector<float>* data, unsigned int useType);

	//bind vertex buffer
	DLL_TREATMENT void Bind();

	//unbind vertex buffer
	DLL_TREATMENT static void Unbind();

	/*modifies buffer data, also binding buffer before changing its data so you can call it whenever you want,
	uses GL_DYNAMIC_DRAW as buffer use type becouse if you changing buffer data then it should be dynamic by logic*/
	DLL_TREATMENT void ModifyData(const std::vector<float>* data);
};