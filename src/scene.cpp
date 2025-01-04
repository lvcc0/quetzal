#include "scene.h"

Scene::Scene(int viewport_width, int viewport_height)
    : m_Renderer(std::make_shared<Renderer>(viewport_width, viewport_height))
{
}

void Scene::update()
{
    this->doProcessing();
    this->m_Renderer->draw();
}

void Scene::doPhysicsProcessing()
{
    // TODO::Change this
    //Physics::processPhysics(m_CollisionMap);
}

void Scene::doProcessing()
{
    // Do smth

    // Turning on physics 
    if (m_IsPhysics) {
        doPhysicsProcessing();
    }
}

void Scene::enablePhysics()
{
    this->m_IsPhysics = !this->m_IsPhysics;
}

void Scene::addCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts)
{
    // TODO
}

void Scene::addSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts)
{
    // TODO
}

void Scene::addDirLight(DirLight dir_light)
{
    this->m_Lights.push_back(std::make_shared<DirLight>(dir_light));
}

void Scene::addPointLight(PointLight point_light, std::vector<Vertex> verts)
{
    this->m_Lights.push_back(std::make_shared<PointLight>(point_light));
    this->addSphBillboard(point_light.m_Name, point_light.m_Position, glm::vec2(1.0f), "lightbulb.png", verts);
}

void Scene::addSpotLight(SpotLight spot_light, std::vector<Vertex> verts)
{
    this->m_Lights.push_back(std::make_shared<SpotLight>(spot_light));
    this->addSphBillboard(spot_light.m_Name, spot_light.m_Position, glm::vec2(1.0f), "highlight.png", verts);
}