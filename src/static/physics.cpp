#include "static/physics.h"

//std::tuple<bool, Physics::Direction, glm::vec3> Physics::areColliding(std::shared_ptr<qtzl::PhysicsNode3D> first, std::shared_ptr<qtzl::PhysicsNode3D> second)
//{
//    switch (first->getType())
//    {
//    case (qtzl::Object::Type::BOX_COLLISION):
//    {
//        switch (second->getType())
//        {
//        case (qtzl::Object::Type::BOX_COLLISION): // BOX - BOX collision
//        {
//            bool collisionX = first->getGlobalPosition().x + first->m_Size.x / 2.0f >= second->getGlobalPosition().x - second->m_Size.x / 2.0f && second->getGlobalPosition().x + second->m_Size.x / 2.0f >= first->getGlobalPosition().x - first->m_Size.x / 2.0f;
//            bool collisionY = first->getGlobalPosition().y + first->m_Size.y / 2.0f >= second->getGlobalPosition().y - second->m_Size.y / 2.0f && second->getGlobalPosition().y + second->m_Size.y / 2.0f >= first->getGlobalPosition().y - first->m_Size.y / 2.0f;
//            bool collisionZ = first->getGlobalPosition().z + first->m_Size.z / 2.0f >= second->getGlobalPosition().z - second->m_Size.z / 2.0f && second->getGlobalPosition().z + second->m_Size.z / 2.0f >= first->getGlobalPosition().z - first->m_Size.z / 2.0f;
//
//            glm::vec3 clampedDifference = glm::clamp(first->getGlobalPosition() - second->getGlobalPosition(), -second->m_Size / 2.0f, second->m_Size / 2.0f);
//            glm::vec3 shapesDifference = second->getGlobalPosition() + clampedDifference - first->getGlobalPosition();
//
//            return std::make_tuple(collisionX && collisionY && collisionZ, getCollisionDirection(shapesDifference), shapesDifference);
//        }
//        case (qtzl::Object::Type::SPHERE_COLLISION): // BOX - SPHERE collision NOTE: needs testing
//        {
//            glm::vec3 sphereCenter(second->getGlobalPosition() + second->getFloat("Radius"));
//
//            glm::vec3 boxHalfExtents(first->m_Size.x / 2.0f, first->m_Size.y / 2.0f, first->m_Size.z / 2.0f);
//            glm::vec3 boxCenter(first->getGlobalPosition().x + boxHalfExtents.x, first->getGlobalPosition().y + boxHalfExtents.y, first->getGlobalPosition().z + boxHalfExtents.z);
//
//            glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
//            glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box
//
//            if (glm::length(shapesDifference) < second->getFloat("Radius"))
//                return std::make_tuple(true, getCollisionDirection(shapesDifference), shapesDifference);
//            else
//                return std::make_tuple(false, RIGHT, glm::vec3(0.0f));
//        }
//        } // switch (second->getType())
//        break;
//    }
//    case (qtzl::Object::Type::SPHERE_COLLISION):
//    {
//        switch (second->getType())
//        {
//        case (qtzl::Object::Type::BOX_COLLISION): // SPHERE - BOX collision NOTE: needs testing
//        {
//            glm::vec3 sphereCenter(first->getGlobalPosition() + first->getFloat("Radius"));
//
//            glm::vec3 boxHalfExtents(second->m_Size.x / 2.0f, second->m_Size.y / 2.0f, second->m_Size.z / 2.0f);
//            glm::vec3 boxCenter(second->getGlobalPosition().x + boxHalfExtents.x, second->getGlobalPosition().y + boxHalfExtents.y, second->getGlobalPosition().z + boxHalfExtents.z);
//
//            glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
//            glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box
//
//            if (glm::length(shapesDifference) < second->getFloat("Radius"))
//                return std::make_tuple(true, getCollisionDirection(shapesDifference), shapesDifference);
//            else
//                return std::make_tuple(false, RIGHT, glm::vec3(0.0f));
//        }
//        case (qtzl::Object::Type::SPHERE_COLLISION): // SPHERE - SPHERE collision NOTE: needs testing
//        {
//            // if the distance between centers of spheres is less than the sum of their radii, then the spheres collided
//
//            glm::vec3 difference = (first->getGlobalPosition() + first->getFloat("Radius")) - (second->getGlobalPosition() + second->getFloat("Radius"));
//
//            if (glm::length(difference) < first->getFloat("Radius") + second->getFloat("Radius"))
//                return std::make_tuple(true, getCollisionDirection(difference), difference);
//            else
//                return std::make_tuple(false, RIGHT, glm::vec3(0.0f));
//        }
//        } // switch (second->getType())
//    }
//    } // switch (first->m_Type)
//
//    std::cerr << "ERROR::Physics::checkPhysicsNode3D: wrong collision type" << std::endl;
//
//    return std::make_tuple(false, UP, glm::vec3(0.0f)); // just in case something goes wrong
//}

