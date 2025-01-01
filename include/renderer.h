#pragma once

#include <glad/glad.h>
#include <iostream>
#include <algorithm>
#include <vector>

#include "scene.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

// Singleton
class Renderer
{
private:
    Renderer();
    ~Renderer();

public:
    Renderer(const Renderer&) = delete;  // no copying
    Renderer(Renderer&&) = delete; // no moving
    Renderer& operator= (const Renderer&) = delete; // delete copy operator
    Renderer& operator= (Renderer&&) = delete; // delete move operator

    static Renderer& instance();

    bool isPostProcessing = false;

    void draw(std::shared_ptr<Scene> scene, bool swap_buffers = true);
};