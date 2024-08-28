#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <algorithm>

#include "texture.h"
#include "renderable_object.h"

class Model : public Renderable
{
public:
    float m_StencilScaling = 1.4f;

    std::vector<std::shared_ptr<Texture>> m_Textures;

    // Constructor
    Model(std::vector<Vertex>& vertices,
          std::vector<unsigned int>& indices,
          std::vector<std::shared_ptr<Texture>>& textures);

    Model(const Model& obj);

    // Draw model
    virtual void draw(std::vector<std::shared_ptr<Shader>>& shader_vector) override;

    // Abstract variables for using in world space
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);        // position in the world space
    glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);           // scaling in the world space
    glm::vec3 m_RotationDegrees = glm::vec3(0.0f, 0.0f, 0.0f); // rotation in all 3 axes respectively in the world space

    // Moving in world space
    virtual void translate(glm::vec3 vector);
    virtual void scale(glm::vec3 vector);
    virtual void rotate(float degrees, glm::vec3 vector);

    virtual glm::mat4 getModelMatrix() override;
protected:

    // Setup VAO, VBO, EBO
    void setupRender() override;
};