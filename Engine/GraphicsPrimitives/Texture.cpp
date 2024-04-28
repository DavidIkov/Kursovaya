#include"Texture.h"
#include"stb_image/stb_image.h"

Texture::Texture(const char* filePath) {
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* textureData = stbi_load(filePath, &width, &height, &nrChannels, 0);
	if (not textureData) __debugbreak();
	glSC(glGenTextures(1, &TextureID));
	glSC(glBindTexture(GL_TEXTURE_2D, TextureID));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(textureData);
}