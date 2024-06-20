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

//Static class with expanded math logic
class ExpMath {
public:
	// Delete all constructors
	ExpMath() = delete;
	ExpMath(const ExpMath& obj) = delete;
	ExpMath(const ExpMath&& obj) = delete;

	static std::vector<glm::vec3> makeGlobalCoordsFromVertex(std::vector<Vertex>& local_coords, glm::mat4& model_matrix = glm::mat4(1.0f));
	static glm::mat4 makeSummarizeMat4(glm::mat4 model_matrix, glm::mat4 view_matrix = glm::mat4(1.0f), glm::mat4 proj_matrix = glm::mat4(1.0f));
};