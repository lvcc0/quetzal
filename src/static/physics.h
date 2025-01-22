#pragma once

// std
#include <iostream>
#include <algorithm>
#include <vector>

// thirdparty
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/scene.h"
#include "core/rendering/shader_program.h"
#include "scene/3d/physics_node3d.h"

namespace qtzl {
    class Physics
    {
    public:
        // Delete all constructors
        Physics() = delete;
        Physics(const Physics&) = delete;
        Physics(Physics&&) = delete;

        // TODO: the rest lol
        static bool checkPhysicsNode3D(std::shared_ptr<PhysicsNode3D> first, std::shared_ptr<PhysicsNode3D> second);
        static void physicsLoop(std::shared_ptr<Scene>& scene);
    };
}