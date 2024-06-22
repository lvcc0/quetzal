#include "expanded_math.h"
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

glm::mat4 ExpMath::makeSummarizeMat4(const glm::mat4 model_matrix, const glm::mat4 view_matrix, const glm::mat4 proj_matrix)
{
	return (proj_matrix * view_matrix * model_matrix);
}

std::vector<glm::vec3> ExpMath::returnPositionFromVertex(const std::vector<Vertex>& vertex_vector)
{
	std::vector<glm::vec3> return_verts;
	for (auto item : vertex_vector)
	{
		return_verts.push_back(item.Position);
	}
	return return_verts;
}
