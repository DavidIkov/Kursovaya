#pragma once
#include<vector>
#include"Debug.h"
class IndexBuffer {
	unsigned int ID;
public:
	//create empty index buffer
	IndexBuffer();

	//create and fill index buffer
	IndexBuffer(const std::vector<unsigned int>* const data, unsigned int useType);

	//bind buffer
	void Bind();

	//unbind buffer
	static void Unbind() {
		glSC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	/*modifies buffer data, also binding buffer before changing its data so you can call it whenever you want, 
	uses GL_DYNAMIC_DRAW as buffer use type becouse if you changing buffer data then it should be dynamic by logic*/
	void ModifyData(const std::vector<unsigned int>* const data);
};