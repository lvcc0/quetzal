#include "scene.h"
#include "scene.h"

Scene::Scene(Camera& camera) :
    m_Camera(std::make_shared<Camera>(camera)),
    m_PostProcessing(std::make_shared<PostProcessing>(ResourceManager::makePostProcessingShaders("screen_shaders"), camera.m_width, camera.m_height))
{ /* empty */ }

Scene::~Scene()
{
    std::map<std::string, std::shared_ptr<Shader>>::iterator it = this->shaderMap.begin();
    while (it != this->shaderMap.end())
    {
        it->second->deleteShader();
        it++;
    }
}

void Scene::update()
{
    if (m_CurrentScreenShader && m_is_PostProcessing)
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
            this->m_DirLights[i]->updateUni(this->m_CurrentShader, i);
        }
    }
    if (!this->m_PointLights.empty())
    {
        for (auto i = 0; i < this->m_PointLights.size(); i++)
        {
            this->m_PointLights[i]->updateUni(this->m_CurrentShader, i);
        }
    }
    if (!this->m_SpotLights.empty())
    {
        for (auto i = 0; i < this->m_SpotLights.size(); i++)
        {
            this->m_SpotLights[i]->updateUni(this->m_CurrentShader, i);
        }
    }

    // TODO: do something with billboards corresponding to point- and spotlights

    // Draw all the stuff inside all of the maps 
    if (!this->modelMap.empty())
    {
        std::map<std::string, std::shared_ptr<Model>>::iterator it = this->modelMap.begin();
        while (it != this->modelMap.end())
        {
            it->second->Draw(m_CurrentShader);
            it++;
        }
    }
    if (!this->cylBillboardMap.empty())
    {
        std::map<std::string, std::shared_ptr<CylindricalBillboard>>::iterator it = this->cylBillboardMap.begin();
        while (it != this->cylBillboardMap.end())
        {
            it->second->Draw(m_CurrentShader, this->m_Camera->m_pos);
            it++;
        }
    }
    if (!this->sphBillboardMap.empty())
    {
        std::map<std::string, std::shared_ptr<SphericalBillboard>>::iterator it = this->sphBillboardMap.begin();
        while (it != this->sphBillboardMap.end())
        {
            it->second->Draw(m_CurrentShader, this->m_Camera->m_pos);
            it++;
        }
    }

    if (m_CurrentScreenShader && m_is_PostProcessing)
        this->m_PostProcessing->activate(m_CurrentScreenShader);
}

void Scene::setPostProcessing()
{
    m_is_PostProcessing = !m_is_PostProcessing;
}

void Scene::setShader(const std::string& name)
{
    this->m_CurrentShader = this->shaderMap.at(name);
}

void Scene::setScreenShader(const std::string& name)
{
    this->m_CurrentScreenShader = this->m_PostProcessing->m_ShaderMap.at(name);
}

std::shared_ptr<Shader> Scene::addShader(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path)
{
    auto shader = ResourceManager::makeShaderProgram(name, vertex_shader_rel_path, fragment_shader_rel_path);
    shaderMap.emplace(name, shader);
    return shader;
}

std::shared_ptr<Texture> Scene::addTexture(std::string name, std::string type, const std::string& texture_rel_path)
{
    auto texture = ResourceManager::makeTexture(name, type, texture_rel_path);
    textureMap.emplace(name, texture);
    return texture;
}

std::shared_ptr<Model> Scene::addModel(std::string name, const std::string& model_rel_path)
{
    auto model = ResourceManager::makeModel(name, model_rel_path);
    modelMap.emplace(name, model);
    return model;
}

std::shared_ptr<CylindricalBillboard> Scene::addCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    auto cyl_billboard = ResourceManager::makeCylBillboard(name, pos, size, texture_path);
    cylBillboardMap.emplace(name, cyl_billboard);
    return cyl_billboard;
}

std::shared_ptr<SphericalBillboard> Scene::addSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    auto sph_billboard = ResourceManager::makeSphBillboard(name, pos, size, texture_path);
    sphBillboardMap.emplace(name, sph_billboard);
    return sph_billboard;
}

