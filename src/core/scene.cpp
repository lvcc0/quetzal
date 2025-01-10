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
    if (m_IsPhysicsProcessing)
    {
        //Physics::processPhysics(m_CollisionMap);
    }

    // Processing nodes' stuff
    for (const auto& node_sptr : m_Nodes)
    {
        // NOTE: is this alright? we'll keep it like this for now ig
        if (typeid(*node_sptr) == typeid(qtzl::CylindricalBillboard))
        {
            std::shared_ptr<qtzl::CylindricalBillboard> casted_sptr = std::dynamic_pointer_cast<qtzl::CylindricalBillboard>(node_sptr);
            casted_sptr->setTarget(m_Camera.m_Position);
        }
        else if (typeid(*node_sptr) == typeid(qtzl::SphericalBillboard))
        {
            std::shared_ptr<qtzl::SphericalBillboard> casted_sptr = std::dynamic_pointer_cast<qtzl::SphericalBillboard>(node_sptr);
            casted_sptr->setTarget(m_Camera.m_Position);
        }
    }
}

void Scene::togglePhysicsProcessing()
{
    this->m_IsPhysicsProcessing = !this->m_IsPhysicsProcessing;
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

std::shared_ptr<qtzl::CylindricalBillboard> Scene::createCylindricalBillboard(
    const std::string& name,
    const std::string& texture_name,
    glm::vec3 position,
    glm::vec2 size
)
{
    std::shared_ptr<qtzl::CylindricalBillboard> node_sptr = std::make_shared<qtzl::CylindricalBillboard>(name, ResourceManager::getTexture(texture_name));
    node_sptr->setPosition(position);
    node_sptr->setScale(glm::vec3(size.x, size.y, 1.0f));

    this->m_Nodes.push_back(node_sptr);
    return node_sptr;
}

std::shared_ptr<qtzl::SphericalBillboard> Scene::createSphericalBillboard(
    const std::string& name,
    const std::string& texture_name,
    glm::vec3 position,
    glm::vec2 size
)
{
    std::shared_ptr<qtzl::SphericalBillboard> node_sptr = std::make_shared<qtzl::SphericalBillboard>(name, ResourceManager::getTexture(texture_name));
    node_sptr->setPosition(position);
    node_sptr->setScale(glm::vec3(size.x, size.y, 1.0f));

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