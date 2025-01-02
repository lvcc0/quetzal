#include "scene.h"

Scene::Scene(int viewport_width, int viewport_height)
    : m_Renderer(std::make_shared<Renderer>(viewport_width, viewport_height))
{
}

Scene::~Scene()
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

void Scene::addShader(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path, ShaderType type)
{
    auto shader = ResourceManager::makeShaderProgram(vertex_shader_rel_path, fragment_shader_rel_path);
    
    shaders_active.push(shader, type);
}

void Scene::addTexture(std::string name, std::string type)
{
    auto texture = ResourceManager::getTexture(name);
    texture->m_Type = type;

    unsigned int i{ 0 };

    while (m_TextureMap.find(name) != m_TextureMap.end())
    {
        i++;

        if (i == 1)
            name += std::to_string(i);
        else
            name = name.substr(0, name.size() - std::to_string(i - 1).length()) + std::to_string(i);
    }

    m_TextureMap.emplace(name, texture);
}

void Scene::addModel(std::string name)
{
    auto model = std::make_shared<Model>(ResourceManager::getModel(name));

    unsigned int i{ 0 };

    for (auto item: m_NodeVec)
    {
        if (item->getName() == name) {
            i++;

            if (i == 1)
                name += std::to_string(i);
            else
                name = name.substr(0, name.size() - std::to_string(i - 1).length()) + std::to_string(i);
        }
    }
    model->setName(name);

    //m_RenderableVec.push_back(model);
    //m_NodeVec.push_back(model);
    //m_SceneObjectVec.push_back(model);
}

void Scene::addCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts)
{
    auto cyl_billboard = std::make_shared<CylindricalBillboard>(ResourceManager::makeCylBillboard(pos, size, texture_name, verts));
    cyl_billboard->setName(name);
    m_RenderableVec.push_back(cyl_billboard);
    m_NodeVec.push_back(cyl_billboard);
    m_SceneObjectVec.push_back(cyl_billboard);
}

void Scene::addSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts)
{
    auto sph_billboard = std::make_shared<SphericalBillboard>(ResourceManager::makeSphBillboard(pos, size, texture_name, verts));
    sph_billboard->setName(name);
    m_RenderableVec.push_back(sph_billboard);
    m_NodeVec.push_back(sph_billboard);
    m_SceneObjectVec.push_back(sph_billboard);
}

void Scene::addDirLight(DirLight dir_light)
{
    std::shared_ptr<DirLight> light = std::make_shared<DirLight>(dir_light);
    this->m_DirLights.push_back(light);
}

void Scene::addPointLight(PointLight point_light, std::vector<Vertex> verts)
{
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(point_light);
    this->m_PointLights.push_back(light);

    this->addSphBillboard(point_light.m_name, point_light.m_pos, glm::vec2(1.0f), "lightbulb.png", verts);
}

void Scene::addSpotLight(SpotLight spot_light, std::vector<Vertex> verts)
{
    std::shared_ptr<SpotLight> light = std::make_shared<SpotLight>(spot_light);
    this->m_SpotLights.push_back(light);

    this->addSphBillboard(spot_light.m_name, spot_light.m_pos, glm::vec2(1.0f), "highlight.png", verts);
}
