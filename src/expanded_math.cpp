#include "expanded_math.h"

std::vector<glm::vec3> ExpMath::makeGlobalCoordsFromVertex(const std::vector<Vertex>& local_coords, const glm::mat4& model_matrix)
{
	std::vector<glm::vec3> return_verts;
	for (auto item : local_coords)
	{
		return_verts.push_back(model_matrix * glm::vec4(item.Position, 1.0f));
	}
	return return_verts;
}

std::vector<glm::vec3> ExpMath::makeGlobalCoordsFromLocal(const std::vector<glm::vec3>& local_coords, const glm::mat4& model_matrix)
{
	std::vector<glm::vec3> return_verts;
	for (auto item : local_coords)
	{
		return_verts.push_back(model_matrix * glm::vec4(item, 1.0f));
	}
	return return_verts;
}

glm::vec3 ExpMath::getGlobalCoordsFromScreen(GLfloat screen_x, GLfloat screen_y, GLfloat width, GLfloat height, glm::mat4 projection_matrix, glm::mat4 view_matrix)
{
	float x = (2.0f * screen_x) / width - 1.0f;
	float y = 1.0f - (2.0f * screen_y) / height;

	glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);

	glm::vec4 ray_eye = glm::inverse(projection_matrix) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	glm::vec4 inter_res = glm::inverse(view_matrix) * ray_eye;
	glm::vec3 ray_wor = glm::vec3(inter_res.x, inter_res.y, inter_res.z);
	ray_wor = glm::normalize(ray_wor);

	return ray_wor;
}

// Actually using for AABB
glm::vec3 ExpMath::getMinimumCoordsFromVertex(const std::vector<Vertex>& vertex)
{
	bool first_iteration = true;

	glm::vec3 return_vec3;
	for (const auto& item : vertex) 
	{
		if (first_iteration) 
		{
			return_vec3 = item.Position;
			first_iteration = false;
		}
		else {
			if (item.Position.x <= return_vec3.x && item.Position.y <= return_vec3.y && item.Position.z <= return_vec3.z)
			{
				return_vec3 = item.Position;
			}
		}
	}

	return return_vec3;
}

glm::vec3 ExpMath::getMaximumCoordsFromVertex(const std::vector<Vertex>& vertex)
{
	bool first_iteration = true;

	glm::vec3 return_vec3;
	for (const auto& item : vertex)
	{
		if (first_iteration)
		{
			return_vec3 = item.Position;
			first_iteration = false;
		}
		else {
			if (item.Position.x >= return_vec3.x && item.Position.y >= return_vec3.y && item.Position.z >= return_vec3.z)
			{
				return_vec3 = item.Position;
			}
		}
	}

	return return_vec3;
}

glm::mat4 ExpMath::makeSummarizeMat4(const glm::mat4 model_matrix, const glm::mat4 view_matrix, const glm::mat4 proj_matrix)
{
	return (proj_matrix * view_matrix * model_matrix);
}

std::vector<glm::vec3> ExpMath::returnPositionFromVertex(const std::vector<Vertex>& vertex_vector)
{
	std::vector<glm::vec3> return_verts;
	for (const auto& item : vertex_vector)
	{
		return_verts.push_back(item.Position);
	}
	return return_verts;
}
