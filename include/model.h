#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <algorithm>

#include "texture.h"
#include "base_classes.h"

class Model : public Renderable, public Scene_Node, public Scene_Object
{
public:
    float m_StencilScaling = 1.4f;

    std::vector<std::shared_ptr<Texture>> m_Textures;

    // Constructor
    Model(std::vector<Vertex>& vertices,
        std::vector<unsigned int>& indices,
        std::vector<std::shared_ptr<Texture>>& textures,
        bool is_preload = false, std::string name = "");

    Model(const Model& obj);

    // Draw model
    virtual void draw(const Shaders_pack& shaders) override;

    virtual glm::mat4 getModelMatrix() const noexcept override;
protected:
    // Moving in world space
    inline virtual void setPosition(const glm::vec3 pos) override;
    inline virtual void setScale(const glm::vec3 scale) override;
    inline virtual void setRotationDegrees(const glm::vec3 rotation, float degrees) override;

    // Setup VAO, VBO, EBO
    void setupRender() override;
};