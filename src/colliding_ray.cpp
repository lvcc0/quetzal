#include "colliding_ray.h"

Ray::Ray(glm::vec3 m_ray_origin, glm::vec3 m_direction)
{
	this->m_RayOrigin = m_ray_origin;
	this->m_RayDirection = glm::normalize(m_direction);
}

bool Ray::TestRayOBBIntersection(glm::vec3 aabb_min, glm::vec3 aabb_max, glm::mat4 ModelMatrix, float& intersection_distance)
{
	// Intersection method from Real-Time Rendering and Essential Mathematics for Games

	float tMin = 0.0f;
	float tMax = 100000.0f;

	glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

	glm::vec3 delta = OBBposition_worldspace - m_RayOrigin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(m_RayDirection, xaxis);

		if (fabs(f) > 0.001f) { // Standard case

			float t1 = (e + aabb_min.x) / f; // Intersection with the "left" plane
			float t2 = (e + aabb_max.x) / f; // Intersection with the "right" plane
			// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

			// We want t1 to represent the nearest intersection, 
			// so if it's not the case, invert t1 and t2
			if (t1 > t2) {
				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin)
				return false;

		}
		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(m_RayDirection, yaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabb_min.y) / f;
			float t2 = (e + aabb_max.y) / f;

			if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	// Exactly the same thing than above.
	{
		glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
		float e = glm::dot(zaxis, delta);
		float f = glm::dot(m_RayDirection, zaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabb_min.z) / f;
			float t2 = (e + aabb_max.z) / f;

			if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
				return false;
		}
	}

	intersection_distance = tMin;
	return true;
}
// TODO:: change this(maybe use scheme as in TestRayOBBIntersection)
bool CollidingRay::checkCollision(const Collision& object)
{
	switch (object.m_Type)
	{
	case (CollisionType::BOX): {
		std::vector<glm::vec3> directions_from_object_to_ray;
		directions_from_object_to_ray.push_back(glm::normalize(object.m_Position- this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(object.m_Size.x, 0, 0)) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(object.m_Size.x, object.m_Size.y, 0)) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(object.m_Size.x, 0, object.m_Size.z)) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0, object.m_Size.y, 0)) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0, object.m_Size.y, object.m_Size.z)) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0, 0, object.m_Size.z)) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + object.m_Size) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(object.m_Size.x/2, object.m_Size.y/2, object.m_Size.z/2)) - this->m_RayOrigin));

		for (auto item : directions_from_object_to_ray) {
			if (glm::length(item - this->m_RayDirection) < vec_coef) {
				return true;
			}
		}
		break;
	}
	case (CollisionType::SPHERE): {
		std::vector<glm::vec3> directions_from_object_to_ray;
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(object.m_Radius, 0.0, 0.0)) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0.0, object.m_Radius, 0.0)) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0.0, 0.0, object.m_Radius)) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(-object.m_Radius, 0.0, 0.0)) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0.0, -object.m_Radius, 0.0)) - this->m_RayOrigin));
		directions_from_object_to_ray.push_back(glm::normalize((object.m_Position + glm::vec3(0.0, 0.0, -object.m_Radius)) - this->m_RayOrigin));

		for (auto item : directions_from_object_to_ray) {
			if (glm::length(item - this->m_RayDirection) < vec_coef) {
				return true;
			}
		}
		break;
	}
	}
	return false;
}
