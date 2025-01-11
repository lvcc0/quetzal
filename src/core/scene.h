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
    bool m_IsPhysicsProcessing = true;

    std::vector<std::shared_ptr<qtzl::DirectionalLight3D>> getDirectionalLights() const;
    std::vector<std::shared_ptr<qtzl::PointLight3D>> getPointLights() const;
    std::vector<std::shared_ptr<qtzl::SpotLight3D>> getSpotLights() const;

    std::vector<std::shared_ptr<qtzl::Node>> getNodes() const;

    // Gets called every frame in the engine class
    void update();

    // Node creating

    std::shared_ptr<qtzl::StaticBody> createStaticBody(
        const std::string& name,
        const std::string& mesh_name,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f)
    );

    std::shared_ptr<qtzl::RigidBody> createRigidBody();
    
    std::shared_ptr<qtzl::Billboard> createBillboard();

    std::shared_ptr<qtzl::CylindricalBillboard> createCylindricalBillboard(
        const std::string& name,
        const std::string& texture_name,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec2 size = glm::vec2(1.0f)
    );

    std::shared_ptr<qtzl::SphericalBillboard> createSphericalBillboard(
        const std::string& name,
        const std::string& texture_name,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec2 size = glm::vec2(1.0f)
    );

    std::shared_ptr<qtzl::DirectionalLight3D> createDirectionalLight(
        const std::string& name,
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3 color = glm::vec3(1.0f),
        glm::vec3 ambient = glm::vec3(0.5f),
        glm::vec3 diffuse = glm::vec3(0.5f),
        glm::vec3 specular = glm::vec3(0.5f)
    );

    std::shared_ptr<qtzl::PointLight3D> createPointLight(
        const std::string& name,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 color = glm::vec3(1.0f),
        glm::vec3 ambient = glm::vec3(0.5f),
        glm::vec3 diffuse = glm::vec3(0.5f),
        glm::vec3 specular = glm::vec3(0.5f),
        float constant = 1.0f,
        float linear = 0.09f,
        float quadratic = 0.032f
    );

    std::shared_ptr<qtzl::SpotLight3D> createSpotLight(
        const std::string& name,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 direction = glm::vec3(0.0f),
        glm::vec3 color = glm::vec3(1.0f),
        glm::vec3 ambient = glm::vec3(0.5f),
        glm::vec3 diffuse = glm::vec3(0.5f),
        glm::vec3 specular = glm::vec3(0.5f),
        float constant = 1.0f,
        float linear = 0.09f,
        float quadratic = 0.032f,
        float inner_cutoff = 7.5f,
        float outer_cutoff = 10.0f
    );

private:
    std::vector<std::shared_ptr<qtzl::DirectionalLight3D>> m_DirectionalLights;
    std::vector<std::shared_ptr<qtzl::PointLight3D>> m_PointLights;
    std::vector<std::shared_ptr<qtzl::SpotLight3D>> m_SpotLights;

    std::vector<std::shared_ptr<qtzl::Node>> m_Nodes;
};