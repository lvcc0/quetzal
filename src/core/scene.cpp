#include "scene.h"

Scene::Scene(int viewport_width, int viewport_height)
    : m_Camera(viewport_width, viewport_height), m_PostProcessing(viewport_width, viewport_height)
{
}

std::vector<std::shared_ptr<qtzl::Node>> Scene::getNodes() const
{
    return this->m_Nodes;
}

void Scene::update()
{
    this->doProcessing();
}

void Scene::doPhysicsProcessing()
{
    // TODO: Change this
    //Physics::processPhysics(m_CollisionMap);
}

void Scene::doProcessing()
{
    // Do smth

    // Turning on physics 
    if (m_IsPhysics)
        doPhysicsProcessing();
}

void Scene::enablePhysics()
{
    this->m_IsPhysics = !this->m_IsPhysics;
}

//qtzl::RigidBody Scene::createRigidBody() TODO
//{
//}

//qtzl::StaticBody Scene::createStaticBody() TODO
//{
//}

//qtzl::Billboard Scene::createBillboard() TODO
//{
//}

std::shared_ptr < qtzl::CylindricalBillboard> Scene::createCylindricalBillboard(
    const std::string& name,
    const std::string& texture_name,
    glm::vec3 position,
    glm::vec2 size
)
{
    std::shared_ptr<qtzl::CylindricalBillboard> node_sptr = std::make_shared<qtzl::CylindricalBillboard>(name, ResourceManager::getTexture(texture_name));

    this->m_Nodes.push_back(node_sptr);
    return node_sptr;
}

std::shared_ptr < qtzl::SphericalBillboard> Scene::createSphericalBillboard(
    const std::string& name,
    const std::string& texture_name,
    glm::vec3 position,
    glm::vec2 size
)
{
    std::shared_ptr<qtzl::SphericalBillboard> node_sptr = std::make_shared<qtzl::SphericalBillboard>(name, ResourceManager::getTexture(texture_name));

    this->m_Nodes.push_back(node_sptr);
    return node_sptr;
}

//qtzl::DirectionalLight3D Scene::createDirectionalLight() TODO
//{
//}

//qtzl::PointLight3D Scene::createPointLight() TODO
//{
//}

//qtzl::SpotLight3D Scene::createSpotLight() TODO
//{
//}