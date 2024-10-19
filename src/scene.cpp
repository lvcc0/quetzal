#include "scene.h"

Scene::Scene(Camera& camera)
    : m_Camera(std::make_shared<Camera>(camera)),
      m_PostProcessing(std::make_shared<PostProcessing>(ResourceManager::makePostProcessingShaders(), camera.m_width, camera.m_height))
{ /* empty */ }

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

            std::shared_ptr<SphericalBillboard> sph_bill = std::static_pointer_cast<SphericalBillboard>(this->m_RenderableMap.at(m_PointLights[i]->m_name));

            sph_bill->translate(this->m_PointLights[i]->m_pos);
            sph_bill->m_Target = m_Camera->m_pos;
            sph_bill->draw(shaders_active);
        }
    }
    if (!this->m_SpotLights.empty())
    {
        for (auto i = 0; i < this->m_SpotLights.size(); i++)
        {
            this->m_SpotLights[i]->updateUni(shaders_active.MAIN_SHADER, i);
            
            std::shared_ptr<SphericalBillboard> sph_bill = std::static_pointer_cast<SphericalBillboard>(this->m_RenderableMap.at(m_PointLights[i]->m_name));

            sph_bill->translate(this->m_SpotLights[i]->m_pos);
            sph_bill->m_Target = m_Camera->m_pos;
            sph_bill->draw(shaders_active);
        }
    }
    // Draw all renderable
    if (!this->m_RenderableMap.empty())
    {
        std::map<std::string, std::shared_ptr<Renderable>>::iterator it = this->m_RenderableMap.begin();
        while (it != this->m_RenderableMap.end())
        {
            auto object = it->second;
            if (typeid(*object) == typeid(SphericalBillboard))
            {
                std::shared_ptr<SphericalBillboard> sph_bill = std::static_pointer_cast<SphericalBillboard>(it->second);
                sph_bill->m_Target = m_Camera->m_pos;
                sph_bill->draw(shaders_active);
                it++;
            }
            else if (typeid(*object) == typeid(CylindricalBillboard))
            {
                std::shared_ptr<CylindricalBillboard> cyl_bill = std::static_pointer_cast<CylindricalBillboard>(it->second);
                cyl_bill->m_Target = m_Camera->m_pos;
                cyl_bill->draw(shaders_active);
                it++;
            }
            else 
            {
                it->second->draw(shaders_active);
                it++;
            }
        }
    }
    if (!m_PostProcessing->m_ActiveShaders.empty() && m_IsPostProcessing)
        this->m_PostProcessing->activate();
}

void Scene::doPhysicsProcessing()
{
    // TODO::Change this
    Physics::processPhysics(m_CollisionMap);
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

Shaders_pack Scene::getActiveShaders() const
{
    return this->shaders_active;
}

std::map<const std::string, std::shared_ptr<Texture>> Scene::getTextureMap() const
{
    return this->m_TextureMap;
}

std::map<const std::string, std::shared_ptr<Renderable>> Scene::getRenderableMap() const
{
    return this->m_RenderableMap;
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

    while (m_RenderableMap.find(name) != m_RenderableMap.end())
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

    while (m_RenderableMap.find(name) != m_RenderableMap.end())
    {
        i++;

        if (i == 1)
            name += std::to_string(i);
        else
            name = name.substr(0, name.size() - std::to_string(i - 1).length()) + std::to_string(i);
    }

    m_RenderableMap.emplace(name, model);
}

void Scene::addRigidBody(std::string name, Collision &collision)
{
    auto rigid_body = std::make_shared<RigidBody>(ResourceManager::makeModel(name), collision);
    
    unsigned int i{ 0 };

    while (m_RenderableMap.find(name) != m_RenderableMap.end())
    {
        i++;
        
        if (i == 1)
            name += std::to_string(i);
        else
            name = name.substr(0, name.size() - std::to_string(i-1).length()) + std::to_string(i);
    }

    m_RenderableMap.emplace(name, rigid_body);
}

void Scene::addCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts)
{
    auto cyl_billboard = std::make_shared<CylindricalBillboard>(ResourceManager::makeCylBillboard(pos, size, texture_name, verts));
    m_RenderableMap.emplace(name, cyl_billboard);
}

void Scene::addSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts)
{
    auto sph_billboard = std::make_shared<SphericalBillboard>(ResourceManager::makeSphBillboard(pos, size, texture_name, verts));
    m_RenderableMap.emplace(name, sph_billboard);
}

void Scene::copyModel(std::string name, const std::shared_ptr<Model> model)
{
    auto c_model = std::make_shared<Model>(*model);
    m_RenderableMap.emplace(name, c_model);
}

void Scene::copyCylBillboard(std::string name, const std::shared_ptr<CylindricalBillboard> cyl_billboard)
{
    auto c_cyl_billboard = std::make_shared<CylindricalBillboard>(*cyl_billboard);
    m_RenderableMap.emplace(name, c_cyl_billboard);
}

void Scene::copySphBillboard(std::string name, const std::shared_ptr<SphericalBillboard> sph_billboard)
{
    auto c_sph_billboard = std::make_shared<SphericalBillboard>(*sph_billboard);
    m_RenderableMap.emplace(name, c_sph_billboard);
}

void Scene::deleteRenderable(std::string name, std::shared_ptr<Renderable>& renderable_object)
{
    m_RenderableMap.erase(name);
    renderable_object.reset();
}

void Scene::addDirLight(DirLight& dir_light)
{
    std::shared_ptr<DirLight> light = std::make_shared<DirLight>(dir_light);
    this->m_DirLights.push_back(light);
}

void Scene::addPointLight(PointLight& point_light, std::vector<Vertex> verts)
{
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(point_light);
    this->m_PointLights.push_back(light);

    this->addSphBillboard(point_light.m_name, point_light.m_pos, glm::vec2(1.0f), "lightbulb", verts);
}

void Scene::addSpotLight(SpotLight& spot_light, std::vector<Vertex> verts)
{
    std::shared_ptr<SpotLight> light = std::make_shared<SpotLight>(spot_light);
    this->m_SpotLights.push_back(light);

    this->addSphBillboard(spot_light.m_name, spot_light.m_pos, glm::vec2(1.0f), "highlight", verts);
}

void Scene::printObjectsInMaps(ObjectType objectType)
{
    switch (objectType)
    {
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
    case ObjectType::RENDERABLE:
    {
        std::map<std::string, std::shared_ptr<Renderable>>::iterator it = m_RenderableMap.begin();
        while (it != m_RenderableMap.end())
        {
            std::cout << "renderable \"" << it->first << "\" with vao " << it->second->getVAO() << std::endl;
            it++;
        }
        break;
    }
    default:
        std::cout << "the default output of the switch statement in the getObjectsInMaps() function" << std::endl;
    }
}
