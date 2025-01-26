#pragma once

#include "core/scene.h"

namespace qtzl
{
    class Physics
    {
    public:
        Physics() = delete;
        Physics(const Physics&) = delete;
        Physics(Physics&&) = delete;

        static bool checkPhysicsNode3D(std::shared_ptr<PhysicsNode3D> first, std::shared_ptr<PhysicsNode3D> second);
        static void physicsLoop(std::shared_ptr<Scene>& scene);
    };
}