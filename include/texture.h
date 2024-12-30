#pragma once

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include "renderer.h"

class Texture
{
public:
    GLuint m_ID;
    std::string m_Type;
    std::string m_Path;

    Texture(const std::string& path, std::string type = "");
};