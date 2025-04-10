#include "static/physics.h"

PHYSICS_RET_TYPE Physics::areColliding(const qtzl::BoxCollision& first, const qtzl::BoxCollision& second)
{
    bool collisionX = first.getGlobalPosition().x + first.m_Size.x / 2.0f >= second.getGlobalPosition().x - second.m_Size.x / 2.0f && second.getGlobalPosition().x + second.m_Size.x / 2.0f >= first.getGlobalPosition().x - first.m_Size.x / 2.0f;
    bool collisionY = first.getGlobalPosition().y + first.m_Size.y / 2.0f >= second.getGlobalPosition().y - second.m_Size.y / 2.0f && second.getGlobalPosition().y + second.m_Size.y / 2.0f >= first.getGlobalPosition().y - first.m_Size.y / 2.0f;
    bool collisionZ = first.getGlobalPosition().z + first.m_Size.z / 2.0f >= second.getGlobalPosition().z - second.m_Size.z / 2.0f && second.getGlobalPosition().z + second.m_Size.z / 2.0f >= first.getGlobalPosition().z - first.m_Size.z / 2.0f;

    glm::vec3 clampedDifference = glm::clamp(first.getGlobalPosition() - second.getGlobalPosition(), -second.m_Size / 2.0f, second.m_Size / 2.0f);
    glm::vec3 shapesDifference = second.getGlobalPosition() + clampedDifference - first.getGlobalPosition();

    CollisionVectors coll_vec{ -clampedDifference, clampedDifference };

    return std::make_tuple(collisionX && collisionY && collisionZ, getCollisionDirection(shapesDifference), coll_vec);
}

PHYSICS_RET_TYPE Physics::areColliding(const qtzl::SphereCollision& first, const qtzl::SphereCollision& second)
{
    // if the distance between centers of spheres is less than the sum of their radii, then the spheres collided

    glm::vec3 difference = (first.getGlobalPosition() + first.m_Radius) - (second.getGlobalPosition() + second.m_Radius);

    CollisionVectors coll_vec{-difference, difference};

    if (glm::length(difference) < first.m_Radius + second.m_Radius)
        return std::make_tuple(true, getCollisionDirection(difference), coll_vec);
    else
        return std::make_tuple(false, RIGHT, coll_vec);
}

PHYSICS_RET_TYPE Physics::areColliding(const qtzl::BoxCollision& first, const qtzl::SphereCollision& second)
{
    glm::vec3 sphereCenter(second.getGlobalPosition() + second.m_Radius);

    glm::vec3 boxHalfExtents(first.m_Size.x / 2.0f, first.m_Size.y / 2.0f, first.m_Size.z / 2.0f);
    glm::vec3 boxCenter(first.getGlobalPosition().x + boxHalfExtents.x, first.getGlobalPosition().y + boxHalfExtents.y, first.getGlobalPosition().z + boxHalfExtents.z);

    glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
    glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

    CollisionVectors coll_vec{clampedDifference, -clampedDifference};

    if (glm::length(shapesDifference) < second.m_Radius)
        return std::make_tuple(true, getCollisionDirection(shapesDifference), coll_vec);
    else
        return std::make_tuple(false, RIGHT, coll_vec);
}

PHYSICS_RET_TYPE Physics::areColliding(const qtzl::SphereCollision& first, const qtzl::BoxCollision& second)
{
    glm::vec3 sphereCenter(first.getGlobalPosition() + first.m_Radius);

    glm::vec3 boxHalfExtents(second.m_Size.x / 2.0f, second.m_Size.y / 2.0f, second.m_Size.z / 2.0f);
    glm::vec3 boxCenter(second.getGlobalPosition().x + boxHalfExtents.x, second.getGlobalPosition().y + boxHalfExtents.y, second.getGlobalPosition().z + boxHalfExtents.z);

    glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
    glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

    CollisionVectors coll_vec{ -clampedDifference, clampedDifference };

    if (glm::length(shapesDifference) < first.m_Radius)
        return std::make_tuple(true, getCollisionDirection(shapesDifference), coll_vec);
    else
        return std::make_tuple(false, RIGHT, coll_vec);
}

