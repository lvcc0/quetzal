#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>
#include <concepts>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.h"
#include "renderer.h"
#include "buffers_objects.h"
#include "vertex_array.h"

// TODO: rename some stuff in here

// Group of base, abstract classes
// WARNING!!! ALL CLASSES HERE MUST HAVE UNIQUE NAMES OF THEIR VARIABLES AND METHODS

// Parent for classes which objects are used as node in scene GUI
class Scene_Node
{
public:
    // Constructors
    Scene_Node(std::string name);
    Scene_Node(const Scene_Node& obj);

    // Destructor
    virtual ~Scene_Node();

    inline const std::string getName() const noexcept { return m_Name; }
    inline const std::shared_ptr<Scene_Node> getParent() const noexcept { return m_Parent_node.lock(); }

    inline void setName(std::string name) { m_Name = name; }
    inline void setParent(const std::shared_ptr<Scene_Node>& parent) { m_Parent_node = parent; }

protected:
    std::string m_Name;
    std::weak_ptr<Scene_Node> m_Parent_node;
};

// Parent for classes which objects are used as objects in scene (They are orientated in space, have position, etc.)
class Scene_Object
{
public:
    // Constructors
    Scene_Object(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
    Scene_Object(const Scene_Object& obj);

    // Destructor
    virtual ~Scene_Object();

    // TODO: make this stuff protected
    // Abstract variables for using in world space
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);        // position in the world space
    glm::vec3 m_AbsolutePosition = glm::vec3(0.0f, 0.0f, 0.0f);  

    glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);           // scaling in the world space
    glm::vec3 m_AbsoluteScale = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 m_RotationDegrees = glm::vec3(0.0f, 0.0f, 0.0f); // rotation in all 3 axis respectively in the world space
    glm::vec3 m_AbsoluteRotationDegrees = glm::vec3(0.0f, 0.0f, 0.0f);

    inline virtual const glm::vec3 getPosition() const noexcept { return m_AbsolutePosition; }
    inline virtual const glm::vec3 getScale() const noexcept { return m_AbsoluteScale; }
    inline virtual const glm::vec3 getRotationDegrees() const noexcept { return m_AbsoluteRotationDegrees; }

protected:
    // TODO: make this stuff public
    // Use while drawing
    inline virtual void setPosition() {  }
    inline virtual void setScale() {  }
    inline virtual void setRotationDegrees(const glm::vec3 rotation) {  }
};

// Parent for classes which objects are used as something renderable
class Renderable
{
public:
    bool is_selected = false;

    // Constructors
    Renderable(std::vector<Vertex>& vertices, std::vector<unsigned int> indices = std::vector<unsigned int>{});
    Renderable(const Renderable& obj);

    // Destructor
    virtual ~Renderable();

    // TODO: do all renderable object really need vertices and indices?
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;

    virtual void draw(const Shaders_pack& shaders) = 0;

    inline virtual GLuint getVAO() const noexcept final { return this->vao_ptr->getID(); };
    inline virtual glm::mat4 getModelMatrix() const noexcept { return m_ModelMatrix; }  // Sometimes model matrix could be identity matrix, so for getting true matrix we using this func

protected:
    // At now model matrix updates every tick (not good)
    glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

    std::unique_ptr<VBO> vbo_ptr{ nullptr };
    std::unique_ptr<IBO> ibo_ptr{ nullptr };
    std::unique_ptr<VAO> vao_ptr{ nullptr };

    virtual void setupRender() = 0;
};
