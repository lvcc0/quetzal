#include "scene.h"

Scene::Scene(int viewport_width, int viewport_height)
    : m_Camera(viewport_width, viewport_height, glm::vec3(0.0f)),
      m_PostProcessing(ResourceManager::getPPShaders(), viewport_width, viewport_height)
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

// TODO: all the create funcs