#include"IndexBuffer.h"

IndexBuffer::IndexBuffer() {
	glSC(glGenBuffers(1, &ID));
	glSC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));//////maybe GL_ARRAY_BUFFER..?
}

IndexBuffer::IndexBuffer(const std::vector<unsigned int>* const data, unsigned int useType) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->size() * sizeof(unsigned int), &((*data)[0]), useType);
}

void IndexBuffer::Bind() {
	glSC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
}

void IndexBuffer::Unbind() {
	glSC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::ModifyData(const std::vector<unsigned int>* const data) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glSC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->size() * sizeof(unsigned int), &((*data)[0]), GL_DYNAMIC_DRAW));
}