# pragma once
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <type_traits>
#include <concepts>
#include "shader.h"
#include "renderer.h"
#include "buffers_objects.h"
#include "vertex_array.h"

// Group of base, abstract classes
// WARNING!!! ALL CLASSES HERE MUST HAVE UNIQUE NAMES OF THEIR VARIABLES AND METHODS

// Parent for classes which objects are used as objects in scene (They are orientated in space, have position, etc.)
class Scene_Object {

public:
	// Constructors
	Scene_Object(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
	Scene_Object(const Scene_Object& obj);
	// Destructor
	virtual ~Scene_Object();

public:
	// Abstract variables for using in world space
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);        // position in the world space
	glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);           // scaling in the world space
	glm::vec3 m_RotationDegrees = glm::vec3(0.0f, 0.0f, 0.0f); // rotation in all 3 axis respectively in the world space

	inline virtual void setPosition(const glm::vec3 pos) { m_Position = pos; }
	inline virtual void setScale(const glm::vec3 scale) { m_Scale = scale; }
	inline virtual void setRotationDegrees(const glm::vec3 rotation, float degrees) { m_RotationDegrees = rotation; }
	inline virtual const glm::vec3 getPosition() const { return m_Position; }
	inline virtual const glm::vec3 getScale() const { return m_Scale; }
	inline virtual const glm::vec3 getRotationDegrees() const { return m_RotationDegrees; }
};

// Parent for classes which objects are used as node in scene GUI
class Scene_Node {
public:
	// Constructor
	Scene_Node(std::string name);
	Scene_Node(const Scene_Node& obj);
	// Destructor
	virtual ~Scene_Node();

protected:
	std::string m_Name;
	std::weak_ptr<Scene_Node> m_Parent_node;

public:
	inline const std::string getName() const { return m_Name; }
	inline const std::shared_ptr<Scene_Node> getParent() const { return m_Parent_node.lock(); }
	inline void setName(std::string name) { m_Name = name; }
	inline void setParent(const std::shared_ptr<Scene_Node>& parent) { m_Parent_node = parent; }
};

// Parent for classes which objects are used as something renderable
class Renderable {
public:

	// Constructors
	Renderable(std::vector<Vertex>& vertices, std::vector<unsigned int> indices = std::vector<unsigned int>{});
	Renderable(const Renderable& obj);

	// Destructor
	virtual ~Renderable();

	std::vector<Vertex>      m_Vertices;
	std::vector<unsigned int> m_Indices;

	bool is_selected = false;

	virtual void draw(const Shaders_pack& shaders) = 0;

	inline virtual GLuint getVAO() const final { return this->vao_ptr->getID(); };
	inline virtual glm::mat4 getModelMatrix() const { return m_ModelMatrix; }  // Sometimes model matrix could be identity matrix, so for getting true matrix we using this func

protected:
	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

	std::unique_ptr<VBO> vbo_ptr{ nullptr };
	std::unique_ptr<IBO> ibo_ptr{ nullptr };
	std::unique_ptr<VAO> vao_ptr{ nullptr };

	virtual void setupRender() = 0;
};
