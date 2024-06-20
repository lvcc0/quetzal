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
			else if (first_item.x < second_item.x)
				first_less_second.x = true;
			else if (first_item.x == second_item.x) {
				first_more_second.x = true;
				first_less_second.x = true;
			}
			if (first_item.y > second_item.y)
				first_more_second.y = true;
			else if (first_item.y < second_item.y)
				first_less_second.y = true;
			else if (first_item.y == second_item.y) {
				first_more_second.y = true;
				first_less_second.y = true;
			}
			if (first_item.z > second_item.z)
				first_more_second.z = true;
			else if (first_item.z < second_item.z)
				first_less_second.z = true;
			else if (first_item.z == second_item.z) {
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

void Physics::physicsProcessing(std::vector<std::shared_ptr<Model>>& models)
{
	// Checking collisions 
	for (std::shared_ptr<Model> model1: models) {
		for (std::shared_ptr<Model> model2: models) {
			if (model1 != model2)
				std::cout << fullCheckCollision(*model1->m_collision, *model2->m_collision) << std::endl;
				// ¬ообще сюда можно вставить че угодно
		}
	}
}


