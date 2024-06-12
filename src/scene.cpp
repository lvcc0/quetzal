#include "scene.h"

Scene::Scene() :
    m_ResourceManager(std::make_shared<ResourceManager>(RES_PATH)),
    m_PostProcessing(std::make_shared<PostProcessing>(m_ResourceManager->make_post_processing_shaders("screen_shaders"), 1280, 720)) // TODO: set width and height the normal way
{ /* empty */ }

Scene::~Scene()
{
    std::map<std::string, std::shared_ptr<Shader>>::iterator it = this->m_ResourceManager->shaderMap.begin();
    while (it != this->m_ResourceManager->shaderMap.end())
    {
        it->second->deleteShader();
        it++;
    }
}

void Scene::update()
{
    if (m_CurrentScreenShader)
        this->m_PostProcessing->deactivate();

    this->m_CurrentShader->activateShader();

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)this->m_Camera->m_width / (float)this->m_Camera->m_height, 0.1f, 100.0f);
    glm::mat4 view = this->m_Camera->getViewMatrix();

    this->m_CurrentShader->setVec3("viewPos", this->m_Camera->m_pos);
    this->m_CurrentShader->setFloat("material.shininess", 32.0f);

    this->m_CurrentShader->setMat4("projection", proj);
    this->m_CurrentShader->setMat4("view", view);

    // Rendering lights' influence
    if (!this->m_DirLights.empty())
    {
        for (auto i = 0; i < this->m_DirLights.size(); i++)
        {
            this->m_DirLights[i]->UpdateUni(this->m_CurrentShader, i);
        }
    }
    if (!this->m_PointLights.empty())
    {
        for (auto i = 0; i < this->m_PointLights.size(); i++)
        {
            this->m_PointLights[i]->UpdateUni(this->m_CurrentShader, i);
        }
    }
    if (!this->m_SpotLights.empty())
    {
        for (auto i = 0; i < this->m_SpotLights.size(); i++)
        {
            this->m_SpotLights[i]->UpdateUni(this->m_CurrentShader, i);
        }
    }

    // TODO: do something with billboards corresponding to point- and spotlights

    // Draw all the stuff inside all of the maps in corresponding resource manager
    if (!this->m_ResourceManager->modelMap.empty())
    {
        std::map<std::string, std::shared_ptr<Model>>::iterator it = this->m_ResourceManager->modelMap.begin();
        while (it != this->m_ResourceManager->modelMap.end())
        {
            it->second->Draw(m_CurrentShader);
            it++;
        }
    }
    if (!this->m_ResourceManager->cylBillboardMap.empty())
    {
        std::map<std::string, std::shared_ptr<CylindricalBillboard>>::iterator it = this->m_ResourceManager->cylBillboardMap.begin();
        while (it != this->m_ResourceManager->cylBillboardMap.end())
        {
            it->second->Draw(m_CurrentShader, this->m_Camera->m_pos);
            it++;
        }
    }
    if (!this->m_ResourceManager->sphBillboardMap.empty())
    {
        std::map<std::string, std::shared_ptr<SphericalBillboard>>::iterator it = this->m_ResourceManager->sphBillboardMap.begin();
        while (it != this->m_ResourceManager->sphBillboardMap.end())
        {
            it->second->Draw(m_CurrentShader, this->m_Camera->m_pos);
            it++;
        }
    }

    if (m_CurrentScreenShader)
        this->m_PostProcessing->activate(m_CurrentScreenShader);
}

void Scene::setShader(const std::string& name)
{
    this->m_CurrentShader = this->m_ResourceManager->shaderMap.at(name);
}

void Scene::setScreenShader(const std::string& name)
{
    this->m_CurrentScreenShader = this->m_PostProcessing->shaderMap.at(name);
}

std::shared_ptr<Shader> Scene::addShader(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path)
{
    return this->m_ResourceManager->make_shader_program(name, vertex_shader_rel_path, fragment_shader_rel_path);
}

std::shared_ptr<Texture> Scene::addTexture(std::string name, std::string type, const std::string& texture_rel_path)
{
    return this->m_ResourceManager->make_texture(name, type, texture_rel_path);
}

std::shared_ptr<Model> Scene::addModel(std::string name, const std::string& model_rel_path)
{
    return this->m_ResourceManager->make_model(name, model_rel_path);
}

std::shared_ptr<CylindricalBillboard> Scene::addCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    return this->m_ResourceManager->make_cyl_billboard(name, pos, size, texture_path);
}

std::shared_ptr<SphericalBillboard> Scene::addSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    return this->m_ResourceManager->make_sph_billboard(name, pos, size, texture_path);
}

std::shared_ptr<DirLight> Scene::addDirLight(DirLight dir_light)
{
    std::shared_ptr<DirLight> light = std::make_shared<DirLight>(dir_light);
    this->m_DirLights.push_back(light);

    return light;
}

std::shared_ptr<PointLight> Scene::addPointLight(PointLight point_light)
{
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(point_light);
    this->m_PointLights.push_back(light);

    return light;
}

std::shared_ptr<SpotLight> Scene::addSpotLight(SpotLight spot_light)
{
    std::shared_ptr<SpotLight> light = std::make_shared<SpotLight>(spot_light);
    this->m_SpotLights.push_back(light);

    return light;
}