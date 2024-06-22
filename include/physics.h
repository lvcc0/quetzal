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
#include "collision.h"
#include "model.h"
#include "expanded_math.h"

class Physics {
public:
	static bool checkCollision(Collision& object1, Collision& object2);
	static bool fullCheckCollision(Collision& object1, Collision& object2);
	static void physicsProcessing(std::vector<std::shared_ptr<Model>>& models);
};