#include "scene.h"

Scene::Scene(int viewport_width, int viewport_height)
    : m_Camera(viewport_width, viewport_height), m_PostProcessing(viewport_width, viewport_height)
{
}

NodeContainer& Scene::getNodeContainer()
{
    return this->m_NodeContainer;
}

void Scene::update()
{
    // Processing all nodes
    
}

std::shared_ptr<qtzl::RigidBody3D> Scene::createRigidBody(
    std::string name,
    std::string mesh_path,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale, 
    unsigned int streangth
)
{
    std::shared_ptr<qtzl::PhysicsNode3D> phys_node = createBoxCollision(name, position, scale, streangth);
    std::shared_ptr<qtzl::RigidBody3D> node_sptr = std::make_shared<qtzl::RigidBody3D>(name, createModel3D(name, mesh_path), phys_node);
    
    node_sptr->setGlobalPosition(position);
    node_sptr->setGlobalRotation(rotation);
    node_sptr->setScale(scale);

    this->m_NodeContainer.addNode(node_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::Model3D> Scene::createModel3D(
    std::string name,
    std::string mesh_path,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale)
{
    std::shared_ptr<qtzl::Model3D> node_sptr = std::make_shared<qtzl::Model3D>(name, ResourceManager::getMesh(mesh_path));

    node_sptr->setGlobalPosition(position);
    node_sptr->setGlobalRotation(rotation);
    node_sptr->setScale(scale);

    this->m_NodeContainer.addNode(node_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::StaticBody3D> Scene::createStaticBody(
    std::string name,
    std::string mesh_path,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale,
    unsigned int streangth
)
{
    std::shared_ptr<qtzl::PhysicsNode3D> phys_node = createBoxCollision(name, position, scale, streangth);
    std::shared_ptr<qtzl::StaticBody3D> node_sptr = std::make_shared<qtzl::StaticBody3D>(name, createModel3D(name, mesh_path), phys_node);
    
    node_sptr->setGlobalPosition(position);
    node_sptr->setGlobalRotation(rotation);
    node_sptr->setScale(scale);
    
    this->m_NodeContainer.addNode(node_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::CylindricalBillboard> Scene::createCylindricalBillboard(
    std::string name,
    std::string texture_path,
    glm::vec3 position,
    glm::vec2 size
)
{
    std::shared_ptr<qtzl::CylindricalBillboard> node_sptr = std::make_shared<qtzl::CylindricalBillboard>(name, ResourceManager::getTexture(texture_path));
    
    node_sptr->setGlobalPosition(position);
    node_sptr->setScale(glm::vec3(size.x, size.y, 1.0f));

    this->m_NodeContainer.addNode(node_sptr);
    
    return node_sptr;
}

std::shared_ptr<qtzl::SphericalBillboard> Scene::createSphericalBillboard(
    std::string name,
    std::string texture_path,
    glm::vec3 position,
    glm::vec2 size
)
{
    std::shared_ptr<qtzl::SphericalBillboard> node_sptr = std::make_shared<qtzl::SphericalBillboard>(name, ResourceManager::getTexture(texture_path));
    
    node_sptr->setGlobalPosition(position);
    node_sptr->setScale(glm::vec3(size.x, size.y, 1.0f));

    this->m_NodeContainer.addNode(node_sptr);
    
    return node_sptr;
}

std::shared_ptr<qtzl::BoxCollision> Scene::createBoxCollision(
    std::string name,
    glm::vec3 position,
    glm::vec3 size,
    unsigned int streangth
)
{
    std::shared_ptr<qtzl::BoxCollision> node_sptr = std::make_shared<qtzl::BoxCollision>(name, streangth);

    node_sptr->setGlobalPosition(position);
    node_sptr->setSize(size);

    this->m_NodeContainer.addNode(node_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::SphereCollision> Scene::createSphereCollision(
    std::string name,
    glm::vec3 position,
    float radius,
    unsigned int streangth
)
{
    std::shared_ptr<qtzl::SphereCollision> node_sptr = std::make_shared<qtzl::SphereCollision>(name, streangth);

    node_sptr->setGlobalPosition(position);
    node_sptr->setRadius(radius);

    this->m_NodeContainer.addNode(node_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::DirectionalLight3D> Scene::createDirectionalLight(
    std::string name,
    glm::vec3 direction,
    glm::vec3 color,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular
)
{
    std::shared_ptr<qtzl::DirectionalLight3D> node_sptr = std::make_shared<qtzl::DirectionalLight3D>(name, direction, color, ambient, diffuse, specular);

    this->m_NodeContainer.addNode(node_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::PointLight3D> Scene::createPointLight(
    std::string name,
    glm::vec3 position,
    glm::vec3 color,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    float ant,
    float linear,
    float quadratic
)
{
    std::shared_ptr<qtzl::PointLight3D> node_sptr = std::make_shared<qtzl::PointLight3D>(name, position, color, ambient, diffuse, specular,         ant, linear, quadratic);
    
    std::shared_ptr<qtzl::SphericalBillboard> bb_sptr = this->createSphericalBillboard(name + "_bb", "textures/lightbulb.png", position, glm::vec3(0.5f, 0.5f, 1.0f));
    node_sptr->addChild(bb_sptr);
    this->m_NodeContainer.addNode(node_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::SpotLight3D> Scene::createSpotLight(
    std::string name,
    glm::vec3 position,
    glm::vec3 direction,
    glm::vec3 color,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    float ant,
    float linear,
    float quadratic,
    float inner_cutoff,
    float outer_cutoff
)
{
    std::shared_ptr<qtzl::SpotLight3D> node_sptr = std::make_shared<qtzl::SpotLight3D>(name, position, direction, color, ambient, diffuse, specular, ant, linear, quadratic, inner_cutoff, outer_cutoff);

    std::shared_ptr<qtzl::SphericalBillboard> bb_sptr = this->createSphericalBillboard(name + "_bb", "textures/highlight.png", position, glm::vec3(0.5f, 0.5f, 1.0f));
    node_sptr->addChild(bb_sptr);
    this->m_NodeContainer.addNode(node_sptr);

    return node_sptr;
}

std::shared_ptr<qtzl::Skybox> Scene::createSkybox(
    std::string     name,
    unsigned int texture
)
{
    std::shared_ptr<qtzl::Skybox> node_sptr = std::make_shared<qtzl::Skybox>(name, texture);

    this->m_NodeContainer.addNode(node_sptr);

    return node_sptr;
}