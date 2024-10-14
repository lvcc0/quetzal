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

std::pair<glm::vec3, glm::vec3> ExpMath::makeAABB(const std::vector<Vertex>& coords)
{
	if (coords.size() == 0)
		std::cerr << "ERROR::EXP_MATH makeAABB HAS TAKEN COORDS WITH ZERO SIZE" << std::endl;

	glm::vec3 min_corner, max_corner;

	bool first_iter = true;
	for (auto item : coords) 
	{
		if (first_iter)
		{
			min_corner = item.Position;
			max_corner = item.Position;

			first_iter = false;
		}
		else
		{
			if (min_corner.x > item.Position.x)
				min_corner.x = item.Position.x;
			if (max_corner.x < item.Position.x)
				max_corner.x = item.Position.x;
			if (min_corner.y > item.Position.y)
				min_corner.y = item.Position.y;
			if (max_corner.y < item.Position.y)
				max_corner.y = item.Position.y;
			if (min_corner.z > item.Position.z)
				min_corner.z = item.Position.z;
			if (max_corner.z < item.Position.z)
				max_corner.z = item.Position.z;
		}
	}
	return std::make_pair(min_corner, max_corner);
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
