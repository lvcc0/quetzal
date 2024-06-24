#include "collision.h"
#include "collision.h"
#include "collision.h"

// Makes collision in local space
void Collision::makeCollision(CollisionType type, const std::vector<glm::vec3>& m_vertices)
{
	this->m_type = type;

	if (m_type == CollisionType::SQUARE) {
		makeSquare(m_vertices);
	}
	else if (m_type == CollisionType::SELF) {
		setVerts(m_vertices);
	}
	else if (m_type == CollisionType::NONE) {
		if (this->m_vertices.size() > 0)
			this->m_vertices.clear();
	}
	else {
		std::cerr << "ERROR::COLLISION_MAKER::COLLISION TYPE NOT FOUND" << std::endl;
	}
}

void Collision::updateModelMatrix(const glm::mat4& model_matrix)
{
	this->m_model_matrix = model_matrix;
}

void Collision::setVerts(const std::vector<glm::vec3>& m_vertices)
{
	this->m_vertices = m_vertices;
}

void Collision::makeSquare(const std::vector<glm::vec3>& m_vertices)
{
	glm::vec3 min_coords, max_coords;
	bool first_iteration = true;
	for (auto item : m_vertices) {
		if (first_iteration) {
			min_coords = item;
			max_coords = item;
			first_iteration = false;
		}
		else {
			if (item.x < min_coords.x)
				min_coords.x = item.x;
			if (item.y < min_coords.y)
				min_coords.y = item.y;
			if (item.z < min_coords.z)
				min_coords.z = item.z;
			if (item.x > max_coords.x)
				max_coords.x = item.x;
			if (item.y > max_coords.y)
				max_coords.y = item.y;
			if (item.z > max_coords.z)
				max_coords.z = item.z;
		}
	}

	this->m_vertices.push_back(min_coords); // less coords corner
	this->m_vertices.push_back(max_coords); // more coords corner
	this->m_vertices.push_back(min_coords + glm::vec3(max_coords.x - min_coords.x, 0, 0));
	this->m_vertices.push_back(min_coords + glm::vec3(0, max_coords.y - min_coords.y, 0));
	this->m_vertices.push_back(min_coords + glm::vec3(0, 0, max_coords.z - min_coords.z));
	this->m_vertices.push_back(max_coords - glm::vec3(max_coords.x - min_coords.x, 0, 0));
	this->m_vertices.push_back(max_coords - glm::vec3(0, max_coords.y - min_coords.y, 0));
	this->m_vertices.push_back(max_coords - glm::vec3(0, 0, max_coords.z - min_coords.z));

}



Collision::Collision()
{
	// Nothing //
}

// Makes collision in local space
Collision::Collision(CollisionType type, const std::vector<glm::vec3>& m_vertices)
{
	makeCollision(type, m_vertices);
}

Collision::Collision(CollisionType type, const std::vector<Vertex>& m_vertices)
{
	makeCollision(type, ExpMath::returnPositionFromVertex(m_vertices));
}

Collision::Collision(const Collision& obj)
{
	this->m_type = m_type;
	this->m_vertices = obj.m_vertices;
	this->m_model_matrix = glm::mat4(1.0f);
}