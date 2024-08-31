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

    // Default shader
    setShader("default_shader", ShaderType::MAIN);
    std::shared_ptr<Shader> currentMainShader = m_CurrentShaders[to_underlying(ShaderType::MAIN)];

    currentMainShader->setVec3("viewPos", this->m_Camera->m_pos);
    currentMainShader->setFloat("material.shininess", 32.0f);

    currentMainShader->setMat4("projection", m_ProjectionMatrix);
    currentMainShader->setMat4("view", view);

    // Stencil shader
    setShader("stencil_shader", ShaderType::STENCIL);
    std::shared_ptr<Shader> currentStencilShader = m_CurrentShaders[to_underlying(ShaderType::STENCIL)];

    currentStencilShader->setMat4("projection", m_ProjectionMatrix);
    currentStencilShader->setMat4("view", view);

    // Rendering
    setShader("default_shader", ShaderType::MAIN);

    // Vector of all working shaders(except screen shaders)
    std::vector<std::shared_ptr<Shader>> curr_shaders{ currentMainShader, currentStencilShader };

    // Rendering lights' influence
    if (!this->m_DirLights.empty())
    {
        for (auto i = 0; i < this->m_DirLights.size(); i++)
        {
            this->m_DirLights[i]->updateUni(currentMainShader, i);
        }
    }
    if (!this->m_PointLights.empty())
    {
        for (auto i = 0; i < this->m_PointLights.size(); i++)
        {
            this->m_PointLights[i]->updateUni(currentMainShader, i);

            std::shared_ptr<SphericalBillboard> sph_bill = std::static_pointer_cast<SphericalBillboard>(this->m_RenderableMap.at(m_PointLights[i]->m_name));

            sph_bill->translate(this->m_PointLights[i]->m_pos);
            sph_bill->m_Target = m_Camera->m_pos;
            sph_bill->draw(curr_shaders);
        }
    }
    if (!this->m_SpotLights.empty())
    {
        for (auto i = 0; i < this->m_SpotLights.size(); i++)
        {
            this->m_SpotLights[i]->updateUni(currentMainShader, i);
            
            std::shared_ptr<SphericalBillboard> sph_bill = std::static_pointer_cast<SphericalBillboard>(this->m_RenderableMap.at(m_PointLights[i]->m_name));

            sph_bill->translate(this->m_SpotLights[i]->m_pos);
            sph_bill->m_Target = m_Camera->m_pos;
            sph_bill->draw(curr_shaders);
        }
    }
    // Draw all renderable
    if (!this->m_RenderableMap.empty())
    {
        std::map<std::string, std::shared_ptr<Renderable>>::iterator it = this->m_RenderableMap.begin();
        while (it != this->m_RenderableMap.end())
        {
            if (it->second->type == RenderableType::SPH_BILL) 
            {
                std::shared_ptr<SphericalBillboard> sph_bill = std::static_pointer_cast<SphericalBillboard>(it->second);
                sph_bill->m_Target = m_Camera->m_pos;
                sph_bill->draw(curr_shaders);
                it++;
            }
            else if (it->second->type == RenderableType::CYL_BILL)
            {
                std::shared_ptr<CylindricalBillboard> cyl_bill = std::static_pointer_cast<CylindricalBillboard>(it->second);
                cyl_bill->m_Target = m_Camera->m_pos;
                cyl_bill->draw(curr_shaders);
                it++;
            }
            else 
            {
                it->second->draw(curr_shaders);
                it++;
            }
        }
    }
    if (!m_CurrentScreenShaders.empty() && m_IsPostProcessing)
        this->m_PostProcessing->activate(m_CurrentScreenShaders);
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

