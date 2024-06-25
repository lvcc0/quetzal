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
#include "rigid_body.h"
#include "expanded_math.h"

class Physics
{
public:
    static bool checkCollision(Collision& one, Collision& two);
    static void physicsProcessing(std::vector<std::shared_ptr<RigidBody>>& bodies);
};