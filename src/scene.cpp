#include "scene.h"

Scene::Scene(Camera&& camera)
    : m_Camera(std::make_shared<Camera>(camera)),
      m_PostProcessing(std::make_shared<PostProcessing>(ResourceManager::makePostProcessingShaders(), camera.m_width, camera.m_height))
{
}

Scene::~Scene()
{    
}

void Scene::update()
{
    if (!m_PostProcessing->m_ActiveShaders.empty() && m_IsPostProcessing)
        this->m_PostProcessing->deactivate();

    m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)this->m_Camera->m_width / (float)this->m_Camera->m_height, 0.1f, 100.0f);
    glm::mat4 view = this->m_Camera->getViewMatrix();

    // Default shader
    shaders_active.MAIN_SHADER->activateShader();
    
    shaders_active.MAIN_SHADER->setVec3("viewPos", this->m_Camera->m_pos);
    shaders_active.MAIN_SHADER->setFloat("material.shininess", 32.0f);

    shaders_active.MAIN_SHADER->setMat4("projection", m_ProjectionMatrix);
    shaders_active.MAIN_SHADER->setMat4("view", view);

    // Stencil shader
    shaders_active.STENCIL_SHADER->activateShader();

    shaders_active.STENCIL_SHADER->setMat4("projection", m_ProjectionMatrix);
    shaders_active.STENCIL_SHADER->setMat4("view", view);

    // Rendering

    // Rendering lights' influence
    if (!this->m_DirLights.empty())
    {
        for (auto i = 0; i < this->m_DirLights.size(); i++)
        {
            this->m_DirLights[i]->updateUni(shaders_active.MAIN_SHADER, i);
        }
    }
    if (!this->m_PointLights.empty())
    {
        for (auto i = 0; i < this->m_PointLights.size(); i++)
        {
            this->m_PointLights[i]->updateUni(shaders_active.MAIN_SHADER, i);
            std::string name = m_PointLights[i]->m_name;
            std::shared_ptr<SphericalBillboard> sph_bill = std::dynamic_pointer_cast<SphericalBillboard>(*std::find_if(m_NodeVec.begin(), m_NodeVec.end(), [name](std::shared_ptr<Scene_Node> item) {if (item->getName() == name) return true; return false; }));

            sph_bill->m_Position = m_PointLights[i]->m_pos;
            sph_bill->m_Target = m_Camera->m_pos;
            sph_bill->draw(shaders_active);
        }
    }
    if (!this->m_SpotLights.empty())
    {
        for (auto i = 0; i < this->m_SpotLights.size(); i++)
        {
            this->m_SpotLights[i]->updateUni(shaders_active.MAIN_SHADER, i);
            
            std::string name = m_SpotLights[i]->m_name;
            std::shared_ptr<SphericalBillboard> sph_bill = std::dynamic_pointer_cast<SphericalBillboard>(*std::find_if(m_NodeVec.begin(), m_NodeVec.end(), [name](std::shared_ptr<Scene_Node> item) {if (item->getName() == name) return true; return false; }));

            sph_bill->m_Position = m_PointLights[i]->m_pos;
            sph_bill->m_Target = m_Camera->m_pos;
            sph_bill->draw(shaders_active);
        }
    }

    // TODO: until i make meshes renderable
    for (auto& entry : ResourceManager::takeModels())
    {
        std::cout << entry.second.getName() << std::endl;
        entry.second.draw(shaders_active);
    }

    // Draw all renderable
    if (!this->m_RenderableVec.empty())
    {
        for (auto it: m_RenderableVec)
        {
            auto object = it;
            if (typeid(*object) == typeid(SphericalBillboard))
            {
                std::shared_ptr<SphericalBillboard> sph_bill = std::dynamic_pointer_cast<SphericalBillboard>(it);
                sph_bill->m_Target = m_Camera->m_pos;
                sph_bill->draw(shaders_active);
            }
            else if (typeid(*object) == typeid(CylindricalBillboard))
            {
                std::shared_ptr<CylindricalBillboard> cyl_bill = std::dynamic_pointer_cast<CylindricalBillboard>(it);
                cyl_bill->m_Target = m_Camera->m_pos;
                cyl_bill->draw(shaders_active);
            }
            else 
            {
                it->draw(shaders_active);
            }
        }
    }
    if (!m_PostProcessing->m_ActiveShaders.empty() && m_IsPostProcessing)
        this->m_PostProcessing->activate();
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

void Scene::enablePostProcessing()
{
    this->m_IsPostProcessing = !this->m_IsPostProcessing;
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
    auto texture = ResourceManager::makeTexture(name);
    texture->m_type = type;

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
    auto model = std::make_shared<Model>(ResourceManager::makeModel(name));

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
