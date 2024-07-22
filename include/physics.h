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

#include "rigid_body.h"
#include "colliding_ray.h"

// TODO: Make clearing impact if objects no longer collide

class Physics
{
public:
    static bool checkCollision(Collision& one, Collision& two);

    static void processPhysics(std::map<const std::string, std::shared_ptr<Collision>>& rig_body_map); // Called every tick
};