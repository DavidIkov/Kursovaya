#pragma once
#include"DLLStuff.h"
#include"Debug.h"
#include"Texture.h"

class FrameBuffer {
	unsigned int FrameBufferID;
	unsigned int RenderBufferID;
	Texture vTexture;
	bool DepthTest = false;
public:

	DLL_TREATMENT void sDepthTest(bool state);
	DLL_TREATMENT bool gDepthTest() const;

	DLL_TREATMENT FrameBuffer(unsigned int width, unsigned int height);

	DLL_TREATMENT const Texture& gTexture() const;

	//should be called before drawing
	DLL_TREATMENT void Bind() const;

	DLL_TREATMENT unsigned int gRenderBufferID() const;
	DLL_TREATMENT unsigned int gFrameBufferID() const;

	//also binds default buffer
	DLL_TREATMENT static void Unbind();

	DLL_TREATMENT void Delete();
};