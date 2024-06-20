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

#include "vertex.h"

enum class CollisionType { NONE, SQUARE, SPHERE };

class Collision {
public:

	void updateCollision(const std::vector<glm::vec3>& m_vertices);
	void makeCollision(CollisionType m_type, const std::vector<glm::vec3>& m_vertices);
	void setVerts(const std::vector<glm::vec3>& m_vertices);

	// Void constructor
	Collision();

	// Copy constructor
	Collision(const Collision& obj);

	// All verts
	std::vector<glm::vec3> m_vertices;

	// This vars should be using for NOT FULL checking of collision
	// Type
	CollisionType m_type;

	// Position
	glm::vec3 m_position;

	// Size
	glm::vec3 m_size;
};