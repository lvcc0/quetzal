#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "vertex.h"

//expanded math logic
namespace ExpMath {
	std::vector<glm::vec3> makeGlobalCoordsFromVertex(const std::vector<Vertex>& local_coords, const glm::mat4& model_matrix = glm::mat4(1.0f));
	std::vector<glm::vec3> makeGlobalCoordsFromLocal(const std::vector<glm::vec3>& local_coords, const glm::mat4& model_matrix = glm::mat4(1.0f));
	glm::mat4 makeSummarizeMat4(const glm::mat4 model_matrix, const glm::mat4 view_matrix = glm::mat4(1.0f), const glm::mat4 proj_matrix = glm::mat4(1.0f));
	std::vector<glm::vec3> returnPositionFromVertex(const std::vector<Vertex>& vertex_vector);
};