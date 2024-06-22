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

enum class CollisionType { NONE, SELF, SQUARE, SPHERE };

class Collision {
public:

	// Funcs
	void makeCollision(const std::vector<glm::vec3>& m_vertices);
	void updateModelMatrix(const glm::mat4& model_matrix);

	// Constructor
	Collision(const std::vector<glm::vec3>& m_vertices, CollisionType type = CollisionType::NONE);

	// Copy constructor
	Collision(const Collision& obj);

	// Verts of collision (in local space)
	std::vector<glm::vec3> m_vertices;

	// Model matrix
	glm::mat4 m_model_matrix = glm::mat4(1.0f);

	// Type of collision
	CollisionType m_type;
private:
	void setVerts(const std::vector<glm::vec3>& m_vertices);
	void makeSquare(const std::vector<glm::vec3>& m_vertices);

};