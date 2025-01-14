#include "scene/resources/texture.h"

namespace qtzl
{
	Texture::Texture(const std::string& name, const std::string& path, const std::string& type)
		: Resource(name, path), m_Type(type)
	{
		glGenTextures(1, &this->ID);

		int width, height, componentsNum;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &componentsNum, 0);
		
		if (data)
		{
			GLenum format{};
		
			if (componentsNum == 1)
				format = GL_RED;
			else if (componentsNum == 3)
				format = GL_RGB;
			else if (componentsNum == 4)
				format = GL_RGBA;
		
			glBindTexture(GL_TEXTURE_2D, this->ID);
		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
			std::cerr << "TEXTURE FAILED TO LOAD AT PATH:: " << path << std::endl;
		
		stbi_image_free(data);
	}

	GLuint Texture::getID() const
	{
		return this->ID;
	}

	std::string Texture::getType() const
	{
		return this->m_Type;
	}
}