#pragma once

// std
#include <tuple>

#include "core/scene.h"

// TODO: function that predicts a collision with given velocity
// NOTE: some of current collisions take wrong coordinates

class Physics
{
public:
    Physics() = delete;
    Physics(const Physics&) = delete;
    Physics(Physics&&) = delete;

    enum Direction
    {
        RIGHT, // +X
        LEFT,  // -X
        UP,    // +Y
        DOWN,  // -Y
        FRONT, // +Z
        BACK,  // -Z
        NIL
    };

    // Check if two nodes are colliding
    static std::tuple<bool, Direction, glm::vec3> areColliding(std::shared_ptr<qtzl::PhysicsNode3D> first, std::shared_ptr<qtzl::PhysicsNode3D> second);

    static void addPhysicsNode(qtzl::PhysicsNode3D* node);
    static bool checkPhysicsNode(void* ref);
private:
    // Returns snapped ro axis collision direction
    static Direction getCollisionDirection(const glm::vec3& target);
    inline static std::vector<qtzl::PhysicsNode3D*> physicsNodes;
};