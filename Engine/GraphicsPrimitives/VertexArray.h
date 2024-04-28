#pragma once
#include"Debug.h"
class VertexArray {
	unsigned int ID;
public:

	//creates vertex array
	VertexArray();

	//binds vertex array
	void Bind();

	//unbinds vertex array
	static void Unbind() {
		glSC(glBindVertexArray(0));
	}
};