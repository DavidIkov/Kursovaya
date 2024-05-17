#include"VertexArray.h"

VertexArray::VertexArray() {
	glSC(glGenVertexArrays(1, &ID));
	glSC(glBindVertexArray(ID));
}

void VertexArray::Bind() {
	glSC(glBindVertexArray(ID));
}

void VertexArray::Unbind() {
	glSC(glBindVertexArray(0));
}