#pragma once
#include<vector>
#include"Debug.h"
class VertexBuffer {
	unsigned int ID;
public:
	
	//creates empty vertex buffer
	VertexBuffer();

	//create vertex buffer and fills its data
	VertexBuffer(const std::vector<float>* data, unsigned int useType);

	//bind vertex buffer
	void Bind();

	//unbind vertex buffer
	static void Unbind() {
		glSC(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	/*modifies buffer data, also binding buffer before changing its data so you can call it whenever you want,
	uses GL_DYNAMIC_DRAW as buffer use type becouse if you changing buffer data then it should be dynamic by logic*/
	void ModifyData(const std::vector<float>* data);
};