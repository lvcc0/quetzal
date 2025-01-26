#pragma once

#include "core/scene.h"

// TODO: function that predicts a collision with given velocity

class Physics
{
public:
    Physics() = delete;
    Physics(const Physics&) = delete;
    Physics(Physics&&) = delete;

    // Gets called every frame in the engine class
    static void process(std::shared_ptr<Scene>& scene);

    // Check if two nodes are colliding
    static bool areColliding(std::shared_ptr<qtzl::PhysicsNode3D> first, std::shared_ptr<qtzl::PhysicsNode3D> second);

    // Get all the nodes given node is colliding with
    static std::vector<std::shared_ptr<qtzl::PhysicsNode3D>> getCollisions(std::shared_ptr<Scene>& scene, std::shared_ptr<qtzl::PhysicsNode3D> node);
};