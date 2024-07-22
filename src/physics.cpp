#include "physics.h"

bool Physics::checkCollision(Collision& one, Collision& two)
{
    switch (one.m_col_Type)
    {
    case (CollisionType::BOX):
    {
        switch (two.m_col_Type)
        {
        case (CollisionType::BOX): // BOX - BOX collision
        {
            bool collisionX = one.m_col_Position.x + one.m_col_Size.x >= two.m_col_Position.x && two.m_col_Position.x + two.m_col_Size.x >= one.m_col_Position.x;
            bool collisionY = one.m_col_Position.y + one.m_col_Size.y >= two.m_col_Position.y && two.m_col_Position.y + two.m_col_Size.y >= one.m_col_Position.y;
            bool collisionZ = one.m_col_Position.z + one.m_col_Size.z >= two.m_col_Position.z && two.m_col_Position.z + two.m_col_Size.z >= one.m_col_Position.z;

            return collisionX && collisionY && collisionZ;
        }
        case (CollisionType::SPHERE): // BOX - SPHERE collision
        {
            glm::vec3 sphereCenter(two.m_col_Position + two.m_col_Radius);

            glm::vec3 boxHalfExtents(one.m_col_Size.x / 2.0f, one.m_col_Size.y / 2.0f, one.m_col_Size.z / 2.0f);
            glm::vec3 boxCenter(one.m_col_Position.x + boxHalfExtents.x, one.m_col_Position.y + boxHalfExtents.y, one.m_col_Position.z + boxHalfExtents.z);

            glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
            glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

            return glm::length(shapesDifference) < two.m_col_Radius;
        }
        } // switch (two.m_Type)
    }
    case (CollisionType::SPHERE):
    {
        switch (two.m_col_Type)
        {
        case (CollisionType::BOX): // SPHERE - BOX collision
        {
            glm::vec3 sphereCenter(one.m_col_Position + one.m_col_Radius);

            glm::vec3 boxHalfExtents(two.m_col_Size.x / 2.0f, two.m_col_Size.y / 2.0f, two.m_col_Size.z / 2.0f);
            glm::vec3 boxCenter(two.m_col_Position.x + boxHalfExtents.x, two.m_col_Position.y + boxHalfExtents.y, two.m_col_Position.z + boxHalfExtents.z);

            glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
            glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

            return glm::length(shapesDifference) < one.m_col_Radius;
        }
        case (CollisionType::SPHERE): // SPHERE - SPHERE collision
        {
            // if the distance between centers of spheres is less than the sum of their radi, then the spheres collided
            return glm::length((one.m_col_Position + one.m_col_Radius) - (two.m_col_Position + two.m_col_Radius)) < one.m_col_Radius + two.m_col_Radius;
        }
        } // switch (two.m_Type)
    }
    } // switch (one.m_Type)

    std::cerr << "ERROR IN PHYSICS::CHECK_COLLISION::ERROR COLLISION TYPE " << std::endl;

    return false; // just in case something goes wrong
}

void Physics::processPhysics(std::map<const std::string, std::shared_ptr<Collision>>& rig_body_map)
{
    for (std::pair<const std::string, std::shared_ptr<Collision>> one : rig_body_map)
    {
        for (std::pair<const std::string, std::shared_ptr<Collision>> two : rig_body_map)
        {
            if (one.second == two.second)
                continue;

            if (checkCollision(*one.second, *two.second)) {
                // Do smth //
            }
        }
    }
}