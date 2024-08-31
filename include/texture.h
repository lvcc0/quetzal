#pragma once

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.h"

class Texture
{
public:
    GLuint ID;
    std::string m_type;

    // Constructor
    Texture(std::string type, unsigned char* image, int width, int height, int numComponents);
};