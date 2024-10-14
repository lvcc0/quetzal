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

enum class CollisionType { BOX, SPHERE };

// TODO: (?) make inherited BoxCollider and SphereCollider
// TODO: make rotations

class Collision
{
public:
    CollisionType m_col_Type; // collision type

    glm::vec3 m_col_Position; // for both
    glm::vec3 m_col_Size;     // for box collision
    float m_col_Radius;       // for sphere collision       

    // Constructor
    Collision(CollisionType type, glm::vec3 position, glm::vec3 size); // box collider
    Collision(CollisionType type, glm::vec3 position, float radius);   // sphere collider

    // Copy constructor
    Collision(const Collision& obj);
};