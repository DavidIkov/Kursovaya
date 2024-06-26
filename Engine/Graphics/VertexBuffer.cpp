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

void VertexBuffer::Unbind() {
	glSC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::ModifyData(const std::vector<float>* data) {
	glSC(glBindBuffer(GL_ARRAY_BUFFER, ID));
	glSC(glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), (data->size() > 0)?&((*data)[0]):nullptr, GL_DYNAMIC_DRAW));
}