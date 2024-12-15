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
namespace ExpMath
{
    std::vector<glm::vec3> makeGlobalCoordsFromVertex(const std::vector<Vertex>& local_coords, const glm::mat4& model_matrix = glm::mat4(1.0f));
    std::vector<glm::vec3> makeGlobalCoordsFromLocal(const std::vector<glm::vec3>& local_coords, const glm::mat4& model_matrix = glm::mat4(1.0f));
    glm::vec3 getGlobalCoordsFromScreen(GLfloat screen_x, GLfloat screen_y, GLfloat width, GLfloat height, glm::mat4 projection_matrix, glm::mat4 view_matrix);
    std::pair<glm::vec3, glm::vec3> makeAABB(const std::vector<Vertex>& vector); // First - corner with lesser coords 
    glm::mat4 makeSummarizeMat4(const glm::mat4 model_matrix, const glm::mat4 view_matrix = glm::mat4(1.0f), const glm::mat4 proj_matrix = glm::mat4(1.0f));
    std::vector<glm::vec3> returnPositionFromVertex(const std::vector<Vertex>& vertex_vector);
    
    template <typename T> std::pair<T, GLfloat> getItemWithMinimumFloat(const std::vector<std::pair<T, GLfloat>> vector);

    template<typename T>
    std::pair<T, GLfloat> getItemWithMinimumFloat(const std::vector<std::pair<T, GLfloat>> vector)
    {
        if (vector.size() == 0) {
            std::cerr << "ERROR::vector had zero size\n";
            __debugbreak();
        }
        return *std::min_element(vector.begin(), vector.end(), [](std::pair<T, float> first, std::pair<T, float> min_element) {if (first.second < min_element.second) return true; return false; });
    }
};