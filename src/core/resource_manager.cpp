#include "core/resource_manager.h"

void ResourceManager::loadMesh(const std::string& file_path)
{
    std::string name = std::filesystem::path(file_path).filename().string();
    
    // do not load new mesh if it already exists
    if (m_LoadedMeshes.find(name) != m_LoadedMeshes.end())
    {
        std::cerr << "MESH WITH NAME:: " << name << " ALREADY LOADED" << std::endl;
        return;
    }

    m_LoadedMeshes.emplace(name, std::make_shared<qtzl::Mesh>(name, file_path));
}

void ResourceManager::loadTexture(const std::string& file_path, const std::string& texture_type)
{
    std::string name = std::filesystem::path(file_path).filename().string();

    // do not load new texture if it already exists
    if (m_LoadedTextures.find(name) != m_LoadedTextures.end())
    {
        std::cerr << "TEXTURE WITH NAME:: " << name << " ALREADY LOADED" << std::endl;
        return;
    }

    m_LoadedTextures.emplace(name, std::make_shared<qtzl::Texture>(name, file_path, texture_type));
}

void ResourceManager::loadShader(const std::string& file_path)
{
    std::filesystem::path path(file_path);

    std::string name = path.filename().string();
    std::string extension = path.extension().string();

    // do not load new shader if it already exists
    if (m_LoadedShaders.find(name) != m_LoadedShaders.end())
    {
        std::cerr << "SHADER WITH NAME:: " << name << " ALREADY LOADED" << std::endl;
        return;
    }

    GLenum shaderType{};

    if (extension == VERTEX_SHADER_FILE_EXTENSION)
        shaderType = GL_VERTEX_SHADER;
    else if (extension == FRAGMENT_SHADER_FILE_EXTENSION)
        shaderType = GL_FRAGMENT_SHADER;

    m_LoadedShaders.emplace(name, std::make_shared<qtzl::Shader>(name, file_path, getFileString(file_path), shaderType));
}

void ResourceManager::loadMeshes(const std::string& dir_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        if (entry.path().extension().string() == MESH_MAIN_FILE_EXTENSION)
            loadMesh(entry.path().string());
    }
}

void ResourceManager::loadTextures(const std::string& dir_path, const std::string& texture_type)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        if (std::find(TEXTURES_FILE_EXTENSIONS.begin(), TEXTURES_FILE_EXTENSIONS.end(), entry.path().extension().string()) != TEXTURES_FILE_EXTENSIONS.end())
            loadTexture(entry.path().string(), texture_type);
    }
}

void ResourceManager::loadShaders(const std::string& dir_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        if (entry.path().extension().string() == VERTEX_SHADER_FILE_EXTENSION || entry.path().extension().string() == FRAGMENT_SHADER_FILE_EXTENSION)
            loadShader(entry.path().string());
    }
}

std::shared_ptr<qtzl::Mesh> ResourceManager::getMesh(const std::string& name)
{
    if (m_LoadedMeshes.find(name) != m_LoadedMeshes.end())
        return m_LoadedMeshes.at(name);

    std::cerr << "MESH WITH NAME:: " << name << " NOT LOADED" << std::endl;
    //ASSERT(false);
}

std::shared_ptr<qtzl::Texture> ResourceManager::getTexture(const std::string& name)
{
    if (m_LoadedTextures.find(name) != m_LoadedTextures.end())
        return m_LoadedTextures.at(name);

    std::cerr << "TEXTURE WITH NAME:: " << name << " NOT LOADED" << std::endl;
    //ASSERT(false);
}

std::shared_ptr<qtzl::Shader> ResourceManager::getShader(const std::string& name)
{
    if (m_LoadedShaders.find(name) != m_LoadedShaders.end())
        return m_LoadedShaders.at(name);

    std::cerr << "SHADER WITH NAME:: " << name << " NOT LOADED" << std::endl;
    //ASSERT(false);
}

std::map<const std::string, std::shared_ptr<qtzl::Mesh>> ResourceManager::getMeshes()
{
    return m_LoadedMeshes;
}

std::map<const std::string, std::shared_ptr<qtzl::Texture>> ResourceManager::getTextures()
{
    return m_LoadedTextures;
}

std::map<const std::string, std::shared_ptr<qtzl::Shader>> ResourceManager::getShaders()
{
    return m_LoadedShaders;
}

