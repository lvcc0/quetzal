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

    m_LoadedMeshes.emplace(name, qtzl::Mesh(name, file_path));
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

    m_LoadedTextures.emplace(name, qtzl::Texture(name, file_path, texture_type));
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

    if (extension == VERTEX_FILE_EXTENSION)
        shaderType = GL_VERTEX_SHADER;
    else if (extension == FRAGMENT_FILE_EXTENSION)
        shaderType = GL_FRAGMENT_SHADER;

    m_LoadedShaders.emplace(name, qtzl::Shader(name, file_path, getFileString(file_path), shaderType));
}

void ResourceManager::loadMeshes(const std::string& dir_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        if (entry.path().extension().string() == MESH_MAIN_FILE_EXTENSION)
            loadMesh(entry.path().string());
    }
}

void ResourceManager::loadTextures(const std::string& dir_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        if (std::find(TEXTURES_FILE_EXTENSIONS.begin(), TEXTURES_FILE_EXTENSIONS.end(), entry.path().extension().string()) != TEXTURES_FILE_EXTENSIONS.end())
            loadTexture(entry.path().string());
    }
}

void ResourceManager::loadShaders(const std::string& dir_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        if (entry.path().extension().string() == VERTEX_FILE_EXTENSION || entry.path().extension().string() == FRAGMENT_FILE_EXTENSION)
            loadShader(entry.path().string());
    }
}

std::shared_ptr<qtzl::Mesh> ResourceManager::getMesh(const std::string& name)
{
    if (m_LoadedMeshes.find(name) != m_LoadedMeshes.end())
        return std::make_shared<qtzl::Mesh>(m_LoadedMeshes.at(name));

    std::cerr << "MESH WITH NAME:: " << name << " NOT LOADED" << std::endl;
    ASSERT(false);
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& name)
{
    if (m_LoadedTextures.find(name) != m_LoadedTextures.end())
        return std::make_shared<qtzl::Texture>(m_LoadedTextures.at(name));

    std::cerr << "TEXTURE WITH NAME:: " << name << " NOT LOADED" << std::endl;
    ASSERT(false);
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string& name)
{
    if (m_LoadedShaders.find(name) != m_LoadedShaders.end())
        return std::make_shared<qtzl::Shader>(m_LoadedShaders.at(name));

    std::cerr << "SHADER WITH NAME:: " << name << " NOT LOADED" << std::endl;
    ASSERT(false);
}

void ResourceManager::preLoadResources()
{
    // TODO: redo this without exclusive folder names, make it loop through all the res_path directory and load everything there

    loadMeshes(relResPath + MODELS_FOLDER);
    loadTextures(relResPath + TEXTURES_FOLDER);
    loadShaders(relResPath + SHADERS_FOLDER);
}

std::string ResourceManager::getFileString(const std::string& file_path)
{
    std::string tempFilePath = file_path;

    // check if file_path is relative
    if (tempFilePath.find(relResPath) == std::string::npos)
        tempFilePath = relResPath + file_path.c_str();

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