PHYSICS_RET_TYPE Physics::areColliding(const qtzl::BoxCollision& first, const qtzl::BoxCollision& second)
{
    bool collisionX = first.getGlobalPosition().x + first.m_Size.x / 2.0f >= second.getGlobalPosition().x - second.m_Size.x / 2.0f && second.getGlobalPosition().x + second.m_Size.x / 2.0f >= first.getGlobalPosition().x - first.m_Size.x / 2.0f;
    bool collisionY = first.getGlobalPosition().y + first.m_Size.y / 2.0f >= second.getGlobalPosition().y - second.m_Size.y / 2.0f && second.getGlobalPosition().y + second.m_Size.y / 2.0f >= first.getGlobalPosition().y - first.m_Size.y / 2.0f;
    bool collisionZ = first.getGlobalPosition().z + first.m_Size.z / 2.0f >= second.getGlobalPosition().z - second.m_Size.z / 2.0f && second.getGlobalPosition().z + second.m_Size.z / 2.0f >= first.getGlobalPosition().z - first.m_Size.z / 2.0f;

    glm::vec3 clampedDifference = glm::clamp(first.getGlobalPosition() - second.getGlobalPosition(), -second.m_Size / 2.0f, second.m_Size / 2.0f);
    glm::vec3 shapesDifference = second.getGlobalPosition() + clampedDifference - first.getGlobalPosition();

    return std::make_tuple(collisionX && collisionY && collisionZ, getCollisionDirection(shapesDifference), shapesDifference);
}

PHYSICS_RET_TYPE Physics::areColliding(const qtzl::SphereCollision& first, const qtzl::SphereCollision& second)
{
    // if the distance between centers of spheres is less than the sum of their radii, then the spheres collided

    glm::vec3 difference = (first.getGlobalPosition() + first.m_Radius) - (second.getGlobalPosition() + second.m_Radius);

    if (glm::length(difference) < first.m_Radius + second.m_Radius)
        return std::make_tuple(true, getCollisionDirection(difference), difference);
    else
        return std::make_tuple(false, RIGHT, glm::vec3(0.0f));
}

PHYSICS_RET_TYPE Physics::areColliding(const qtzl::BoxCollision& first, const qtzl::SphereCollision& second)
{
    glm::vec3 sphereCenter(second.getGlobalPosition() + second.m_Radius);

    glm::vec3 boxHalfExtents(first.m_Size.x / 2.0f, first.m_Size.y / 2.0f, first.m_Size.z / 2.0f);
    glm::vec3 boxCenter(first.getGlobalPosition().x + boxHalfExtents.x, first.getGlobalPosition().y + boxHalfExtents.y, first.getGlobalPosition().z + boxHalfExtents.z);

    glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
    glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

    if (glm::length(shapesDifference) < second.m_Radius)
        return std::make_tuple(true, getCollisionDirection(shapesDifference), shapesDifference);
    else
        return std::make_tuple(false, RIGHT, glm::vec3(0.0f));
}

PHYSICS_RET_TYPE Physics::areColliding(const qtzl::SphereCollision& first, const qtzl::BoxCollision& second)
{
    glm::vec3 sphereCenter(first.getGlobalPosition() + first.m_Radius);

    glm::vec3 boxHalfExtents(second.m_Size.x / 2.0f, second.m_Size.y / 2.0f, second.m_Size.z / 2.0f);
    glm::vec3 boxCenter(second.getGlobalPosition().x + boxHalfExtents.x, second.getGlobalPosition().y + boxHalfExtents.y, second.getGlobalPosition().z + boxHalfExtents.z);

    glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
    glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

    if (glm::length(shapesDifference) < first.m_Radius)
        return std::make_tuple(true, getCollisionDirection(shapesDifference), shapesDifference);
    else
        return std::make_tuple(false, RIGHT, glm::vec3(0.0f));
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