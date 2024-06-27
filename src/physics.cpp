#include "physics.h"

// TODO: Make rays

bool Physics::checkCollision(Collision& one, Collision& two)
{
    switch (one.m_Type)
    {
    case (CollisionType::BOX):
    {
        switch (two.m_Type)
        {
        case (CollisionType::BOX): // BOX - BOX collision
        {
            bool collisionX = one.m_Position.x + one.m_Size.x >= two.m_Position.x && two.m_Position.x + two.m_Size.x >= one.m_Position.x;
            bool collisionY = one.m_Position.y + one.m_Size.y >= two.m_Position.y && two.m_Position.y + two.m_Size.y >= one.m_Position.y;
            bool collisionZ = one.m_Position.z + one.m_Size.z >= two.m_Position.z && two.m_Position.z + two.m_Size.z >= one.m_Position.z;

            return collisionX && collisionY && collisionZ;
        }
        case (CollisionType::SPHERE): // BOX - SPHERE collision
        {
            glm::vec3 sphereCenter(two.m_Position + two.m_Radius);

            glm::vec3 boxHalfExtents(one.m_Size.x / 2.0f, one.m_Size.y / 2.0f, one.m_Size.z / 2.0f);
            glm::vec3 boxCenter(one.m_Position.x + boxHalfExtents.x, one.m_Position.y + boxHalfExtents.y, one.m_Position.z + boxHalfExtents.z);

            glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
            glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

            return glm::length(shapesDifference) < two.m_Radius;
        }
        } // switch (two.m_Type)
    }
    case (CollisionType::SPHERE):
    {
        switch (two.m_Type)
        {
        case (CollisionType::BOX): // SPHERE - BOX collision
        {
            glm::vec3 sphereCenter(one.m_Position + one.m_Radius);

            glm::vec3 boxHalfExtents(two.m_Size.x / 2.0f, two.m_Size.y / 2.0f, two.m_Size.z / 2.0f);
            glm::vec3 boxCenter(two.m_Position.x + boxHalfExtents.x, two.m_Position.y + boxHalfExtents.y, two.m_Position.z + boxHalfExtents.z);

            glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
            glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

            return glm::length(shapesDifference) < one.m_Radius;
        }
        case (CollisionType::SPHERE): // SPHERE - SPHERE collision
        {
            // if the distance between centers of spheres is less than the sum of their radi, then the spheres collided
            return glm::length((one.m_Position + one.m_Radius) - (two.m_Position + two.m_Radius)) < one.m_Radius + two.m_Radius;
        }
        } // switch (two.m_Type)
    }
    } // switch (one.m_Type)

    return false; // just in case something goes wrong
}

void Physics::simplePhysics(std::shared_ptr<RigidBody>& one, std::shared_ptr<RigidBody>& two)
{
    absolutelyInelasticImpact(one, two);
}

void Physics::advancedPhysics(std::shared_ptr<RigidBody>& one, std::shared_ptr<RigidBody>& two)
{
    
}

void Physics::absolutelyInelasticImpact(std::shared_ptr<RigidBody>& one, std::shared_ptr<RigidBody>& two)
{
    
    one->m_MoveVector = (one->m_Mass * one->m_MoveVector + two->m_Mass * two->m_MoveVector) / (one->m_Mass + two->m_Mass);
    two->m_MoveVector = (one->m_Mass * one->m_MoveVector + two->m_Mass * two->m_MoveVector) / (one->m_Mass + two->m_Mass);
}

void Physics::processPhysics(std::vector<std::shared_ptr<RigidBody>>& bodies)
{
    for (std::shared_ptr<RigidBody> one : bodies)
    {
        for (std::shared_ptr<RigidBody> two : bodies)
        {
            if (one == two)
                continue;

            if (checkCollision(one->m_Collision, two->m_Collision)) {
                simplePhysics(one, two);
            }
        }
    }
}