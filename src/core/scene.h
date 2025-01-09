#pragma once

// std
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

// thirdparty
#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/resource_manager.h"
#include "core/rendering/post_processing.h"
#include "core/rendering/camera.h"

#include "scene/3d/rigid_body.h"
#include "scene/3d/static_body.h"

#include "scene/3d/cylindrical_billboard.h"
#include "scene/3d/spherical_billboard.h"

#include "scene/3d/directional_light3d.h"
#include "scene/3d/point_light3d.h"
#include "scene/3d/spot_light3d.h"

class Scene
{
public:
    Scene(int viewport_width, int viewport_height);

    // NOTE: store not the camera, but it's position and stuff in a scene and have have the camera stored in the engine or be static?
    Camera m_Camera;
    PostProcessing m_PostProcessing;

    bool m_IsPostProcessing = false;
    bool m_IsPhysics = true;

    std::vector<std::shared_ptr<qtzl::Node>> getNodes() const;

    // Gets called every frame in the engine class
    void update();

    // Do physics
    void doPhysicsProcessing();

    // Do processing
    void doProcessing();

    // Enable physics
    void enablePhysics();

    qtzl::RigidBody            createRigidBody();
    qtzl::StaticBody           createStaticBody();

    qtzl::CylindricalBillboard createCylindricalBillboard(
        const std::string& name,
        const std::string& texture_name,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec2 size = glm::vec2(1.0f)
    );

    qtzl::SphericalBillboard createSphericalBillboard(
        const std::string& name,
        const std::string& texture_name,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec2 size = glm::vec2(1.0f)
    );

    qtzl::DirectionalLight3D   createDirectionalLight();
    qtzl::PointLight3D         createPointLight();
    qtzl::SpotLight3D          createSpotLight();

private:
    std::vector<std::shared_ptr<qtzl::Node>> m_Nodes;
};