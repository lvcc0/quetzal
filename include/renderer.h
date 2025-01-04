#pragma once

#include <glad/glad.h>
#include <iostream>
#include <algorithm>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene.h"

// TODO: move this stuff
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    // Delete all constructors
    Renderer() = delete;
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    glm::mat4 currentProjectionMatrix;
    std::shared_ptr<Shader> currentShader;
    std::shared_ptr<Shader> currentStencilShader;

    void draw(Scene& scene, bool swap_buffers = true);
};