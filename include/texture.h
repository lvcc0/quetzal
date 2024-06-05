#pragma once
#include <string>
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture {
public:
    unsigned int ID;
    std::string Type;

    Texture(std::string type, unsigned char* image, const int& width, const int& height, const int& numComponents);
};