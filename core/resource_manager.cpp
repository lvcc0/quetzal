#include "resource_manager.h"

void ResourceManager::loadModel(const std::string& file_path)
{
    std::string name = std::filesystem::path(file_path).filename().string();
    
    // do not load new model if it already exists
    if (m_LoadedModels.find(name) != m_LoadedModels.end())
    {
        std::cerr << "MODEL WITH NAME:: " << name << " ALREADY LOADED" << std::endl;
        return;
    }

    m_LoadedModels.emplace(name, Model(file_path, true, name));
}

void ResourceManager::loadTexture(const std::string& file_path)
{
    std::string name = std::filesystem::path(file_path).filename().string();

    // do not load new texture if it already exists
    if (m_LoadedTextures.find(name) != m_LoadedTextures.end())
    {
        std::cerr << "TEXTURE WITH NAME:: " << name << " ALREADY LOADED" << std::endl;
        return;
    }

    m_LoadedTextures.emplace(name, Texture(file_path));
}

void ResourceManager::loadShader(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
    std::string name = vertex_shader_path + "||" + fragment_shader_path;

    // do not load new shader program if it already exists
    if (m_LoadedShaders.find(name) != m_LoadedShaders.end())
    {
        std::cerr << "SHADER WITH NAME:: " << name << " ALREADY LOADED" << std::endl;
        return;
    }

    m_LoadedShaders.emplace(name, makeShaderProgram(vertex_shader_path, fragment_shader_path));
}

void ResourceManager::loadPPShader(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
    // do not load new shader program if it already exists
    if (m_LoadedPPShaders.find(fragment_shader_path) != m_LoadedPPShaders.end())
    {
        std::cerr << "POSTPROCESSING SHADER WITH NAME:: " << fragment_shader_path << " ALREADY LOADED" << std::endl;
        return;
    }

    m_LoadedPPShaders.emplace(fragment_shader_path, makeShaderProgram(vertex_shader_path, fragment_shader_path));
}

void ResourceManager::loadModels(const std::string& dir_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        if (entry.path().extension().string() == MODELS_MAIN_FILE_EXTENSION)
            loadModel(entry.path().string());
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
    // NOTE: for every shader_name.vert shader there should be one shader_name.frag

    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        std::string filepath = entry.path().string();

        if (entry.path().extension().string() == VERTEX_FILE_EXTENSION) // kinda dumb but oh well
            loadShader(filepath, filepath.replace(filepath.find(VERTEX_FILE_EXTENSION), VERTEX_FILE_EXTENSION.length(), FRAGMENT_FILE_EXTENSION));
    }
}

void ResourceManager::loadPPShaders(const std::string& dir_path)
{
    std::string vertPath;
    std::vector<std::string> fragPaths;

    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        std::string filepath = entry.path().string();
        std::string extension = entry.path().extension().string();

        if (extension == VERTEX_FILE_EXTENSION)
            vertPath = filepath;
        else if (extension == FRAGMENT_FILE_EXTENSION)
            fragPaths.push_back(filepath);
    }

    for (const std::string& fragPath : fragPaths)
        loadPPShader(vertPath, fragPath);
}

std::shared_ptr<Model> ResourceManager::getModel(const std::string& name)
{
    if (m_LoadedModels.find(name) != m_LoadedModels.end())
        return std::make_shared<Model>(m_LoadedModels.at(name));

    std::cerr << "MODEL WITH NAME:: " << name << " NOT LOADED" << std::endl;
    ASSERT(false);
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& name)
{
    if (m_LoadedTextures.find(name) != m_LoadedTextures.end())
        return std::make_shared<Texture>(m_LoadedTextures.at(name));

    std::cerr << "TEXTURE WITH NAME:: " << name << " NOT LOADED" << std::endl;
    ASSERT(false);
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string& name)
{
    if (m_LoadedShaders.find(name) != m_LoadedShaders.end())
        return std::make_shared<Shader>(m_LoadedShaders.at(name));

    std::cerr << "SHADER WITH NAME:: " << name << " NOT LOADED" << std::endl;
    ASSERT(false);
}

std::shared_ptr<Shader> ResourceManager::getPPShader(const std::string& name)
{
    if (m_LoadedPPShaders.find(name) != m_LoadedPPShaders.end())
        return std::make_shared<Shader>(m_LoadedPPShaders.at(name));

    std::cerr << "POSTPROCESSING SHADER WITH NAME:: " << name << " NOT LOADED" << std::endl;
    ASSERT(false);
}

void ResourceManager::preLoadResources()
{
    // TODO: redo this without exclusive folder names, make it loop through all the res_path directory and load everything there

    loadModels(relResPath + MODELS_FOLDER);
    loadTextures(relResPath + TEXTURES_FOLDER);
    loadShaders(relResPath + SHADERS_FOLDER);
    loadPPShaders(relResPath + PPSHADERS_FOLDER);
}

std::string ResourceManager::getFileString(const std::string& file_path)
{
    std::string temp_file_path = file_path;

    // check if file_path is relative
    if (temp_file_path.find(relResPath) == std::string::npos)
        temp_file_path = relResPath + file_path.c_str();

    std::ifstream file(temp_file_path, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "FILE AT PATH:: " << temp_file_path << " FAILED TO LOAD" << std::endl;
        return {};
    }

    // return file contents as a string
    std::stringstream readbuffer;
    readbuffer << file.rdbuf();
    return readbuffer.str();
}

Shader ResourceManager::makeShaderProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
    // Getting shader files' contents (.vert and .frag)
    std::string vertex_shader_src = getFileString(vertex_shader_path);
    std::string fragment_shader_src = getFileString(fragment_shader_path);

    return Shader(vertex_shader_src, fragment_shader_src);
}