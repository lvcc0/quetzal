#include "resource_manager.h"

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

void ResourceManager::loadModel(const std::string& file_path)
{
    std::filesystem::path path(file_path);
    std::string name = path.filename().string();
    
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
    std::filesystem::path path(file_path);
    std::string name = path.filename().string();

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
    // TODO
}

void ResourceManager::loadModels(const std::string& dir_path)
{
    // TODO
}

void ResourceManager::loadTextures(const std::string& dir_path)
{
    // TODO
}

void ResourceManager::loadShaders(const std::string& dir_path)
{
    // AT NOW THERE ONLY ONE VERTEX FILE CAN BE IN THIS FOLDER 
    std::filesystem::path vert_path;
    std::vector<std::filesystem::path> frag_paths;

    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        std::filesystem::path path(entry.path());

        std::string file_name_with_extension = path.filename().string();
        std::string extension = path.extension().string();

        if (extension == VERTEX_FILE_EXTENSION)
            vert_path = dir_path + "/" + file_name_with_extension;
        else if (extension == FRAGMENT_FILE_EXTENSION)
            frag_paths.push_back(dir_path + "/" + file_name_with_extension);
    }

    for (const auto& frag_path : frag_paths)
    {
        std::filesystem::path path(frag_path);
        m_LoadedScreenShaders.emplace(path.filename().string(), makeShaderProgram(vert_path.string(), frag_path.string()));
    }
}

std::shared_ptr<Shader> ResourceManager::makeShaderProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
    if (m_LoadedShaders.find(vertex_shader_path + "||" + fragment_shader_path) != m_LoadedShaders.end())
        return m_LoadedShaders.at(vertex_shader_path + "||" + fragment_shader_path);

    // Getting shader files (.vert and .frag)
    std::string vertex_shader_src = getFileString(vertex_shader_path);
    std::string fragment_shader_src = getFileString(fragment_shader_path);

    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertex_shader_src, fragment_shader_src);

    m_LoadedShaders.emplace(vertex_shader_path + "||" + fragment_shader_path, shader);
    return shader;
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& name)
{
    if (m_LoadedTextures.find(name) != m_LoadedTextures.end())
        return std::make_shared<Texture>(m_LoadedTextures.at(name));

    std::cerr << "TEXTURE WITH NAME:: " << name << " NOT LOADED" << std::endl;
    ASSERT(false);
}

Model ResourceManager::getModel(const std::string& name)
{
    if (m_LoadedModels.find(name) != m_LoadedModels.end())
        return m_LoadedModels.at(name);

    std::cerr << "MODEL WITH NAME:: " << name << " NOT LOADED" << std::endl;
    ASSERT(false);
}

CylindricalBillboard ResourceManager::makeCylBillboard(glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts)
{
    auto texture = getTexture(texture_name);
    texture->m_Type = "texture_diffuse";

    return CylindricalBillboard(pos, size, texture, verts);
}

SphericalBillboard ResourceManager::makeSphBillboard(glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts)
{
    auto texture = getTexture(texture_name);
    texture->m_Type = "texture_diffuse";

    return SphericalBillboard(pos, size, texture, verts);
}

void ResourceManager::displayLoadedObjects()
{
    for (auto item : m_LoadedObjects)
        std::cout << item.first << std::endl;
}

void ResourceManager::preLoadResources()
{
    // TODO: redo this without exclusive folder names, make it loop through all the res_path directory and load everything there

    std::string model_folder_path = relResPath + MODELS_FOLDER;
    std::string shaders_folder_path = relResPath + SHADERS_FOLDER;
    std::string textures_folder_path = relResPath + TEXTURES_FOLDER;
    std::string screen_shaders_folder_path = relResPath + SCREEN_SHADERS_FOLDER;

    // Loading textures
    for (const auto& entry : std::filesystem::directory_iterator(textures_folder_path))
    {
        std::filesystem::path file_path = entry.path();

        std::string extension = file_path.extension().string();
        if (std::find(TEXTURES_FILE_EXTENSIONS.begin(), TEXTURES_FILE_EXTENSIONS.end(), extension) != TEXTURES_FILE_EXTENSIONS.end())
            loadTexture(file_path.string());
    }

    // Loading models
    for (const auto& entry : std::filesystem::directory_iterator(model_folder_path))
    {
        std::filesystem::path folder_path = entry.path();

        for (const auto& file : std::filesystem::directory_iterator(folder_path))
        {
            std::filesystem::path file_path = file.path();

            std::string extension = file_path.extension().string();
            if (extension == MODELS_MAIN_FILE_EXTENSION)
                loadModel(file_path.string());
        }
    }

    // Loading screen shaders
    loadShaders(screen_shaders_folder_path);
}