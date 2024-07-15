#include "scene.h"

Scene::Scene(Camera& camera)
    : m_Camera(std::make_shared<Camera>(camera)),
      m_PostProcessing(std::make_shared<PostProcessing>(ResourceManager::makePostProcessingShaders("postprocess"), camera.m_width, camera.m_height))
{ /* empty */ }

Scene::~Scene()
{
    std::map<std::string, std::shared_ptr<Shader>>::iterator it = this->m_ShaderMap.begin();
    while (it != this->m_ShaderMap.end())
    {
        it->second->deleteShader();
        it++;
    }
}

void Scene::update()
{
    if (!m_CurrentScreenShaders.empty() && m_IsPostProcessing)
        this->m_PostProcessing->deactivate();

    m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)this->m_Camera->m_width / (float)this->m_Camera->m_height, 0.1f, 100.0f);
    glm::mat4 view = this->m_Camera->getViewMatrix();

    if (this->m_StencilShader != nullptr) 
    {
        this->m_StencilShader->activateShader();

        this->m_StencilShader->setMat4("projection", m_ProjectionMatrix);
        this->m_StencilShader->setMat4("view", view);
    }

    this->m_CurrentShader->activateShader();

    this->m_CurrentShader->setVec3("viewPos", this->m_Camera->m_pos);
    this->m_CurrentShader->setFloat("material.shininess", 32.0f);

    this->m_CurrentShader->setMat4("projection", m_ProjectionMatrix);
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

            this->m_SphBillboardMap.at(m_PointLights[i]->m_name)->translate(this->m_PointLights[i]->m_pos);
            this->m_SphBillboardMap.at(m_PointLights[i]->m_name)->draw(m_CurrentShader, m_Camera->m_pos);
        }
    }
    if (!this->m_SpotLights.empty())
    {
        for (auto i = 0; i < this->m_SpotLights.size(); i++)
        {
            this->m_SpotLights[i]->updateUni(this->m_CurrentShader, i);

            this->m_SphBillboardMap.at(m_SpotLights[i]->m_name)->translate(this->m_SpotLights[i]->m_pos);
            this->m_SphBillboardMap.at(m_SpotLights[i]->m_name)->draw(m_CurrentShader, m_Camera->m_pos);
        }
    }

    // Draw all models 
    if (!this->m_ModelMap.empty())
    {
        std::map<std::string, std::shared_ptr<Model>>::iterator it = this->m_ModelMap.begin();
        while (it != this->m_ModelMap.end())
        {
            it->second->draw(m_CurrentShader);
            it++;
        }
    }
    // Draw all rigid bodies
    if (!this->m_RigidBodyMap.empty())
    {
        std::map<std::string, std::shared_ptr<RigidBody>>::iterator it = this->m_RigidBodyMap.begin();
        while (it != this->m_RigidBodyMap.end())
        {
            it->second->draw(m_CurrentShader);
            it++;
        }
    }
    // Draw all cylindrical billboards
    if (!this->m_CylBillboardMap.empty())
    {
        std::map<std::string, std::shared_ptr<CylindricalBillboard>>::iterator it = this->m_CylBillboardMap.begin();
        while (it != this->m_CylBillboardMap.end())
        {
            it->second->draw(m_CurrentShader, this->m_Camera->m_pos);
            it++;
        }
    }
    // Draw all spherical billboards
    if (!this->m_SphBillboardMap.empty())
    {
        std::map<std::string, std::shared_ptr<SphericalBillboard>>::iterator it = this->m_SphBillboardMap.begin();
        while (it != this->m_SphBillboardMap.end())
        {
            it->second->draw(m_CurrentShader, this->m_Camera->m_pos);
            it++;
        }
    }

    if (!m_CurrentScreenShaders.empty() && m_IsPostProcessing)
        this->m_PostProcessing->activate(m_CurrentScreenShaders);
}

void Scene::doPhysicsProcessing()
{
    Physics::processPhysics(m_RigidBodies);
}

