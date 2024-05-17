#pragma once
#include"DLLStuff.h"
#include"Debug.h"
class Texture {
	unsigned int TextureID;
public:
	
	//read image with "stb_image" and make it a texture
	DLL_TREATMENT Texture(const char* filePath);
};