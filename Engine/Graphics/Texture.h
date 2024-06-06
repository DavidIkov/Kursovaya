#pragma once
#include"DLLStuff.h"
#include"Debug.h"
class Texture {
	unsigned int TextureID;
public:
	
	//read image with "stb_image" and make it a texture
	DLL_TREATMENT Texture(const char* filePath);
	
	//creates empty texture with specified size
	DLL_TREATMENT Texture(unsigned int width, unsigned int height);

	DLL_TREATMENT void Delete() const;

	DLL_TREATMENT void Bind() const;

	DLL_TREATMENT unsigned int gID() const;
};