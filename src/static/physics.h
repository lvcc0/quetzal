#pragma once

// std
#include <tuple>

#include "core/scene.h"
#include "core/debugger.h"

// TODO: function that predicts a collision with given velocity
// NOTE: some of current collisions take wrong coordinates

#define PHYSICS_RET_TYPE std::tuple<bool, Physics::Direction, CollisionVectors>

struct CollisionVectors
{
    glm::vec3 first;
    glm::vec3 second;
};

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

    static Physics::Direction doCollisions(qtzl::BoxCollision& first, qtzl::BoxCollision& second);
    static Physics::Direction doCollisions(qtzl::SphereCollision& first, qtzl::SphereCollision& second);
    static Physics::Direction doCollisions(qtzl::BoxCollision& first, qtzl::SphereCollision& second);
    static Physics::Direction doCollisions(qtzl::SphereCollision& first, qtzl::BoxCollision& second);

    //template<class F, class S> requires std::is_base_of_v<qtzl::PhysicsNode3D, F> && std::is_base_of_v<qtzl::PhysicsNode3D, S>
    //static Physics::Direction doCollisions(F& first, S& second);

private:
    // Returns snapped ro axis collision direction
    static Direction getCollisionDirection(const glm::vec3& target);
};

//template<class F, class S> requires std::is_base_of_v<qtzl::PhysicsNode3D, F>&& std::is_base_of_v<qtzl::PhysicsNode3D, S>
//Physics::Direction Physics::doCollisions(F& first, S& second)
//{
//    PHYSICS_RET_TYPE collision = Physics::areColliding(first, second);
//
//    if (std::get<0>(collision)) // collision detected
//    {
//        Physics::Direction direction = std::get<1>(collision);
//        CollisionVectors difference = std::get<2>(collision);
//
//        bool haveSameStreangth = (first.m_Streangth == second.m_Streangth);
//
//        // Get weaker object (it could be any Node3D inheritor or collision)
//        qtzl::PhysicsNode3D* weak_col = first.m_Streangth > second.m_Streangth ? &first : &second;
//        qtzl::Node3D* weak_node = weak_col->getPhysParent().get() != nullptr ? weak_col->getPhysParent().get() : weak_col;
//        glm::vec3 weak_diff = &first == weak_node ? difference.first : difference.second;
//        std::pair<qtzl::Node3D*, glm::vec3> weak_obj = std::make_pair(weak_node, weak_diff); // 2 - vec from position to nearest point of another collision
//
//        // Get stronger object (it could be any Node3D inheritor or collision)
//        qtzl::PhysicsNode3D* strong_col = first.m_Streangth < second.m_Streangth ? &first : &second;
//        qtzl::Node3D* strong_node = strong_col->getPhysParent().get() != nullptr ? strong_col->getPhysParent().get() : strong_col;
//        glm::vec3 strong_diff = &first == strong_node ? difference.first : difference.second;
//        std::pair<qtzl::Node3D*, glm::vec3> strong_obj = std::make_pair(strong_node, strong_diff); // 2 - vec from position to nearest point of another collision
//
//        if (haveSameStreangth)
//        {
//            weak_obj.first->translate(weak_obj.second);
//            strong_obj.first->translate(strong_obj.second);
//        }
//        else
//        {
//            // Simple interaction(stronger object moving weaker)
//            weak_obj.first->translate(strong_obj.second);
//        }
//        return direction;
//    }
//
//    return Physics::Direction::NIL;
//}