#include "expanded_math.h"

std::vector<glm::vec3> ExpMath::makeGlobalCoordsFromVertex(std::vector<Vertex>& local_coords, glm::mat4& model_matrix)
{
	std::vector<glm::vec3> return_verts;
	for (auto item : local_coords)
	{
		return_verts.push_back(model_matrix * glm::vec4(item.Position, 1.0f));
	}
	return return_verts;
}

glm::mat4 ExpMath::makeSummarizeMat4(glm::mat4 model_matrix, glm::mat4 view_matrix, glm::mat4 proj_matrix)
{
	return (proj_matrix * view_matrix * model_matrix);
}
