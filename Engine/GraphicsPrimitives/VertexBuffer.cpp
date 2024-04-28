#include"VertexBuffer.h"

VertexBuffer::VertexBuffer() {
	glSC(glGenBuffers(1, &ID));
	glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
}

VertexBuffer::VertexBuffer(const std::vector<float>* data, unsigned int useType) {
	glSC(glGenBuffers(1, &ID));
	glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));

	glSC(glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), &((*data)[0]), useType));
}

void VertexBuffer::Bind() {
	glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
}

void VertexBuffer::ModifyData(const std::vector<float>* data) {
	glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
	glSC(glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), &((*data)[0]), GL_DYNAMIC_DRAW));
}