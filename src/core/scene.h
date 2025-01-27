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

    std::vector<std::shared_ptr<qtzl::DirectionalLight3D>> getDirectionalLights() const;
    std::vector<std::shared_ptr<qtzl::PointLight3D>>       getPointLights() const;
    std::vector<std::shared_ptr<qtzl::SpotLight3D>>        getSpotLights() const;

    std::vector<std::shared_ptr<qtzl::Node>>          getNodes() const;
    std::vector<std::shared_ptr<qtzl::VisualNode3D>>  getVisualNodes() const;
    std::vector<std::shared_ptr<qtzl::PhysicsNode3D>> getPhysicsNodes() const;

    // Gets called every frame in the engine class
    void update();

    // Node creating

    std::shared_ptr<qtzl::StaticBody3D> createStaticBody(
        const std::string& name,
        const std::string& mesh_path,
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& rotation = glm::vec3(0.0f),
        const glm::vec3& scale = glm::vec3(1.0f)
    );

    std::shared_ptr<qtzl::RigidBody3D> createRigidBody(
        const std::string& name,
        const std::string& mesh_path,
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& rotation = glm::vec3(0.0f),
        const glm::vec3& scale = glm::vec3(1.0f)
    );
    
    std::shared_ptr<qtzl::Billboard> createBillboard(
        // TODO :D
    );

    std::shared_ptr<qtzl::CylindricalBillboard> createCylindricalBillboard(
        const std::string& name,
        const std::string& texture_path,
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec2& size = glm::vec2(1.0f)
    );

    std::shared_ptr<qtzl::SphericalBillboard> createSphericalBillboard(
        const std::string& name,
        const std::string& texture_path,
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec2& size = glm::vec2(1.0f)
    );

    std::shared_ptr<qtzl::BoxCollision> createBoxCollision(
        const std::string& name,
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& size = glm::vec3(1.0f)
    );

    std::shared_ptr<qtzl::SphereCollision> createSphereCollision(
        const std::string& name,
        const glm::vec3& position = glm::vec3(0.0f),
        float radius = 1.0f
    );

    std::shared_ptr<qtzl::DirectionalLight3D> createDirectionalLight(
        const std::string& name,
        const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f),
        const glm::vec3& color = glm::vec3(1.0f),
        const glm::vec3& ambient = glm::vec3(0.5f),
        const glm::vec3& diffuse = glm::vec3(0.5f),
        const glm::vec3& specular = glm::vec3(0.5f)
    );

    std::shared_ptr<qtzl::PointLight3D> createPointLight(
        const std::string& name,
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& color = glm::vec3(1.0f),
        const glm::vec3& ambient = glm::vec3(0.5f),
        const glm::vec3& diffuse = glm::vec3(0.5f),
        const glm::vec3& specular = glm::vec3(0.5f),
        float constant = 1.0f,
        float linear = 0.09f,
        float quadratic = 0.032f
    );

    std::shared_ptr<qtzl::SpotLight3D> createSpotLight(
        const std::string& name,
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& direction = glm::vec3(0.0f),
        const glm::vec3& color = glm::vec3(1.0f),
        const glm::vec3& ambient = glm::vec3(0.5f),
        const glm::vec3& diffuse = glm::vec3(0.5f),
        const glm::vec3& specular = glm::vec3(0.5f),
        float constant = 1.0f,
        float linear = 0.09f,
        float quadratic = 0.032f,
        float inner_cutoff = 7.5f,
        float outer_cutoff = 10.0f
    );

    std::shared_ptr<qtzl::Skybox> createSkybox(
        const std::string& name,
        unsigned int texture
    );

private:
    std::vector<std::shared_ptr<qtzl::DirectionalLight3D>> m_DirectionalLights; // NOTE: 3d
    std::vector<std::shared_ptr<qtzl::PointLight3D>>       m_PointLights; // NOTE: 3d
    std::vector<std::shared_ptr<qtzl::SpotLight3D>>        m_SpotLights; // NOTE: 3d

    std::vector<std::shared_ptr<qtzl::Node>>          m_Nodes; // all scene nodes
    std::vector<std::shared_ptr<qtzl::VisualNode3D>>  m_VisualNodes; // all visible scene nodes NOTE: 3d
    std::vector<std::shared_ptr<qtzl::PhysicsNode3D>> m_PhysicsNodes; // all scene nodes that can be affected by physics NOTE: 3d
};