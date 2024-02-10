#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shader.h"

class Texture
{
public:
	GLuint ID;
	GLuint type;
	GLuint slot;

	Texture(const char* image, GLenum texType, GLenum texSlot, GLenum format, GLenum pixelType);

	void Bind();
	void Unbind();
	void Delete();
};

#endif