Physics::Direction Physics::doCollisions(qtzl::BoxCollision& first, qtzl::BoxCollision& second)
{
    PHYSICS_RET_TYPE collision = Physics::areColliding(first, second);

    if (std::get<0>(collision)) // collision detected
    {
        Physics::Direction direction = std::get<1>(collision);
        CollisionVectors difference = std::get<2>(collision);

        qtzl::PhysicsNode3D* phys_first = &first;
        qtzl::PhysicsNode3D* phys_second = &second;

        bool haveSameStreangth = (first.m_Streangth == second.m_Streangth);

        if (haveSameStreangth)
        {
            first.getPhysParent()->translate(difference.second);
            second.getPhysParent()->translate(difference.first);
            return direction;
        }


        // Get weaker object (it could be any Node3D inheritor or collision)
        qtzl::PhysicsNode3D* weak_col = first.m_Streangth > second.m_Streangth ? phys_second : phys_first;
        qtzl::Node3D& weak_node = weak_col->getPhysParent() != nullptr ? *(weak_col->getPhysParent()) : *weak_col;
        glm::vec3 weak_diff = first.m_Streangth > second.m_Streangth ? difference.first : difference.second;

        // Get stronger object (it could be any Node3D inheritor or collision)
        qtzl::PhysicsNode3D* strong_col = first.m_Streangth > second.m_Streangth ? phys_first : phys_second;
        qtzl::Node3D& strong_node = strong_col->getPhysParent() != nullptr ? *(strong_col->getPhysParent()) : *strong_col;
        glm::vec3 strong_diff = first.m_Streangth > second.m_Streangth ? difference.first : difference.second;

        //weak_diff = glm::vec3(weak_diff.x * 3, weak_diff.y * 3, weak_diff.z * 3);
        //strong_diff = glm::vec3(strong_diff.x * 3, strong_diff.y * 3, strong_diff.z * 3);

        // Simple interaction(stronger object moving weaker)
        weak_node.translate(strong_diff);

        return direction;
    }

    return Physics::Direction::NIL;
}

Physics::Direction Physics::doCollisions(qtzl::BoxCollision& first, qtzl::SphereCollision& second)
{
    PHYSICS_RET_TYPE collision = Physics::areColliding(first, second);

    if (std::get<0>(collision)) // collision detected
    {
        Physics::Direction direction = std::get<1>(collision);
        CollisionVectors difference = std::get<2>(collision);

        qtzl::PhysicsNode3D* phys_first = &first;
        qtzl::PhysicsNode3D* phys_second = &second;

        bool haveSameStreangth = (first.m_Streangth == second.m_Streangth);

        if (haveSameStreangth)
        {
            first.getPhysParent()->translate(difference.second);
            second.getPhysParent()->translate(difference.first);
            return direction;
        }


        // Get weaker object (it could be any Node3D inheritor or collision)
        qtzl::PhysicsNode3D* weak_col = first.m_Streangth > second.m_Streangth ? phys_second : phys_first;
        qtzl::Node3D& weak_node = weak_col->getPhysParent() != nullptr ? *(weak_col->getPhysParent()) : *weak_col;
        glm::vec3 weak_diff = first.m_Streangth > second.m_Streangth ? difference.first : difference.second;

        // Get stronger object (it could be any Node3D inheritor or collision)
        qtzl::PhysicsNode3D* strong_col = first.m_Streangth > second.m_Streangth ? phys_first : phys_second;
        qtzl::Node3D& strong_node = strong_col->getPhysParent() != nullptr ? *(strong_col->getPhysParent()) : *strong_col;
        glm::vec3 strong_diff = first.m_Streangth > second.m_Streangth ? difference.first : difference.second;

        //weak_diff = glm::vec3(weak_diff.x * 3, weak_diff.y * 3, weak_diff.z * 3);
        //strong_diff = glm::vec3(strong_diff.x * 3, strong_diff.y * 3, strong_diff.z * 3);

        // Simple interaction(stronger object moving weaker)
        weak_node.translate(strong_diff);

        return direction;
    }

    return Physics::Direction::NIL;
}