std::shared_ptr<Model> Scene::copyModel(std::string name, const std::shared_ptr<Model> const model)
{
    auto c_model = std::make_shared<Model>(*model);
    modelMap.emplace(name, c_model);
    return c_model;
}

std::shared_ptr<CylindricalBillboard> Scene::copyCylBillboard(std::string name, const std::shared_ptr<CylindricalBillboard> const cyl_billboard)
{
    auto c_cyl_billboard = std::make_shared<CylindricalBillboard>(*cyl_billboard);
    cylBillboardMap.emplace(name, c_cyl_billboard);
    return c_cyl_billboard;
}

std::shared_ptr<SphericalBillboard> Scene::copySphBillboard(std::string name, const std::shared_ptr<SphericalBillboard> const sph_billboard)
{
    auto c_sph_billboard = std::make_shared<SphericalBillboard>(*sph_billboard);
    sphBillboardMap.emplace(name, c_sph_billboard);
    return c_sph_billboard;
}

void Scene::deleteModel(std::string name, std::shared_ptr<Model>& model)
{
    modelMap.erase(name);
    model.reset();
}

void Scene::deleteCylBillboard(std::string name, std::shared_ptr<CylindricalBillboard>& cyl_billboard)
{
    cylBillboardMap.erase(name);
    cyl_billboard.reset();
}

void Scene::deleteSphBillboard(std::string name, std::shared_ptr<SphericalBillboard>& sph_billboard)
{
    sphBillboardMap.erase(name);
    sph_billboard.reset();
}

std::shared_ptr<DirLight> Scene::addDirLight(DirLight& dir_light)
{
    std::shared_ptr<DirLight> light = std::make_shared<DirLight>(dir_light);
    this->m_DirLights.push_back(light);

    return light;
}

std::shared_ptr<PointLight> Scene::addPointLight(PointLight& point_light)
{
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(point_light);
    this->m_PointLights.push_back(light);

    return light;
}

std::shared_ptr<SpotLight> Scene::addSpotLight(SpotLight& spot_light)
{
    std::shared_ptr<SpotLight> light = std::make_shared<SpotLight>(spot_light);
    this->m_SpotLights.push_back(light);

    return light;
}

void Scene::getObjectsInMaps(ObjectType objectType)
{
    switch (objectType)
    {
    case ObjectType::SHADER: {
        std::map<std::string, std::shared_ptr<Shader>>::iterator iter = shaderMap.begin();
        while (iter != shaderMap.end()) {
            std::cout << "shader \"" << iter->first << "\" with id " << iter->second->ID << std::endl;
            iter++;
        }
        break;
    }
    case ObjectType::TEXTURE: {
        std::map<std::string, std::shared_ptr<Texture>>::iterator iter = textureMap.begin();
        while (iter != textureMap.end()) {
            std::cout << "texture \"" << iter->first << "\" with id " << iter->second->ID << std::endl;
            iter++;
        }
        break;
    }
    case ObjectType::MODEL: {
        std::map<std::string, std::shared_ptr<Model>>::iterator iter = modelMap.begin();
        while (iter != modelMap.end()) {
            std::cout << "model \"" << iter->first << "\"" << std::endl;
            iter++;
        }
        break;
    }
    case ObjectType::CYL_BILLBOARD: {
        std::map<std::string, std::shared_ptr<CylindricalBillboard>>::iterator iter = cylBillboardMap.begin();
        while (iter != cylBillboardMap.end()) {
            std::cout << "cylindrical billboard \"" << iter->first << "\"" << std::endl;
            iter++;
        }
        break;
    }
    case ObjectType::SPH_BILLBOARD: {
        std::map<std::string, std::shared_ptr<SphericalBillboard>>::iterator iter = sphBillboardMap.begin();
        while (iter != sphBillboardMap.end()) {
            std::cout << "spherical billboard \"" << iter->first << "\"" << std::endl;
            iter++;
        }
        break;
    }
    default:
        std::cout << "the default output of the switch statement in the getObjectsInMaps() function" << std::endl;
    }
}