#include"BufferLayout.h"

BufferLayout::BufferLayout(unsigned int useType, unsigned int size) {
	UseType = useType;
	SizeOfOneElement = size;
}

void BufferLayout::AddData(unsigned int len) {
	Lengths.push_back(len);
	SumOfLengths += len;
}

void BufferLayout::Bind() const {
	unsigned int offset = 0;
	for (unsigned int i = 0; i < Lengths.size(); i++) {
		glSC(glEnableVertexAttribArray(i));
		glSC(glVertexAttribPointer(i, Lengths[i], UseType, GL_FALSE, SumOfLengths * SizeOfOneElement, (const void*)offset));
		offset += Lengths[i] * SizeOfOneElement;

	}
}

void BufferLayout::Unbind() const {
	for (unsigned int i = 0; i < Lengths.size(); i++) {
		glSC(glDisableVertexAttribArray(i));
	}
}