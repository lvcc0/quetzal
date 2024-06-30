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

#include "expanded_math.h"
#include "collision.h"

class CollidingRay {
	float vec_coef = 0.5;
public:
	glm::vec3 m_Position;
	glm::vec3 m_Direction; // normalized

	CollidingRay(glm::vec3 m_position, glm::vec3 m_direction);

	bool checkCollision(const Collision& object);
};