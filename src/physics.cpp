#include "physics.h"


bool Physics::checkCollision(Collision& object1, Collision& object2)
{
	// Placeholder //
	return false;
}

bool Physics::fullCheckCollision(Collision& object1, Collision& object2)
{
	// using as vectors of bools
	glm::vec3 first_less_second(false, false, false);
	glm::vec3 first_more_second(false, false, false);

	std::vector<glm::vec3> first_verts = ExpMath::makeGlobalCoordsFromLocal(object1.m_vertices, object1.m_model_matrix);
	std::vector<glm::vec3> second_verts = ExpMath::makeGlobalCoordsFromLocal(object2.m_vertices, object2.m_model_matrix);

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

void Physics::physicsProcessing(std::vector<std::shared_ptr<RigidBody>>& bodies)
{
	// Checking collisions 
	for (std::shared_ptr<RigidBody> body1: bodies) {
		for (std::shared_ptr<RigidBody> body2: bodies) {
			if (body1 != body2)
				std::cout << fullCheckCollision(body1->m_collision, body2->m_collision) << std::endl;
				// ¬ообще сюда можно вставить че угодно
		}
	}
}


