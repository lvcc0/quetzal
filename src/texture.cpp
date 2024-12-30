#include "texture.h"

Texture::Texture(const std::string& path, std::string type)
    : m_Path(path), m_Type(type)
{
    GLCall(glGenTextures(1, &m_ID));
    
    int width, height, numComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &numComponents, 0);

    if (data)
    {
        GLenum format{};

        if (numComponents == 1)
            format = GL_RED;
        else if (numComponents == 3)
            format = GL_RGB;
        else if (numComponents == 4)
            format = GL_RGBA;

        GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
    else
        std::cerr << "TEXTURE FAILED TO LOAD AT PATH:: " << path << std::endl;

    stbi_image_free(data);
}