std::shared_ptr<ShaderProgram> ResourceManager::createShaderProgram(const std::string& vertex_shader_name, const std::string& fragment_shader_name)
{
    std::string name = vertex_shader_name + "||" + fragment_shader_name;

    if (auto iterator = std::find_if(m_ShaderPrograms.begin(), m_ShaderPrograms.end(), [name](std::shared_ptr<ShaderProgram> shader_program) { return shader_program->getName() == name; }); iterator != m_ShaderPrograms.end())
        return *iterator;

    std::shared_ptr<ShaderProgram> shaderProgram = std::make_shared<ShaderProgram>(name, m_LoadedShaders.at(vertex_shader_name)->getID(), m_LoadedShaders.at(fragment_shader_name)->getID());
    m_ShaderPrograms.push_back(shaderProgram);

    return shaderProgram;
}

std::vector<std::shared_ptr<ShaderProgram>> ResourceManager::createPPShaderPrograms()
{
    std::string vertexShaderName;
    std::vector<std::string> fragmentShaderNames;

    for (const auto& entry : m_LoadedShaders)
    {
        // kinda lame, we'll rewrite this stuff
        if (entry.second->getPath().find(PPSHADERS_FOLDER) == std::string::npos)
            continue; // not a postprocessing shader

        if (entry.first.find(VERTEX_SHADER_FILE_EXTENSION) != std::string::npos)
            vertexShaderName = entry.first;
        else if (entry.first.find(FRAGMENT_SHADER_FILE_EXTENSION) != std::string::npos)
            fragmentShaderNames.push_back(entry.first);
    }

    for (const auto& fragmentShaderName : fragmentShaderNames)
    {
        std::string name = vertexShaderName + "||" + fragmentShaderName;
        auto iterator = std::find_if(m_PPShaderPrograms.begin(), m_PPShaderPrograms.end(), [name](std::shared_ptr<ShaderProgram> shader_program) { return shader_program->getName() == name; });

        if (iterator == m_PPShaderPrograms.end())
            m_PPShaderPrograms.push_back(std::make_shared<ShaderProgram>(name, m_LoadedShaders.at(vertexShaderName)->getID(), m_LoadedShaders.at(fragmentShaderName)->getID()));
    }

    return m_PPShaderPrograms;
}

std::shared_ptr<ShaderProgram> ResourceManager::getShaderProgram(const std::string& name)
{
    // NOTE: i'm not sure if this will work
    auto result = std::find_if(m_ShaderPrograms.begin(), m_ShaderPrograms.end(), [name](std::shared_ptr<ShaderProgram> shader_program) { return shader_program->getName() == name; });
    return *result;
}

std::shared_ptr<ShaderProgram> ResourceManager::getPPShaderProgram(const std::string& name)
{
    // NOTE: same
    auto result = std::find_if(m_PPShaderPrograms.begin(), m_PPShaderPrograms.end(), [name](std::shared_ptr<ShaderProgram> shader_program) { return shader_program->getName() == name; });
    return *result;
}

std::vector<std::shared_ptr<ShaderProgram>> ResourceManager::getShaderPrograms()
{
    return m_ShaderPrograms;
}

std::vector<std::shared_ptr<ShaderProgram>> ResourceManager::getPPShaderPrograms()
{
    return m_PPShaderPrograms;
}

void ResourceManager::preLoadResources()
{
    // TODO: redo this without exclusive folder names, make it loop through all the res_path directory and load everything there

    loadMeshes(RES_PATH + MESHES_FOLDER);
    loadTextures(RES_PATH + TEXTURES_FOLDER, "texture_diffuse");
    loadShaders(RES_PATH + SHADERS_FOLDER);
    loadShaders(RES_PATH + PPSHADERS_FOLDER);
}

std::string ResourceManager::getFileString(const std::string& file_path)
{
    std::string tempFilePath = file_path;

    // check if file_path is relative
    if (tempFilePath.find(RES_PATH) == std::string::npos)
        tempFilePath = RES_PATH + file_path;

    std::ifstream file(tempFilePath, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "FILE AT PATH:: " << tempFilePath << " FAILED TO LOAD" << std::endl;
        return {};
    }

    // return file contents as a string
    std::stringstream readbuffer;
    readbuffer << file.rdbuf();
    return readbuffer.str();
}