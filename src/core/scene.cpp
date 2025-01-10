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

    this->m_DirectionalLights.push_back(node_sptr);
    this->m_Nodes.push_back(node_sptr);
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
    std::shared_ptr<qtzl::SphericalBillboard> billboard_sptr = std::make_shared<qtzl::SphericalBillboard>(name + "_bb", ResourceManager::getTexture("lightbulb.png"));

    billboard_sptr->setPosition(position);
    billboard_sptr->setScale(glm::vec3(0.5f, 0.5f, 1.0f));
    node_sptr->addChild(billboard_sptr);

    this->m_PointLights.push_back(node_sptr);
    this->m_Nodes.push_back(node_sptr);
    this->m_Nodes.push_back(billboard_sptr);
    
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
    std::shared_ptr<qtzl::SphericalBillboard> billboard_sptr = std::make_shared<qtzl::SphericalBillboard>(name + "_bb", ResourceManager::getTexture("highlight.png"));

    billboard_sptr->setPosition(position);
    billboard_sptr->setScale(glm::vec3(0.5f, 0.5f, 1.0f));
    node_sptr->addChild(billboard_sptr);

    this->m_SpotLights.push_back(node_sptr);
    this->m_Nodes.push_back(node_sptr);
    this->m_Nodes.push_back(billboard_sptr);

    return node_sptr;
}