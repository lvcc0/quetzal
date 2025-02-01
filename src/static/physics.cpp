#include "static/physics.h"

std::tuple<bool, Physics::Direction, glm::vec3> Physics::areColliding(std::shared_ptr<qtzl::PhysicsNode3D> first, std::shared_ptr<qtzl::PhysicsNode3D> second)
{
    switch (first->getType())
    {
    case (qtzl::Object::Type::BOX_COLLISION):
    {
        switch (second->getType())
        {
        case (qtzl::Object::Type::BOX_COLLISION): // BOX - BOX collision
        {
            bool collisionX = first->getVec3("Global position").x + first->getVec3("Size").x / 2.0f >= second->getVec3("Global position").x - second->getVec3("Size").x / 2.0f && second->getVec3("Global position").x + second->getVec3("Size").x / 2.0f >= first->getVec3("Global position").x - first->getVec3("Size").x / 2.0f;
            bool collisionY = first->getVec3("Global position").y + first->getVec3("Size").y / 2.0f >= second->getVec3("Global position").y - second->getVec3("Size").y / 2.0f && second->getVec3("Global position").y + second->getVec3("Size").y / 2.0f >= first->getVec3("Global position").y - first->getVec3("Size").y / 2.0f;
            bool collisionZ = first->getVec3("Global position").z + first->getVec3("Size").z / 2.0f >= second->getVec3("Global position").z - second->getVec3("Size").z / 2.0f && second->getVec3("Global position").z + second->getVec3("Size").z / 2.0f >= first->getVec3("Global position").z - first->getVec3("Size").z / 2.0f;

            glm::vec3 clampedDifference = glm::clamp(first->getVec3("Global position") - second->getVec3("Global position"), -second->getVec3("Size") / 2.0f, second->getVec3("Size") / 2.0f);
            glm::vec3 shapesDifference = second->getVec3("Global position") + clampedDifference - first->getVec3("Global position");

            return std::make_tuple(collisionX && collisionY && collisionZ, getCollisionDirection(shapesDifference), shapesDifference);
        }
        case (qtzl::Object::Type::SPHERE_COLLISION): // BOX - SPHERE collision NOTE: needs testing
        {
            glm::vec3 sphereCenter(second->getVec3("Global position") + second->getFloat("Radius"));

            glm::vec3 boxHalfExtents(first->getVec3("Size").x / 2.0f, first->getVec3("Size").y / 2.0f, first->getVec3("Size").z / 2.0f);
            glm::vec3 boxCenter(first->getVec3("Global position").x + boxHalfExtents.x, first->getVec3("Global position").y + boxHalfExtents.y, first->getVec3("Global position").z + boxHalfExtents.z);

            glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
            glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

            if (glm::length(shapesDifference) < second->getFloat("Radius"))
                return std::make_tuple(true, getCollisionDirection(shapesDifference), shapesDifference);
            else
                return std::make_tuple(false, RIGHT, glm::vec3(0.0f));
        }
        } // switch (second->getType())
        break;
    }
    case (qtzl::Object::Type::SPHERE_COLLISION):
    {
        switch (second->getType())
        {
        case (qtzl::Object::Type::BOX_COLLISION): // SPHERE - BOX collision NOTE: needs testing
        {
            glm::vec3 sphereCenter(first->getVec3("Global position") + first->getFloat("Radius"));

            glm::vec3 boxHalfExtents(second->getVec3("Size").x / 2.0f, second->getVec3("Size").y / 2.0f, second->getVec3("Size").z / 2.0f);
            glm::vec3 boxCenter(second->getVec3("Global position").x + boxHalfExtents.x, second->getVec3("Global position").y + boxHalfExtents.y, second->getVec3("Global position").z + boxHalfExtents.z);

            glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
            glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

            if (glm::length(shapesDifference) < second->getFloat("Radius"))
                return std::make_tuple(true, getCollisionDirection(shapesDifference), shapesDifference);
            else
                return std::make_tuple(false, RIGHT, glm::vec3(0.0f));
        }
        case (qtzl::Object::Type::SPHERE_COLLISION): // SPHERE - SPHERE collision NOTE: needs testing
        {
            // if the distance between centers of spheres is less than the sum of their radii, then the spheres collided

            glm::vec3 difference = (first->getVec3("Global position") + first->getFloat("Radius")) - (second->getVec3("Global position") + second->getFloat("Radius"));

            if (glm::length(difference) < first->getFloat("Radius") + second->getFloat("Radius"))
                return std::make_tuple(true, getCollisionDirection(difference), difference);
            else
                return std::make_tuple(false, RIGHT, glm::vec3(0.0f));
        }
        } // switch (second->getType())
    }
    } // switch (first->m_Type)

    std::cerr << "ERROR::Physics::checkPhysicsNode3D: wrong collision type" << std::endl;

    return std::make_tuple(false, UP, glm::vec3(0.0f)); // just in case something goes wrong
}

void Physics::addPhysicsNode(qtzl::PhysicsNode3D* node)
{
    physicsNodes.push_back(node);
}

bool Physics::checkPhysicsNode(void* ref)
{
    auto it = std::find_if(physicsNodes.begin(), physicsNodes.end(),
        [ref](qtzl::PhysicsNode3D* node_) {return ref == node_; });
    return it != physicsNodes.end();
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