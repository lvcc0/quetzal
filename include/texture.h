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
    std::string m_path;

    Texture(unsigned char* image, int width, int height, int numComponents, std::string type = "");  
    Texture() { /* empty */ };
};