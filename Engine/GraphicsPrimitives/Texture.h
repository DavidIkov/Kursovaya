#pragma once
#include"Debug.h"
class Texture {
	unsigned int TextureID;
public:
	
	//read image with "stb_image" and make it a texture
	Texture(const char* filePath);
};