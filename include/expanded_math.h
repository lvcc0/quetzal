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
	glm::vec3 getGlobalCoordsFromScreen(GLfloat screen_x, GLfloat screen_y, GLfloat width, GLfloat height, glm::mat4 projection_matrix, glm::mat4 view_matrix);
	// TODO: Remake this
	std::pair<glm::vec3, glm::vec3> makeAABB(const std::vector<Vertex>& coords); // First - corner with lesser coords 
	glm::mat4 makeSummarizeMat4(const glm::mat4 model_matrix, const glm::mat4 view_matrix = glm::mat4(1.0f), const glm::mat4 proj_matrix = glm::mat4(1.0f));
	std::vector<glm::vec3> returnPositionFromVertex(const std::vector<Vertex>& vertex_vector);
	
	template <typename T> std::pair<T, GLfloat> getItemWithMinimumFloat(std::vector<std::pair<T, GLfloat>> vector);

	template<typename T>
	std::pair<T, GLfloat> getItemWithMinimumFloat(std::vector<std::pair<T, GLfloat>> vector)
	{
		bool first_iteration = true;

		std::pair<T, float> return_item;
		for (const auto& item : vector)
		{
			if (first_iteration)
			{
				return_item = item;
				first_iteration = false;
			}
			else
			{
				if (item.second < return_item.second)
				{
					return_item = item;
				}
			}
		}

		return return_item;
	}
};