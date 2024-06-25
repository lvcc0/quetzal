#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
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

class Model
{
public:
    glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<std::shared_ptr<Texture>> m_Textures;

    // Constructor
    Model(std::vector<Vertex>& vertices,
          std::vector<unsigned int>& indices,
          std::vector<std::shared_ptr<Texture>>& textures);
    
    // Copy constructor
    Model(const Model& obj);

    // Destructor
    ~Model();

    // Draw model
    void draw(std::shared_ptr<Shader>& shader);

    // Moving in world space
    void translate(glm::vec3 vector);
    void scale(glm::vec3 vector);
    void rotate(float degrees, glm::vec3 vector);

private:
    GLuint VAO, VBO, EBO;

    // Setup VAO, VBO, EBO
    void setupModel();
};