#include "scene.h"

Scene::Scene(int viewport_width, int viewport_height)
    : m_Camera(viewport_width, viewport_height), m_PostProcessing(viewport_width, viewport_height)
{
}

std::vector<std::shared_ptr<qtzl::DirectionalLight3D>> Scene::getDirectionalLights() const
{
    return this->m_DirectionalLights;
}

std::vector<std::shared_ptr<qtzl::PointLight3D>> Scene::getPointLights() const
{
    return this->m_PointLights;
}

std::vector<std::shared_ptr<qtzl::SpotLight3D>> Scene::getSpotLights() const
{
    return this->m_SpotLights;
}

std::vector<std::shared_ptr<qtzl::Node>> Scene::getNodes() const
{
    return this->m_Nodes;
}

std::vector<std::shared_ptr<qtzl::VisualNode3D>> Scene::getVisualNodes() const
{
    return this->m_VisualNodes;
}

std::vector<std::shared_ptr<qtzl::PhysicsNode3D>> Scene::getPhysicsNodes() const
{
    return this->m_PhysicsNodes;
}

void Scene::update()
{
    // Processing all nodes
    for (const auto& node_sptr : m_Nodes)
    {
        switch (node_sptr->getType())
        {
        case qtzl::Object::CYLINDRICAL_BILLBOARD:
            node_sptr->set("Target", m_Camera.m_Position);
            break;
        case qtzl::Object::SPHERICAL_BILLBOARD:
            node_sptr->set("Target", m_Camera.m_Position);
            break;
        }
    }
}

std::shared_ptr<qtzl::RigidBody3D> Scene::createRigidBody(
    const std::string& name,
    const std::string& mesh_path,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale
)
{
    std::shared_ptr<qtzl::RigidBody3D> node_sptr = std::make_shared<qtzl::RigidBody3D>(name, ResourceManager::getMesh(mesh_path));
    
    node_sptr->setGlobalPosition(position);
    node_sptr->setGlobalRotation(rotation);
    node_sptr->setScale(scale);

    this->m_Nodes.push_back(node_sptr);
    this->m_VisualNodes.push_back(node_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::StaticBody3D> Scene::createStaticBody(
    const std::string& name,
    const std::string& mesh_path,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale
)
{
    std::shared_ptr<qtzl::StaticBody3D> node_sptr = std::make_shared<qtzl::StaticBody3D>(name, ResourceManager::getMesh(mesh_path));
    
    node_sptr->setGlobalPosition(position);
    node_sptr->setGlobalRotation(rotation);
    node_sptr->setScale(scale);
    
    this->m_Nodes.push_back(node_sptr);
    this->m_VisualNodes.push_back(node_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::CylindricalBillboard> Scene::createCylindricalBillboard(
    const std::string& name,
    const std::string& texture_path,
    glm::vec3 position,
    glm::vec2 size
)
{
    std::shared_ptr<qtzl::CylindricalBillboard> node_sptr = std::make_shared<qtzl::CylindricalBillboard>(name, ResourceManager::getTexture(texture_path));
    
    node_sptr->setGlobalPosition(position);
    node_sptr->setScale(glm::vec3(size.x, size.y, 1.0f));

    this->m_Nodes.push_back(node_sptr);
    this->m_VisualNodes.push_back(node_sptr);
    
    return node_sptr;
}

std::shared_ptr<qtzl::SphericalBillboard> Scene::createSphericalBillboard(
    const std::string& name,
    const std::string& texture_path,
    glm::vec3 position,
    glm::vec2 size
)
{
    std::shared_ptr<qtzl::SphericalBillboard> node_sptr = std::make_shared<qtzl::SphericalBillboard>(name, ResourceManager::getTexture(texture_path));
    
    node_sptr->setGlobalPosition(position);
    node_sptr->setScale(glm::vec3(size.x, size.y, 1.0f));

    this->m_Nodes.push_back(node_sptr);
    this->m_VisualNodes.push_back(node_sptr);
    
    return node_sptr;
}

std::shared_ptr<qtzl::BoxCollision> Scene::createBoxCollision(const std::string& name, glm::vec3 pos)
{
    auto node_sptr = std::make_shared<qtzl::BoxCollision>(name, pos);
    this->m_Nodes.push_back(node_sptr);
    this->m_PhysicsNodes.push_back(node_sptr);
    this->m_VisualNodes.push_back(node_sptr->getVisiblePart());

    return node_sptr;
}

std::shared_ptr<qtzl::SphereCollision> Scene::createSphereCollision(const std::string& name, glm::vec3 pos)
{
    auto node_sptr = std::make_shared<qtzl::SphereCollision>(name, pos);
    this->m_Nodes.push_back(node_sptr);
    this->m_PhysicsNodes.push_back(node_sptr);
    this->m_VisualNodes.push_back(node_sptr->getVisiblePart());

    return node_sptr;
}

std::shared_ptr<qtzl::DirectionalLight3D> Scene::createDirectionalLight(
    const std::string& name,
    glm::vec3 direction,
    glm::vec3 color,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular
)
{
    std::shared_ptr<qtzl::DirectionalLight3D> node_sptr = std::make_shared<qtzl::DirectionalLight3D>(name, direction, color, ambient, diffuse, specular);

    this->m_Nodes.push_back(node_sptr);
    this->m_DirectionalLights.push_back(node_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::PointLight3D> Scene::createPointLight(
    const std::string& name,
    glm::vec3 position,
    glm::vec3 color,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    float constant,
    float linear,
    float quadratic
)
{
    std::shared_ptr<qtzl::PointLight3D> node_sptr = std::make_shared<qtzl::PointLight3D>(name, position, color, ambient, diffuse, specular, constant, linear, quadratic);

    this->m_Nodes.push_back(node_sptr);
    this->m_PointLights.push_back(node_sptr);
    
    std::shared_ptr<qtzl::SphericalBillboard> bb_sptr = this->createSphericalBillboard(name + "_bb", "textures/lightbulb.png", position, glm::vec3(0.5f, 0.5f, 1.0f));
    node_sptr->addChild(bb_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::SpotLight3D> Scene::createSpotLight(
    const std::string& name,
    glm::vec3 position,
    glm::vec3 direction,
    glm::vec3 color,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    float constant,
    float linear,
    float quadratic,
    float inner_cutoff,
    float outer_cutoff
)
{
    std::shared_ptr<qtzl::SpotLight3D> node_sptr = std::make_shared<qtzl::SpotLight3D>(name, position, direction, color, ambient, diffuse, specular, constant, linear, quadratic, inner_cutoff, outer_cutoff);

    this->m_Nodes.push_back(node_sptr);
    this->m_SpotLights.push_back(node_sptr);

    std::shared_ptr<qtzl::SphericalBillboard> bb_sptr = this->createSphericalBillboard(name + "_bb", "textures/highlight.png", position, glm::vec3(0.5f, 0.5f, 1.0f));
    node_sptr->addChild(bb_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::Skybox> Scene::createSkybox(
    const std::string& name,
    unsigned int texture
)
{
    std::shared_ptr<qtzl::Skybox> node_sptr = std::make_shared<qtzl::Skybox>(name, texture);

    this->m_Nodes.push_back(node_sptr);
    this->m_VisualNodes.push_back(node_sptr);

    return node_sptr;
}