Physics::Direction Physics::doCollisions(qtzl::SphereCollision& first, qtzl::BoxCollision& second)
{
    PHYSICS_RET_TYPE collision = Physics::areColliding(first, second);

    if (std::get<0>(collision)) // collision detected
    {
        Physics::Direction direction = std::get<1>(collision);
        CollisionVectors difference = std::get<2>(collision);

        qtzl::PhysicsNode3D* phys_first = &first;
        qtzl::PhysicsNode3D* phys_second = &second;

        bool haveSameStreangth = (first.m_Streangth == second.m_Streangth);

        if (haveSameStreangth)
        {
            first.getPhysParent()->translate(difference.second);
            second.getPhysParent()->translate(difference.first);
            return direction;
        }


        // Get weaker object (it could be any Node3D inheritor or collision)
        qtzl::PhysicsNode3D* weak_col = first.m_Streangth > second.m_Streangth ? phys_second : phys_first;
        qtzl::Node3D& weak_node = weak_col->getPhysParent() != nullptr ? *(weak_col->getPhysParent()) : *weak_col;
        glm::vec3 weak_diff = first.m_Streangth > second.m_Streangth ? difference.first : difference.second;

        // Get stronger object (it could be any Node3D inheritor or collision)
        qtzl::PhysicsNode3D* strong_col = first.m_Streangth > second.m_Streangth ? phys_first : phys_second;
        qtzl::Node3D& strong_node = strong_col->getPhysParent() != nullptr ? *(strong_col->getPhysParent()) : *strong_col;
        glm::vec3 strong_diff = first.m_Streangth > second.m_Streangth ? difference.first : difference.second;

        //weak_diff = glm::vec3(weak_diff.x * 3, weak_diff.y * 3, weak_diff.z * 3);
        //strong_diff = glm::vec3(strong_diff.x * 3, strong_diff.y * 3, strong_diff.z * 3);

        // Simple interaction(stronger object moving weaker)
        weak_node.translate(strong_diff);

        return direction;
    }

    return Physics::Direction::NIL;
}

Physics::Direction Physics::doCollisions(qtzl::SphereCollision& first, qtzl::SphereCollision& second)
{
    PHYSICS_RET_TYPE collision = Physics::areColliding(first, second);

    if (std::get<0>(collision)) // collision detected
    {
        Physics::Direction direction = std::get<1>(collision);
        CollisionVectors difference = std::get<2>(collision);

        qtzl::PhysicsNode3D* phys_first = &first;
        qtzl::PhysicsNode3D* phys_second = &second;

        bool haveSameStreangth = (first.m_Streangth == second.m_Streangth);

        if (haveSameStreangth)
        {
            first.getPhysParent()->translate(difference.second);
            second.getPhysParent()->translate(difference.first);
            return direction;
        }


        // Get weaker object (it could be any Node3D inheritor or collision)
        qtzl::PhysicsNode3D* weak_col = first.m_Streangth > second.m_Streangth ? phys_second : phys_first;
        qtzl::Node3D& weak_node = weak_col->getPhysParent() != nullptr ? *(weak_col->getPhysParent()) : *weak_col;
        glm::vec3 weak_diff = first.m_Streangth > second.m_Streangth ? difference.first : difference.second;

        // Get stronger object (it could be any Node3D inheritor or collision)
        qtzl::PhysicsNode3D* strong_col = first.m_Streangth > second.m_Streangth ? phys_first : phys_second;
        qtzl::Node3D& strong_node = strong_col->getPhysParent() != nullptr ? *(strong_col->getPhysParent()) : *strong_col;
        glm::vec3 strong_diff = first.m_Streangth > second.m_Streangth ? difference.first : difference.second;

        //weak_diff = glm::vec3(weak_diff.x * 3, weak_diff.y * 3, weak_diff.z * 3);
        //strong_diff = glm::vec3(strong_diff.x * 3, strong_diff.y * 3, strong_diff.z * 3);

        // Simple interaction(stronger object moving weaker)
        weak_node.translate(strong_diff);

        return direction;
    }

    return Physics::Direction::NIL;
}

Physics::Direction Physics::getCollisionDirection(const glm::vec3& target)
{
    glm::vec3 compass[] =
    {
        {  1.0f,  0.0f,  0.0f }, // +X
        { -1.0f,  0.0f,  0.0f }, // -X
        {  0.0f,  1.0f,  0.0f }, // +Y
        {  0.0f, -1.0f,  0.0f }, // -Y
        {  0.0f,  0.0f,  1.0f }, // +Z
        {  0.0f,  0.0f, -1.0f }  // -Z
    };

    float max = 0.0f;
    unsigned int bestMatch = -1;

    for (unsigned int i = 0; i < 6; i++)
    {
        float dotProduct = glm::dot(glm::normalize(target), compass[i]);

        if (dotProduct > max)
        {
            max = dotProduct;
            bestMatch = i;
        }
    }

    return (Direction)bestMatch;
}