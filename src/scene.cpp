#include "scene.h"

Scene::Scene()
{
    this->resourceManager = std::make_shared<ResourceManager>(RES_PATH);
    this->defaultShader = this->addShader("default_shader", "shaders/default.vert", "shaders/default.frag");
}

Scene::~Scene()
{
    this->defaultShader->Delete();
}

void Scene::update()
{
    this->defaultShader->Activate();

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)this->camera->m_width / (float)this->camera->m_height, 0.1f, 100.0f);
    glm::mat4 view = this->camera->getViewMatrix();

    this->defaultShader->setVec3("viewPos", this->camera->m_pos);
    this->defaultShader->setFloat("material.shininess", 32.0f);

    this->defaultShader->setMat4("projection", proj);
    this->defaultShader->setMat4("view", view);

    // Rendering lights' influence
    if (!this->dirLights.empty())
    {
        for (auto i = 0; i < this->dirLights.size(); i++)
        {
            this->dirLights[i]->UpdateUni(this->defaultShader, i);
        }
    }
    if (!this->pointLights.empty())
    {
        for (auto i = 0; i < this->pointLights.size(); i++)
        {
            this->pointLights[i]->UpdateUni(this->defaultShader, i);

            //if (pointLights[i]->m_draw_billboard)
            //{
            //    pointBillboards[i]->Draw(defaultShader, cam.m_pos);
            //    pointBillboards[i]->m_pos = pointLights[i].m_pos;
            //}
        }
    }
    if (!this->spotLights.empty())
    {
        for (auto i = 0; i < this->spotLights.size(); i++)
        {
            this->spotLights[i]->UpdateUni(this->defaultShader, i);

            //if (spotLights[i].m_draw_billboard && spotLights[i].m_name != "flashlight")
            //{
            //    spotBillboards[i]->Draw(defaultShader, cam.m_pos);
            //    spotBillboards[i]->m_pos = spotLights[i].m_pos;
            //}
        }
    }

    // TODO: do something with billboards corresponding to point- and spotlights

    // Draw all the stuff inside all of the maps in corresponding resource manager
    if (!this->resourceManager->modelMap.empty())
    {
        std::map<std::string, std::shared_ptr<Model>>::iterator it = this->resourceManager->modelMap.begin();
        while (it != this->resourceManager->modelMap.end()) {
            it->second->Draw(defaultShader);
            it++;
        }
    }
    if (!this->resourceManager->cylBillboardMap.empty())
    {
        std::map<std::string, std::shared_ptr<CylindricalBillboard>>::iterator it = this->resourceManager->cylBillboardMap.begin();
        while (it != this->resourceManager->cylBillboardMap.end()) {
            it->second->Draw(defaultShader, this->camera->m_pos);
            it++;
        }
    }
    if (!this->resourceManager->sphBillboardMap.empty())
    {
        std::map<std::string, std::shared_ptr<SphericalBillboard>>::iterator it = this->resourceManager->sphBillboardMap.begin();
        while (it != this->resourceManager->sphBillboardMap.end()) {
            it->second->Draw(defaultShader, this->camera->m_pos);
            it++;
        }
    }
}

std::shared_ptr<Shader> Scene::addShader(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path)
{
    return this->resourceManager->make_shader_program(name, vertex_shader_rel_path, fragment_shader_rel_path);
}

std::shared_ptr<Texture> Scene::addTexture(std::string name, std::string type, const std::string& texture_rel_path)
{
    return this->resourceManager->make_texture(name, type, texture_rel_path);
}

std::shared_ptr<Model> Scene::addModel(std::string name, const std::string& model_rel_path)
{
    return this->resourceManager->make_model(name, model_rel_path);
}

std::shared_ptr<CylindricalBillboard> Scene::addCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    return this->resourceManager->make_cyl_billboard(name, pos, size, texture_path);
}

std::shared_ptr<SphericalBillboard> Scene::addSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    return this->resourceManager->make_sph_billboard(name, pos, size, texture_path);
}

std::shared_ptr<DirLight> Scene::addDirLight(DirLight dir_light)
{
    std::shared_ptr<DirLight> light = std::make_shared<DirLight>(dir_light);
    this->dirLights.push_back(light);

    return light;
}

std::shared_ptr<PointLight> Scene::addPointLight(PointLight point_light)
{
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(point_light);
    this->pointLights.push_back(light);

    return light;
}

std::shared_ptr<SpotLight> Scene::addSpotLight(SpotLight spot_light)
{
    std::shared_ptr<SpotLight> light = std::make_shared<SpotLight>(spot_light);
    this->spotLights.push_back(light);

    return light;
}