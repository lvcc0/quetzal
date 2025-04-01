#pragma once

// std
#include <tuple>

#include "core/scene.h"

// TODO: function that predicts a collision with given velocity
// NOTE: some of current collisions take wrong coordinates

#define PHYSICS_RET_TYPE std::tuple<bool, Physics::Direction, glm::vec3>


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
    static PHYSICS_RET_TYPE areColliding(const qtzl::BoxCollision& first, const qtzl::BoxCollision& second);
    static PHYSICS_RET_TYPE areColliding(const qtzl::SphereCollision& first, const qtzl::SphereCollision& second);
    static PHYSICS_RET_TYPE areColliding(const qtzl::BoxCollision& first, const qtzl::SphereCollision& second);
    static PHYSICS_RET_TYPE areColliding(const qtzl::SphereCollision& first, const qtzl::BoxCollision& second);
    
private:
    // Returns snapped ro axis collision direction
    static Direction getCollisionDirection(const glm::vec3& target);
};