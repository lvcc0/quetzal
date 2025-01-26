#include "static/physics.h"

void Physics::process(std::shared_ptr<Scene>& scene)
{
    std::vector<std::shared_ptr<qtzl::PhysicsNode3D>> physicsNodes = scene->getPhysicsNodes();

    // don't process if there is nothing to process
    if (physicsNodes.size() <= 1)
        return;

    std::vector<std::shared_ptr<qtzl::PhysicsNode3D>>::iterator it1 = physicsNodes.begin();
    std::vector<std::shared_ptr<qtzl::PhysicsNode3D>>::iterator it2 = physicsNodes.begin();

    while (it1 != physicsNodes.end())
    {
        it2 = physicsNodes.begin();

        while (it2 != physicsNodes.end())
        {
            if (it1 == it2)
            {
                it2++;
                continue;
            }
            if (areColliding(*it1, *it2))
            {
                // TODO: Do smth
            }
            it2++;
        }

        physicsNodes.erase(it1);
        it1++;
    }
}

bool Physics::areColliding(std::shared_ptr<qtzl::PhysicsNode3D> first, std::shared_ptr<qtzl::PhysicsNode3D> second)
{
    switch (first->getType())
    {
    case (qtzl::Object::Type::BOX_COLLISION):
    {
        switch (second->getType())
        {
        case (qtzl::Object::Type::BOX_COLLISION): // BOX - BOX collision
        {
            bool collisionX = first->getVec3("Global position").x + first->getVec3("Size").x >= second->getVec3("Global position").x && second->getVec3("Global position").x + second->getVec3("Size").x >= first->getVec3("Global position").x;
            bool collisionY = first->getVec3("Global position").y + first->getVec3("Size").y >= second->getVec3("Global position").y && second->getVec3("Global position").y + second->getVec3("Size").y >= first->getVec3("Global position").y;
            bool collisionZ = first->getVec3("Global position").z + first->getVec3("Size").z >= second->getVec3("Global position").z && second->getVec3("Global position").z + second->getVec3("Size").z >= first->getVec3("Global position").z;

            return collisionX && collisionY && collisionZ;
            break;
        }
        case (qtzl::Object::Type::SPHERE_COLLISION): // BOX - SPHERE collision
        {
            glm::vec3 sphereCenter(second->getVec3("Global position") + second->getFloat("Radius"));

            glm::vec3 boxHalfExtents(first->getVec3("Size").x / 2.0f, first->getVec3("Size").y / 2.0f, first->getVec3("Size").z / 2.0f);
            glm::vec3 boxCenter(first->getVec3("Global position").x + boxHalfExtents.x, first->getVec3("Global position").y + boxHalfExtents.y, first->getVec3("Global position").z + boxHalfExtents.z);

            glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
            glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

            return glm::length(shapesDifference) < second->getFloat("Radius");
            break;
        }
        } // switch (second->getType())
        break;
    }
    case (qtzl::Object::Type::SPHERE_COLLISION):
    {
        switch (second->getType())
        {
        case (qtzl::Object::Type::BOX_COLLISION): // SPHERE - BOX collision
        {
            glm::vec3 sphereCenter(first->getVec3("Global position") + first->getFloat("Radius"));

            glm::vec3 boxHalfExtents(second->getVec3("Size").x / 2.0f, second->getVec3("Size").y / 2.0f, second->getVec3("Size").z / 2.0f);
            glm::vec3 boxCenter(second->getVec3("Global position").x + boxHalfExtents.x, second->getVec3("Global position").y + boxHalfExtents.y, second->getVec3("Global position").z + boxHalfExtents.z);

            glm::vec3 clampedDifference = glm::clamp(sphereCenter - boxCenter, -boxHalfExtents, boxHalfExtents); // clamped difference between centers
            glm::vec3 shapesDifference = boxCenter + clampedDifference - sphereCenter; // difference between sphere center and closest point on the box

            return glm::length(shapesDifference) < first->getFloat("Radius");
            break;
        }
        case (qtzl::Object::Type::SPHERE_COLLISION): // SPHERE - SPHERE collision
        {
            // if the distance between centers of spheres is less than the sum of their radi, then the spheres collided
            return glm::length((first->getVec3("Global position") + first->getFloat("Radius")) - (second->getVec3("Global position") + second->getFloat("Radius"))) < first->getFloat("Radius") + second->getFloat("Radius");
            break;
        }
        } // switch (second->getType())
        break;
    }
    } // switch (first->m_Type)

    std::cerr << "ERROR::Physics::checkPhysicsNode3D: wrong collision type" << std::endl;

    return false; // just in case something goes wrong
}

std::vector<std::shared_ptr<qtzl::PhysicsNode3D>> Physics::getCollisions(std::shared_ptr<Scene>& scene, std::shared_ptr<qtzl::PhysicsNode3D> node)
{
    std::vector<std::shared_ptr<qtzl::PhysicsNode3D>> physicsNodes = scene->getPhysicsNodes();

    // don't process if there is nothing to process
    if (physicsNodes.size() <= 1)
        return {};

    std::vector<std::shared_ptr<qtzl::PhysicsNode3D>> collisionPositive;
    std::vector<std::shared_ptr<qtzl::PhysicsNode3D>>::iterator it = physicsNodes.begin();

    while (it != physicsNodes.end())
    {
        if (areColliding(node, *it))
        {
            collisionPositive.push_back(*it);
        }
    }

    return collisionPositive;
}