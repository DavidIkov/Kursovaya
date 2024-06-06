#include"Texture.h"
#include"Dependencies/stb_image/stb_image.h"

Texture::Texture(const char* filePath) {
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* textureData = stbi_load(filePath, &width, &height, &nrChannels, 0);
	if (not textureData) __debugbreak();
	glSC(glGenTextures(1, &TextureID));
	glSC(glBindTexture(GL_TEXTURE_2D, TextureID));
	glSC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData));
	glSC(glGenerateMipmap(GL_TEXTURE_2D));
	stbi_image_free(textureData);
}

Texture::Texture(unsigned int width, unsigned int height) {
	glSC(glGenTextures(1, &TextureID));
	glSC(glBindTexture(GL_TEXTURE_2D, TextureID));
	glSC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));

	//glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//glSC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}

void Texture::Bind() const {
	glSC(glBindTexture(GL_TEXTURE_2D, TextureID));
}

void Texture::Delete() const {
	glSC(glDeleteTextures(1, &TextureID));
}

unsigned int Texture::gID() const {
	return TextureID;
}