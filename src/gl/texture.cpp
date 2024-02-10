#include "texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum texSlot, GLenum format, GLenum pixelType)
{
	stbi_set_flip_vertically_on_load(true);

	type = texType;
	slot = texSlot;

	int imgW, imgH, numColCh;
	unsigned char* bytes = stbi_load(image, &imgW, &imgH, &numColCh, 0);

	glGenTextures(1, &ID);

	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(texType, 0, GL_RGBA, imgW, imgH, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::Bind()
{
	glActiveTexture(slot);
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}