void Scene::doProcessing()
{
    // Let objects moving or smth else
    if (!m_IsPreworking) {
        // Example
        // Let rigid bodies moving()
        if (!this->m_RigidBodyMap.empty())
        {
            std::map<std::string, std::shared_ptr<RigidBody>>::iterator it = this->m_RigidBodyMap.begin();
            while (it != this->m_RigidBodyMap.end())
            {
                it->second->move();
                it++;
            }
        }

        // Turning on physics if not preworking
        if (m_IsPhysics) {
            doPhysicsProcessing();
        }
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

void Scene::enablePreworking()
{
    this->m_IsPreworking = !this->m_IsPreworking;
}

void Scene::setShader(const std::string& name)
{
    this->m_CurrentShader = this->m_ShaderMap.at(name);
}

void Scene::setScreenShader(const std::string& name, bool enabled)
{
    std::vector<std::string>& currentScreenShaderNames = getScreenShaders();

    if (enabled)
        this->m_CurrentScreenShaders.push_back(this->m_PostProcessing->m_ShaderMap.at(name));
    else if (std::find(currentScreenShaderNames.begin(), currentScreenShaderNames.end(), name) != currentScreenShaderNames.end())
        this->m_CurrentScreenShaders.erase(std::find(m_CurrentScreenShaders.begin(), m_CurrentScreenShaders.end(), this->m_PostProcessing->m_ShaderMap.at(name)));
} 

const std::string Scene::getShader()
{
    for (const auto& entry : this->m_ShaderMap)
        if (entry.second == this->m_CurrentShader)
            return entry.first;

    return "";
}

std::vector<std::string> Scene::getScreenShaders()
{
    std::vector<std::string> result;

    for (const auto& entry : this->m_PostProcessing->m_ShaderMap)
        if (std::find(this->m_CurrentScreenShaders.begin(), this->m_CurrentScreenShaders.end(), entry.second) != this->m_CurrentScreenShaders.end())
            result.push_back(entry.first);

    return result;
}

std::map<const std::string, std::shared_ptr<Shader>> Scene::getShaderMap()
{
    return this->m_ShaderMap;
}

std::map<const std::string, std::shared_ptr<Texture>> Scene::getTextureMap()
{
    return this->m_TextureMap;
}

std::map<const std::string, std::shared_ptr<Model>> Scene::getModelMap()
{
    return this->m_ModelMap;
}

std::map<const std::string, std::shared_ptr<RigidBody>> Scene::getRigidBodyMap()
{
    return this->m_RigidBodyMap;
}

std::map<const std::string, std::shared_ptr<CylindricalBillboard>> Scene::getCylindricalBillboardMap()
{
    return this->m_CylBillboardMap;
}

std::map<const std::string, std::shared_ptr<SphericalBillboard>> Scene::getSphericalBiillboardMap()
{
    return this->m_SphBillboardMap;
}

std::shared_ptr<Shader> Scene::addShader(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path)
{
    auto shader = ResourceManager::makeShaderProgram(name, vertex_shader_rel_path, fragment_shader_rel_path);
    
    if (m_ShaderMap.empty()) // automatically set this shader if it's the first one in the map
        this->m_CurrentShader = shader;

    m_ShaderMap.emplace(name, shader);
    return shader;
}

std::shared_ptr<Texture> Scene::addTexture(std::string name, std::string type, const std::string& texture_rel_path)
{
    auto texture = ResourceManager::makeTexture(name, type, texture_rel_path);
    m_TextureMap.emplace(name, texture);

    return texture;
}

std::shared_ptr<Model> Scene::addModel(std::string name, const std::string& model_rel_path)
{
    auto model = ResourceManager::makeModel(name, model_rel_path);
    m_ModelMap.emplace(name, model);

    return model;
}

std::shared_ptr<RigidBody> Scene::addRigidBody(std::string name, const std::string& model_rel_path, Collision &collision)
{
    auto rigid_body = std::make_shared<RigidBody>(ResourceManager::makeModel(name, model_rel_path), collision);
    m_RigidBodyMap.emplace(name, rigid_body);
    m_RigidBodies.push_back(rigid_body);
    
    return rigid_body;
}

std::shared_ptr<CylindricalBillboard> Scene::addCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    auto cyl_billboard = ResourceManager::makeCylBillboard(name, pos, size, texture_path);
    m_CylBillboardMap.emplace(name, cyl_billboard);

    return cyl_billboard;
}

std::shared_ptr<SphericalBillboard> Scene::addSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    auto sph_billboard = ResourceManager::makeSphBillboard(name, pos, size, texture_path);
    m_SphBillboardMap.emplace(name, sph_billboard);

    return sph_billboard;
}

