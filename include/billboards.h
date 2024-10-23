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
#include "renderable_object.h"

// TODO: technically they're almost the same, so we can do something like inheritance here

// Abstract class (draw functuion which is pure virtual isnt declared here)
class Billboard : public Renderable
{
public:
    // Constructors
    Billboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture, std::vector<Vertex> verts, bool is_preload = false, std::string name = "");
    Billboard(const Billboard& obj);

    std::shared_ptr<Texture> m_Texture;

    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec2 m_Scale = glm::vec2(1.0f, 1.0f);
    glm::vec3 m_Target = glm::vec3(0.0f, 0.0f, 0.0f);

    // Moving in the world space
    virtual void translate(glm::vec3 vector);
    virtual void scale(glm::vec2 vector);

protected:

    virtual void setupRender() override;
};

class CylindricalBillboard : public Billboard
{
    // These vars using in draw and getModelMatrix
    glm::vec3 up = glm::vec3(0.0f);
    float angle_in_rad = 0.0f;

public:
    // Constructors
    CylindricalBillboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture, std::vector<Vertex> verts, bool is_preload = false, std::string name = "");
    CylindricalBillboard(const CylindricalBillboard& obj);

    // Draw billboard and change it's model matrix
    void draw(const Shaders_pack& shaders) override;

    virtual glm::mat4 getModelMatrix() const override;
};

class SphericalBillboard : public Billboard
{
    // These vars using in draw and getModelMatrix
    float vert_angle_in_rad = 0.0f;
    float hor_angle_in_rad = 0.0f;
    glm::vec3 up = glm::vec3(0.0f);
    glm::vec3 right = glm::vec3(0.0f);

public:
    // Constructors
    SphericalBillboard(glm::vec3 pos, glm::vec2 scale, std::shared_ptr<Texture>& texture, std::vector<Vertex> verts, bool is_preload = false, std::string name = "");
    SphericalBillboard(const SphericalBillboard& obj);

    // Draw billboard and change it's model matrix
    void draw(const Shaders_pack& shaders) override;

    virtual glm::mat4 getModelMatrix() const override;
};