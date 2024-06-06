#include"FrameBuffer.h"

void FrameBuffer::sDepthTest(bool state) {
	DepthTest = state;
}
bool FrameBuffer::gDepthTest() const {
	return DepthTest;
}

unsigned int FrameBuffer::gRenderBufferID() const {
	return RenderBufferID;
}
unsigned int FrameBuffer::gFrameBufferID() const {
	return FrameBufferID;
}

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) :vTexture(width, height) {
	glSC(glGenFramebuffers(1, &FrameBufferID));
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID));

	glSC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vTexture.gID(), 0));

	glSC(glGenRenderbuffers(1, &RenderBufferID));
	glSC(glBindRenderbuffer(GL_RENDERBUFFER, RenderBufferID));
	glSC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	glSC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBufferID)); // now actually attach it

	glViewport(0, 0, width, height);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FRAMEBUFFER ERROR";
#if defined _DEBUG
		__debugbreak();
#endif
	}
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

const Texture& FrameBuffer::gTexture() const {
	return vTexture;
}

void FrameBuffer::Bind() const {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID));
	glSC(glClearColor(0.f, 1.f, 1.f, 1.f));
	glSC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	//if (DepthTest) { glSC(glEnable(GL_DEPTH_TEST)) }
	//else { glSC(glDisable(GL_DEPTH_TEST)) };
}

void FrameBuffer::Unbind() {
	glSC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::Delete() {
	glSC(glDeleteFramebuffers(1, &FrameBufferID));
	glSC(glDeleteRenderbuffers(1, &RenderBufferID));
	vTexture.Delete();
}