#include "static/resource_manager.h"

void ResourceManager::loadMesh(const std::string& file_path)
{
    // do not load new mesh if it already exists
    if (m_LoadedMeshes.find(file_path) != m_LoadedMeshes.end())
    {
        std::cerr << "MESH AT PATH:: " << file_path << " ALREADY LOADED" << std::endl;
        return;
    }

    m_LoadedMeshes.emplace(file_path, std::make_shared<qtzl::Mesh>(std::filesystem::path(file_path).filename().string(), file_path));
}

void ResourceManager::loadTexture(const std::string& file_path, const std::string& texture_type)
{
    // do not load new texture if it already exists
    if (m_LoadedTextures.find(file_path) != m_LoadedTextures.end())
    {
        std::cerr << "TEXTURE AT PATH:: " << file_path << " ALREADY LOADED" << std::endl;
        return;
    }

    m_LoadedTextures.emplace(file_path, std::make_shared<qtzl::Texture>(std::filesystem::path(file_path).filename().string(), file_path, texture_type));
}

void ResourceManager::loadShader(const std::string& file_path)
{
    std::filesystem::path path(file_path);

    std::string name = path.filename().string();
    std::string extension = path.extension().string();

    // do not load new shader if it already exists
    if (m_LoadedShaders.find(file_path) != m_LoadedShaders.end())
    {
        std::cerr << "SHADER AT PATH:: " << file_path << " ALREADY LOADED" << std::endl;
        return;
    }

    GLenum shaderType{};

    if (extension == VERTEX_SHADER_FILE_EXTENSION)
        shaderType = GL_VERTEX_SHADER;
    else if (extension == FRAGMENT_SHADER_FILE_EXTENSION)
        shaderType = GL_FRAGMENT_SHADER;

    m_LoadedShaders.emplace(file_path, std::make_shared<qtzl::Shader>(name, file_path, getFileString(file_path), shaderType));
}

std::shared_ptr<qtzl::Mesh> ResourceManager::getMesh(const std::string& file_path)
{
    std::string finalPath = (file_path.starts_with(RES_PATH)) ? file_path : RES_PATH + file_path;

    if (m_LoadedMeshes.find(finalPath) != m_LoadedMeshes.end())
        return m_LoadedMeshes.at(finalPath);

    std::cerr << "MESH AT PATH:: " << finalPath << " NOT LOADED" << std::endl;
}

std::shared_ptr<qtzl::Texture> ResourceManager::getTexture(const std::string& file_path)
{
    std::string finalPath = (file_path.starts_with(RES_PATH)) ? file_path : RES_PATH + file_path;

    if (m_LoadedTextures.find(finalPath) != m_LoadedTextures.end())
        return m_LoadedTextures.at(finalPath);

    std::cerr << "TEXTURE AT PATH:: " << finalPath << " NOT LOADED" << std::endl;
}

std::shared_ptr<qtzl::Shader> ResourceManager::getShader(const std::string& file_path)
{
    std::string finalPath = (file_path.starts_with(RES_PATH)) ? file_path : RES_PATH + file_path;

    if (m_LoadedShaders.find(finalPath) != m_LoadedShaders.end())
        return m_LoadedShaders.at(finalPath);

    std::cerr << "SHADER AT PATH:: " << finalPath << " NOT LOADED" << std::endl;
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

std::shared_ptr<ShaderProgram> ResourceManager::createShaderProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path, ShaderProgram::Type type)
{
    std::string name = std::filesystem::path(vertex_shader_path).filename().string() + "||" + std::filesystem::path(fragment_shader_path).filename().string();

    // do not load new shader program if it already exists
    if (auto iterator = std::find_if(m_ShaderPrograms.begin(), m_ShaderPrograms.end(), [&name](std::shared_ptr<ShaderProgram> shader_program) { return shader_program->getName() == name; }); iterator != m_ShaderPrograms.end())
        return *iterator;

    std::string finalVertShaderPath = (vertex_shader_path.starts_with(RES_PATH)) ? vertex_shader_path : RES_PATH + vertex_shader_path;
    std::string finalFragShaderPath = (fragment_shader_path.starts_with(RES_PATH)) ? fragment_shader_path : RES_PATH + fragment_shader_path;

    std::shared_ptr<ShaderProgram> shaderProgram = std::make_shared<ShaderProgram>(name, m_LoadedShaders.at(finalVertShaderPath)->getID(), m_LoadedShaders.at(finalFragShaderPath)->getID(), type);
    m_ShaderPrograms.push_back(shaderProgram);

    return shaderProgram;
}

std::vector<std::shared_ptr<ShaderProgram>> ResourceManager::createPPShaderPrograms()
{
    std::string vertShaderPath;
    std::vector<std::string> fragShaderPaths;

    for (const auto& entry : m_LoadedShaders)
    {
        if (!entry.second->getName().starts_with("pp_"))
            continue; // not a postprocessing shader

        if (entry.first.ends_with(VERTEX_SHADER_FILE_EXTENSION))
        {
            vertShaderPath = entry.first;
            continue;
        }
        if (entry.first.ends_with(FRAGMENT_SHADER_FILE_EXTENSION))
        {
            fragShaderPaths.push_back(entry.first);
            continue;
        }
    }

    for (const auto& fragShaderPath : fragShaderPaths)
    {
        std::string name = std::filesystem::path(vertShaderPath).filename().string() + "||" + std::filesystem::path(fragShaderPath).filename().string();

        auto result = std::find_if(m_PPShaderPrograms.begin(), m_PPShaderPrograms.end(), [&name](std::shared_ptr<ShaderProgram> shader_program) { return shader_program->getName() == name; });
        if (result == m_PPShaderPrograms.end())
            m_PPShaderPrograms.push_back(std::make_shared<ShaderProgram>(name, m_LoadedShaders.at(vertShaderPath)->getID(), m_LoadedShaders.at(fragShaderPath)->getID()));
    }

    return m_PPShaderPrograms;
}

