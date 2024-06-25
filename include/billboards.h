#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.h"
#include "texture.h"
#include "vertex.h"

// TODO: technically they're almost the same, so we can do something like inheritance here

class CylindricalBillboard
{
public:
    std::vector<Vertex>      m_Vertices;
    std::shared_ptr<Texture> m_Texture;

    glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec2 m_Scale = glm::vec2(1.0f, 1.0f);
    
    // Constructor
    CylindricalBillboard(glm::vec3 pos, glm::vec2 size, std::shared_ptr<Texture>& texture);

    // Destructor
    ~CylindricalBillboard();

    // Draw billboard and change it's model matrix
    void draw(std::shared_ptr<Shader>& shader, glm::vec3 object_pos);

    // Moving in the world space
    void translate(glm::vec3 vector);
    void scale(glm::vec2 vector);

private:
    GLuint VAO, VBO;

    // Setup VAO, VBO
    void setupBillboard();
};

class SphericalBillboard
{
public:
    std::vector<Vertex>      m_Vertices;
    std::shared_ptr<Texture> m_Texture;

    glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

    glm::vec3 m_Position;
    glm::vec2 m_Scale;

    // Constructor
    SphericalBillboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture);

    // Destructor
    ~SphericalBillboard();

    // Draw billboard and change it's model matrix
    void draw(std::shared_ptr<Shader>& shader, glm::vec3 target_position);

    // Moving in the world space
    void translate(glm::vec3 vector);
    void scale(glm::vec2 vector);

private:
    GLuint VAO, VBO;

    // Setup VAO, VBO
    void setupBillboard();
};