#pragma once

#include "core/rendering/post_processing.h"
#include "core/rendering/camera.h"

#include "scene/3d/skybox.h"

#include "scene/3d/rigid_body3d.h"
#include "scene/3d/static_body3d.h"

#include "scene/3d/cylindrical_billboard.h"
#include "scene/3d/spherical_billboard.h"

#include "scene/3d/directional_light3d.h"
#include "scene/3d/point_light3d.h"
#include "scene/3d/spot_light3d.h"

// temp
#include "scene/3d/box_collision.h"
#include "scene/3d/sphere_collision.h"

class Scene
{
public:
    Scene(int viewport_width, int viewport_height);

    Camera m_Camera;
    PostProcessing m_PostProcessing;

    bool m_IsPostProcessing = false;

    NodeContainer&     getNodeContainer();

    // Gets called every frame in the engine class
    void update();

    // Node creating

    std::shared_ptr<qtzl::StaticBody3D> createStaticBody(
        std::string name,
        std::string mesh_path,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f)
    );

    std::shared_ptr<qtzl::RigidBody3D> createRigidBody(
        std::string name,
        std::string mesh_path,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f)
    );
    
    std::shared_ptr<qtzl::Billboard> createBillboard(
        // TODO :D
    );

    std::shared_ptr<qtzl::CylindricalBillboard> createCylindricalBillboard(
        std::string name,
        std::string texture_path,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec2 size = glm::vec2(1.0f)
    );

    std::shared_ptr<qtzl::SphericalBillboard> createSphericalBillboard(
        std::string name,
        std::string texture_path,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec2 size = glm::vec2(1.0f)
    );

    std::shared_ptr<qtzl::BoxCollision> createBoxCollision(
        std::string name,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 size = glm::vec3(1.0f)
    );

    std::shared_ptr<qtzl::SphereCollision> createSphereCollision(
        std::string name,
        glm::vec3 position = glm::vec3(0.0f),
        float radius = 1.0f
    );

    std::shared_ptr<qtzl::DirectionalLight3D> createDirectionalLight(
        std::string name,
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3 color = glm::vec3(1.0f),
        glm::vec3 ambient = glm::vec3(0.5f),
        glm::vec3 diffuse = glm::vec3(0.5f),
        glm::vec3 specular = glm::vec3(0.5f)
    );

    std::shared_ptr<qtzl::PointLight3D> createPointLight(
        std::string name,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 color = glm::vec3(1.0f),
        glm::vec3 ambient = glm::vec3(0.5f),
        glm::vec3 diffuse = glm::vec3(0.5f),
        glm::vec3 specular = glm::vec3(0.5f),
        float ant = 1.0f,
        float linear = 0.09f,
        float quadratic = 0.032f
    );

    std::shared_ptr<qtzl::SpotLight3D> createSpotLight(
        std::string name,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 direction = glm::vec3(0.0f),
        glm::vec3 color = glm::vec3(1.0f),
        glm::vec3 ambient = glm::vec3(0.5f),
        glm::vec3 diffuse = glm::vec3(0.5f),
        glm::vec3 specular = glm::vec3(0.5f),
        float ant = 1.0f,
        float linear = 0.09f,
        float quadratic = 0.032f,
        float inner_cutoff = 7.5f,
        float outer_cutoff = 10.0f
    );

    std::shared_ptr<qtzl::Skybox> createSkybox(
        std::string name,
        unsigned int texture
    );

private:
    NodeContainer m_NodeContainer;
};