std::shared_ptr<ShaderProgram> ResourceManager::getShaderProgram(const std::string& name)
{
    // NOTE: i'm not sure if this will work
    auto result = std::find_if(m_ShaderPrograms.begin(), m_ShaderPrograms.end(), [&name](std::shared_ptr<ShaderProgram> shader_program) { return shader_program->getName() == name; });
    return *result;
}

std::shared_ptr<ShaderProgram> ResourceManager::getPPShaderProgram(const std::string& name)
{
    // NOTE: same
    auto result = std::find_if(m_PPShaderPrograms.begin(), m_PPShaderPrograms.end(), [&name](std::shared_ptr<ShaderProgram> shader_program) { return shader_program->getName() == name; });
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

unsigned int ResourceManager::loadCubemap(const std::string& dir_path)
{
    std::string finalPath = (dir_path.starts_with(RES_PATH)) ? dir_path : RES_PATH + dir_path;
    std::vector<std::string> faces;

    // Replace "\" with "/" in the path
    size_t idx = 0;
    while (true) {
        idx = finalPath.find("\\", idx);

        if (idx == std::string::npos)
            break;

        finalPath.replace(idx, 1, "/");

        idx++;
    }

    unsigned int i = 0;
    for (const auto& entry : std::filesystem::directory_iterator(finalPath))
    {
        // We load only the first 6 images in the given directory
        if (i == 6)
            break;

        faces.push_back(entry.path().string());
    }

    unsigned int textureID;
    int width, height, nrChannels;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

        if (data)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else
            std::cerr << "CUBEMAP TEXTURE FAILED TO LOAD AT PATH:: " << faces[i] << std::endl;

        stbi_image_free(data);
    }

    stbi_set_flip_vertically_on_load(true);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_Cubemaps.push_back(textureID);
    return textureID;
}

void ResourceManager::preloadResources()
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(RES_PATH))
    {
        if (entry.is_directory())
            continue;

        std::string path = entry.path().string();
        std::string extension = entry.path().extension().string();

        // Replace "\" with "/" in the path
        size_t idx = 0;
        while (true) {
            idx = path.find("\\", idx);

            if (idx == std::string::npos)
                break;

            path.replace(idx, 1, "/");

            idx++;
        }

        if (extension == MESH_FILE_EXTENSION)
        {
            loadMesh(path);
            continue;
        }
        if (std::find(TEXTURE_FILE_EXTENSIONS.begin(), TEXTURE_FILE_EXTENSIONS.end(), extension) != TEXTURE_FILE_EXTENSIONS.end())
        {
            loadTexture(path, "texture_diffuse");
            continue;
        }
        if (extension == VERTEX_SHADER_FILE_EXTENSION || extension == FRAGMENT_SHADER_FILE_EXTENSION)
        {
            loadShader(path);
            continue;
        }
    }
}

bool ResourceManager::isLoaded(const std::string& file_path)
{
    std::string path = file_path;

    // Replace "\" with "/"
    if (file_path.find("\\") != std::string::npos)
    {
        size_t idx = 0;
        while (true) {
            idx = path.find("\\", idx);

            if (idx == std::string::npos)
                break;

            path.replace(idx, 1, "/");

            idx++;
        }
    }

    if (m_LoadedMeshes.find(path) != m_LoadedMeshes.end())
        return true;

    if (m_LoadedTextures.find(path) != m_LoadedTextures.end())
        return true;

    if (m_LoadedShaders.find(path) != m_LoadedShaders.end())
        return true;

    return false;
}

std::string ResourceManager::getType(const std::string& file_path)
{
    std::string extension = std::filesystem::path(file_path).filename().extension().string();

    if (extension == MESH_FILE_EXTENSION)
        return "Mesh";
    if (std::find(TEXTURE_FILE_EXTENSIONS.begin(), TEXTURE_FILE_EXTENSIONS.end(), extension) != TEXTURE_FILE_EXTENSIONS.end())
        return "Texture";
    if (extension == VERTEX_SHADER_FILE_EXTENSION)
        return "Vert Shader";
    if (extension == FRAGMENT_SHADER_FILE_EXTENSION)
        return "Frag Shader";

    return extension + " File";
}

std::string ResourceManager::getFileString(const std::string& file_path)
{
    std::string finalPath = (file_path.starts_with(RES_PATH)) ? file_path : RES_PATH + file_path;

    std::ifstream file(finalPath, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "FILE AT PATH:: " << finalPath << " FAILED TO LOAD" << std::endl;
        return {};
    }

    // return file contents as a string
    std::stringstream readbuffer;
    readbuffer << file.rdbuf();
    return readbuffer.str();
}