#include "colliding_ray.h"

CollidingRay::CollidingRay(glm::vec3 m_position, glm::vec3 m_direction)
{
	this->m_Position = m_position;
	this->m_Direction = glm::normalize(m_direction);
}

bool CollidingRay::checkCollision(const Collision& object)
{
	switch (object.m_Type)
	{
	case (CollisionType::BOX): {
		std::vector<glm::vec3> directions_from_object_to_ray;
		directions_from_object_to_ray.push_back(glm::normalize(object.m_Position - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(object.m_Size.x, 0, 0)) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(object.m_Size.x, object.m_Size.y, 0)) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(object.m_Size.x, 0, object.m_Size.z)) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0, object.m_Size.y, 0)) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0, object.m_Size.y, object.m_Size.z)) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0, 0, object.m_Size.z)) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + object.m_Size) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(object.m_Size.x/2, object.m_Size.y/2, object.m_Size.z/2)) - this->m_Position));

		for (auto item : directions_from_object_to_ray) {
			if (glm::length(item - this->m_Direction) < vec_coef) {
				return true;
			}
		}

	}
	case (CollisionType::SPHERE): {
		std::vector<glm::vec3> directions_from_object_to_ray;
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(object.m_Radius, 0.0, 0.0)) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0.0, object.m_Radius, 0.0)) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0.0, 0.0, object.m_Radius)) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(-object.m_Radius, 0.0, 0.0)) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0.0, -object.m_Radius, 0.0)) - this->m_Position));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0.0, 0.0, -object.m_Radius)) - this->m_Position));

		for (auto item : directions_from_object_to_ray) {
			if (glm::length(item - this->m_Direction) < vec_coef) {
				return true;
			}
		}
	}
	}
	return false;
}
