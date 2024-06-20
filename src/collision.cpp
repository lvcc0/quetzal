#include "collision.h"


void Collision::updateCollision(const std::vector<glm::vec3>& m_vertices)
{
	makeCollision(m_type, m_vertices);
}

void Collision::makeCollision(CollisionType m_type, const std::vector<glm::vec3>& m_vertices) // m_vertices in local space of object
{
	this->m_type = m_type;

	// Working only with orthogonal object
	if (m_type == CollisionType::SQUARE) {
		glm::vec3 less_coords_corner;
		bool first_iteration = true;
		for (auto i : m_vertices)
		{
			if (first_iteration || i.x <= less_coords_corner.x && i.y <= less_coords_corner.y && i.z <= less_coords_corner.z) {
				less_coords_corner = i;
			}
			first_iteration = false;
		}
		m_position = less_coords_corner;

		for (auto i : m_vertices)
		{
			glm::vec3 next_corner = i;
			if (next_corner.x == m_position.x && next_corner.y == m_position.y) {
				m_size.z = next_corner.z - m_position.z;
			}
			else if (next_corner.x == m_position.x && next_corner.z == m_position.z) {
				m_size.y = next_corner.y - m_position.y;
			}
			else if (next_corner.y == m_position.y && next_corner.z == m_position.z) {
				m_size.x = next_corner.x - m_position.x;
			}
		}
	}
	else if (m_type == CollisionType::NONE) {
		// Nothing
	}
	else {
		std::cerr << "ERROR::COLLISION_CONSTRUCTOR::COLLISION TYPE NOT FOUND" << std::endl;
	}
}

void Collision::setVerts(const std::vector<glm::vec3>& m_vertices)
{
	this->m_vertices = m_vertices;
}

Collision::Collision()
{
	//Free
}

Collision::Collision(const Collision& obj)
{
	this->m_type = obj.m_type;
	this->m_position = obj.m_position;
	this->m_size = obj.m_size;
}