std::shared_ptr<Model> Scene::copyModel(std::string name, const std::shared_ptr<Model> model)
{
    auto c_model = std::make_shared<Model>(*model);
    m_ModelMap.emplace(name, c_model);

    return c_model;
}

std::shared_ptr<CylindricalBillboard> Scene::copyCylBillboard(std::string name, const std::shared_ptr<CylindricalBillboard> cyl_billboard)
{
    auto c_cyl_billboard = std::make_shared<CylindricalBillboard>(*cyl_billboard);
    m_CylBillboardMap.emplace(name, c_cyl_billboard);

    return c_cyl_billboard;
}

std::shared_ptr<SphericalBillboard> Scene::copySphBillboard(std::string name, const std::shared_ptr<SphericalBillboard> sph_billboard)
{
    auto c_sph_billboard = std::make_shared<SphericalBillboard>(*sph_billboard);
    m_SphBillboardMap.emplace(name, c_sph_billboard);

    return c_sph_billboard;
}

void Scene::deleteModel(std::string name, std::shared_ptr<Model>& model)
{
    m_ModelMap.erase(name);
    model.reset();
}

void Scene::deleteCylBillboard(std::string name, std::shared_ptr<CylindricalBillboard>& cyl_billboard)
{
    m_CylBillboardMap.erase(name);
    cyl_billboard.reset();
}

void Scene::deleteSphBillboard(std::string name, std::shared_ptr<SphericalBillboard>& sph_billboard)
{
    m_SphBillboardMap.erase(name);
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

    this->addSphBillboard(point_light.m_name, point_light.m_pos, glm::vec2(1.0f), "textures/lightbulb.png");

    return light;
}

std::shared_ptr<SpotLight> Scene::addSpotLight(SpotLight& spot_light)
{
    std::shared_ptr<SpotLight> light = std::make_shared<SpotLight>(spot_light);
    this->m_SpotLights.push_back(light);

    this->addSphBillboard(spot_light.m_name, spot_light.m_pos, glm::vec2(1.0f), "textures/highlight.png");

    return light;
}

void Scene::printObjectsInMaps(ObjectType objectType)
{
    switch (objectType)
    {
    case ObjectType::SHADER:
    {
        std::map<std::string, std::shared_ptr<Shader>>::iterator it = m_ShaderMap.begin();
        while (it != m_ShaderMap.end())
        {
            std::cout << "shader \"" << it->first << "\" with id " << it->second->ID << std::endl;
            it++;
        }
        break;
    }
    case ObjectType::TEXTURE:
    {
        std::map<std::string, std::shared_ptr<Texture>>::iterator it = m_TextureMap.begin();
        while (it != m_TextureMap.end())
        {
            std::cout << "texture \"" << it->first << "\" with id " << it->second->ID << std::endl;
            it++;
        }
        break;
    }
    case ObjectType::MODEL:
    {
        std::map<std::string, std::shared_ptr<Model>>::iterator it = m_ModelMap.begin();
        while (it != m_ModelMap.end())
        {
            std::cout << "model \"" << it->first << "\"" << std::endl;
            it++;
        }
        break;
    }
    case ObjectType::CYL_BILLBOARD:
    {
        std::map<std::string, std::shared_ptr<CylindricalBillboard>>::iterator it = m_CylBillboardMap.begin();
        while (it != m_CylBillboardMap.end())
        {
            std::cout << "cylindrical billboard \"" << it->first << "\"" << std::endl;
            it++;
        }
        break;
    }
    case ObjectType::SPH_BILLBOARD:
    {
        std::map<std::string, std::shared_ptr<SphericalBillboard>>::iterator it = m_SphBillboardMap.begin();
        while (it != m_SphBillboardMap.end())
        {
            std::cout << "spherical billboard \"" << it->first << "\"" << std::endl;
            it++;
        }
        break;
    }
    default:
        std::cout << "the default output of the switch statement in the getObjectsInMaps() function" << std::endl;
    }
}