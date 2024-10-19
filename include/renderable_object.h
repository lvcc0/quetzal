# pragma once

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

#include "shader.h"
#include "renderer.h"
#include "buffers_objects.h"
#include "vertex_array.h"

// Abstract class
class Renderable {
public:

	// Constructors
	Renderable(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices = std::vector<unsigned int>{});
	Renderable(const Renderable& obj);

	// Destructor
	virtual ~Renderable();

	std::vector<Vertex>      m_Vertices;
	std::vector<unsigned int> m_Indices;

	bool is_selected = false;

	virtual void draw(const Shaders_pack& shaders) = 0;

	virtual GLuint getVAO() const final;
	virtual glm::mat4 getModelMatrix() const;  // Sometimes model matrix could be identity matrix, so for getting true matrix we using this func

protected:
	std::unique_ptr<VBO> vbo_ptr{ nullptr };
	std::unique_ptr<IBO> ibo_ptr{ nullptr };
	std::unique_ptr<VAO> vao_ptr{ nullptr };

	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

	virtual void setupRender() = 0;
};