void Scene::setShader(const std::string& name, ShaderType type)
{
    if (m_ShaderMap.find(name) != m_ShaderMap.end()) {
        switch (type)
        {
        case (ShaderType::MAIN):
            this->m_CurrentShaders[to_underlying(ShaderType::MAIN)] = this->m_ShaderMap.at(name);
            this->m_CurrentShaders[to_underlying(ShaderType::MAIN)]->activateShader();
            break;

        case (ShaderType::STENCIL):
            this->m_CurrentShaders[to_underlying(ShaderType::STENCIL)] = this->m_ShaderMap.at(name);
            this->m_CurrentShaders[to_underlying(ShaderType::STENCIL)]->activateShader();
            break;
        }
    }

    else
        std::cout << "ERROR::SHADER WITH NAME " << name << "NOT FOUND " << std::endl;
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
        if (entry.second == this->m_CurrentShaders[to_underlying(ShaderType::MAIN)])
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

std::map<const std::string, std::shared_ptr<Renderable>> Scene::getRenderableMap()
{
    return this->m_RenderableMap;
}

std::shared_ptr<Shader> Scene::addShader(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path)
{
    auto shader = ResourceManager::makeShaderProgram(name, vertex_shader_rel_path, fragment_shader_rel_path);
    
    if (m_ShaderMap.empty()) // automatically set this shader if it's the first one in the map
        this->m_CurrentShaders[to_underlying(ShaderType::MAIN)] = shader;

    m_ShaderMap.emplace(name, shader);
    return shader;
}

std::shared_ptr<Texture> Scene::addTexture(std::string name, std::string type, const std::string& texture_rel_path)
{
    auto texture = ResourceManager::makeTexture(type, texture_rel_path);
    m_TextureMap.emplace(name, texture);

    return texture;
}

std::shared_ptr<Model> Scene::addModel(std::string name, const std::string& model_rel_path)
{
    auto model = ResourceManager::makeModel(model_rel_path);
    m_RenderableMap.emplace(name, model);

    return model;
}

std::shared_ptr<RigidBody> Scene::addRigidBody(std::string name, const std::string& model_rel_path, Collision &collision)
{
    auto rigid_body = std::make_shared<RigidBody>(ResourceManager::makeModel(model_rel_path), collision);
    m_RenderableMap.emplace(name, rigid_body);
    
    return rigid_body;
}

std::shared_ptr<CylindricalBillboard> Scene::addCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path, std::vector<Vertex> verts)
{
    auto cyl_billboard = ResourceManager::makeCylBillboard(pos, size, texture_path, verts);
    m_RenderableMap.emplace(name, cyl_billboard);

    return cyl_billboard;
}

std::shared_ptr<SphericalBillboard> Scene::addSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path, std::vector<Vertex> verts)
{
    auto sph_billboard = ResourceManager::makeSphBillboard(pos, size, texture_path, verts);
    m_RenderableMap.emplace(name, sph_billboard);

    return sph_billboard;
}

std::shared_ptr<Model> Scene::copyModel(std::string name, const std::shared_ptr<Model> model)
{
    auto c_model = std::make_shared<Model>(*model);
    m_RenderableMap.emplace(name, c_model);

    return c_model;
}

std::shared_ptr<CylindricalBillboard> Scene::copyCylBillboard(std::string name, const std::shared_ptr<CylindricalBillboard> cyl_billboard)
{
    auto c_cyl_billboard = std::make_shared<CylindricalBillboard>(*cyl_billboard);
    m_RenderableMap.emplace(name, c_cyl_billboard);

    return c_cyl_billboard;
}

std::shared_ptr<SphericalBillboard> Scene::copySphBillboard(std::string name, const std::shared_ptr<SphericalBillboard> sph_billboard)
{
    auto c_sph_billboard = std::make_shared<SphericalBillboard>(*sph_billboard);
    m_RenderableMap.emplace(name, c_sph_billboard);

    return c_sph_billboard;
}

void Scene::deleteRenderable(std::string name, std::shared_ptr<Renderable>& renderable_object)
{
    m_RenderableMap.erase(name);
    renderable_object.reset();
}

std::shared_ptr<DirLight> Scene::addDirLight(DirLight& dir_light)
{
    std::shared_ptr<DirLight> light = std::make_shared<DirLight>(dir_light);
    this->m_DirLights.push_back(light);

    return light;
}

std::shared_ptr<PointLight> Scene::addPointLight(PointLight& point_light, std::vector<Vertex> verts)
{
    std::shared_ptr<PointLight> light = std::make_shared<PointLight>(point_light);
    this->m_PointLights.push_back(light);

    this->addSphBillboard(point_light.m_name, point_light.m_pos, glm::vec2(1.0f), "textures/lightbulb.png", verts);

    return light;
}

std::shared_ptr<SpotLight> Scene::addSpotLight(SpotLight& spot_light, std::vector<Vertex> verts)
{
    std::shared_ptr<SpotLight> light = std::make_shared<SpotLight>(spot_light);
    this->m_SpotLights.push_back(light);

    this->addSphBillboard(spot_light.m_name, spot_light.m_pos, glm::vec2(1.0f), "textures/highlight.png", verts);

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
