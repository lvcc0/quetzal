#include "physics.h"
#include "physics.h"
#include "physics.h"
#include "physics.h"
#include "physics.h"
#include "physics.h"

bool Physics::checkCollision(Collision& object1, Collision& object2)
{
	if (object1.m_type == CollisionType::SQUARE && object2.m_type == CollisionType::SQUARE) {
		bool CollisionX = object1.m_position.x + object1.m_size.x >= object2.m_position.x && object2.m_position.x + object2.m_size.x >= object1.m_position.x;
		bool CollisionY = object1.m_position.y + object1.m_size.y >= object2.m_position.y && object2.m_position.y + object2.m_size.y >= object1.m_position.y;
		bool CollisionZ = object1.m_position.z + object1.m_size.z >= object2.m_position.z && object2.m_position.z + object2.m_size.z >= object1.m_position.z;
		return CollisionX && CollisionY && CollisionZ;
	}
	else if (object1.m_type == CollisionType::SQUARE && object2.m_type == CollisionType::SPHERE) {
		//PLACEHOLDER//
	}
	else if (object1.m_type == CollisionType::SPHERE && object2.m_type == CollisionType::SQUARE) {
		//PLACEHOLDER//
	}
	else if (object1.m_type == CollisionType::SPHERE && object2.m_type == CollisionType::SPHERE) {
		//PLACEHOLDER//
	}
	else if (object1.m_type == CollisionType::NONE || object2.m_type == CollisionType::NONE) {
		//PLACEHOLDER//
	}
	else {
		std::cout << "ERROR::INCORRECT COLLISION TYPE" << std::endl;
	}
	return false;
}

bool Physics::fullCheckCollision(Collision& object1, Collision& object2)
{
	// using as vectors of bools
	glm::vec3 first_less_second(false, false, false);
	glm::vec3 first_more_second(false, false, false);

	std::vector<glm::vec3> first_verts = object1.m_vertices;
	std::vector<glm::vec3> second_verts = object2.m_vertices;

	for (glm::vec3 first_item : first_verts) {
		for (glm::vec3 second_item : second_verts) {
			if (first_item.x > second_item.x)
				first_more_second.x = true;
			if (first_item.y > second_item.y)
				first_more_second.y = true;
			if (first_item.z > second_item.z)
				first_more_second.z = true;
			if (first_item.x < second_item.x)
				first_less_second.x = true;
			if (first_item.y < second_item.y)
				first_less_second.y = true;
			if (first_item.z < second_item.z)
				first_less_second.z = true;
			if (first_item.x == second_item.x) {
				first_more_second.x = true;
				first_less_second.x = true;
			}
			if (first_item.y == second_item.y) {
				first_more_second.y = true;
				first_less_second.y = true;
			}
			if (first_item.z == second_item.z) {
				first_more_second.z = true;
				first_less_second.z = true;
			}
		}
	}
	glm::vec3 result;
	result.x = first_less_second.x && first_more_second.x;
	result.y = first_less_second.y && first_more_second.y;
	result.z = first_less_second.z && first_more_second.z;

	if (result == glm::vec3(true, true, true))
		return true;
	return false;
}

void Physics::physicsProcessing(std::vector<std::shared_ptr<Collision>>& collisions)
{
	for (std::shared_ptr<Collision> collision1: collisions) {
		for (std::shared_ptr<Collision> collision2: collisions) {
			if (collision1 != collision2)
				std::cout << fullCheckCollision(*collision1, *collision2) << std::endl;
		}
	}
}

std::vector<glm::vec3> Physics::makeGlobalCoordsFromVertex(std::vector<Vertex>& local_coords, glm::mat4& model_matrix)
{
	std::vector<glm::vec3> return_verts;
	for (auto item : local_coords)
	{
		return_verts.push_back(model_matrix * glm::vec4(item.Position, 1.0f));
	}
	return return_verts;
}

void Collision::updateCollision(const std::vector<glm::vec3>& m_vertices)
{
	makeCollision(m_type, m_vertices);
}

void Collision::makeCollision(CollisionType m_type, const std::vector<glm::vec3>& m_vertices) // m_vertices in local space of object
{
	this->m_type = m_type;

	// Working only with orthogonal object
	if (m_type == CollisionType::SQUARE){
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
