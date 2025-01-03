#pragma once

#include <glad/glad.h>
#include <iostream>
#include <algorithm>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "resource_manager.h"
#include "post_processing.h"
#include "camera.h"
#include "shader.h"
#include "lights.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    Renderer(int viewport_width, int viewport_height);
    ~Renderer();

    std::shared_ptr<Camera> m_Camera;
    std::shared_ptr<PostProcessing> m_PostProcessing;

    int m_ViewportWidth;
    int m_ViewportHeight;

    bool m_IsPostProcessing = false;

    glm::mat4 m_ProjectionMatrix;

    void setCurrentShader(std::shared_ptr<Shader> shader);
    void setCurrentStencilShader(std::shared_ptr<Shader> shader);

    void draw(bool swap_buffers = true);

    void togglePostProcessing();

private:
    std::shared_ptr<Shader> m_CurrentShader;
    std::shared_ptr<Shader> m_CurrentStencilShader;

    std::vector<std::shared_ptr<DirLight>>   m_DirLights;
    std::vector<std::shared_ptr<PointLight>> m_PointLights;
    std::vector<std::shared_ptr<SpotLight>>  m_SpotLights;
};