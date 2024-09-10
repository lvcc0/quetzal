#include "texture.h"

Texture::Texture(unsigned char* image, int width, int height, int numComponents, std::string type)
    : m_type(type)
{
    GLenum format{};

    if (numComponents == 1)
        format = GL_RED;
    else if (numComponents == 3)
        format = GL_RGB;
    else if (numComponents == 4)
        format = GL_RGBA;

    GLCall(glGenTextures(1, &ID));
    GLCall(glBindTexture(GL_TEXTURE_2D, ID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
