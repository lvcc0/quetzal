#include "resource_manager.h"

std::string ResourceManager::getFileString(const std::string& file_path)
{
    
    std::string temp_file_path = file_path;

    // catch if file_path is relative
    if (temp_file_path.find(relResPath) == std::string::npos)
        temp_file_path = relResPath + file_path.c_str();
    
    std::ifstream file(temp_file_path, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "File failed to open at: " << file_path << std::endl;
        return {};
    }
    
    // Returns file contents as a string
    std::stringstream readbuffer;
    readbuffer << file.rdbuf();
    return readbuffer.str();
}

void ResourceManager::loadModel(const std::string model_path)
{
    std::string name = model_path.substr(model_path.find_last_of("/") + 1, model_path.substr(model_path.find_last_of("/") + 1).size() - MODELS_MAIN_FILE_EXTENSION.size() - 1);

    if (m_LoadedModels.find(name) != m_LoadedModels.end())
        std::cout << "MODEL WITH NAME:: " << name << " ALREADY LOADED" << std::endl;

    else
    {
        std::ifstream file(model_path);

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<Texture>> textures;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec3> normals;

        std::vector<std::string> triplets; // vector for face vertices ("v/vt/vn")
        glm::vec3 vector3{};                 // vec3 for v, vn
        glm::vec2 vector2{};                 // vec2 for vt

        std::string line;

        // TODO: do something like reserve() or similar with these big ass vectors

        while (std::getline(file, line))
        {
            std::string linetype = line.substr(0, line.find(" "));

            if (linetype == "#") // comment
                continue;

            if (linetype == "o") // object (mesh)
            {
                // think something
            }

            if (linetype == "v") // vertex
            {
                std::istringstream v(line.substr(line.find(" ")));

                v >> vector3.x >> vector3.y >> vector3.z;
                positions.push_back(vector3);

                continue;
            }

            if (linetype == "vt") // texture
            {
                std::istringstream v(line.substr(line.find(" ")));

                v >> vector2.x >> vector2.y;
                texCoords.push_back(vector2);

                continue;
            }

            if (linetype == "vn") // normal
            {
                std::istringstream v(line.substr(line.find(" ")));

                v >> vector3.x >> vector3.y >> vector3.z;
                normals.push_back(vector3);

                continue;
            }

            if (linetype == "f") // face
            {
                /**
                 *  v - "v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3"
                 *  points - {"v1/vt1/vn1", "v2/vt2/vn2", "v3/vt3/vn3"}
                 *  token - temporal string to get point's v, vt, vn
                 *  tokens - temporal vector to store point's v, vt, vn and push them to vertex
                 */

                std::istringstream v(line.substr(line.find(" ")));

                std::string points[3];
                std::string token;
                std::vector<int> tokens;

                v >> points[0];
                v >> points[1];
                v >> points[2];

                for (unsigned int i = 0; i < 3; i++)
                {
                    std::stringstream point(points[i]);

                    // Check for already existing triplets (v/vt/vn) and push index of the first mention of them to indices
                    triplets.push_back(point.str());
                    indices.push_back(std::find(triplets.begin(), triplets.end(), point.str()) - triplets.begin());

                    // "v/vt/vn" -> {v, vt, vn}
                    while (getline(point, token, '/'))
                        tokens.push_back(std::stoi(token));

                    vertices.push_back(Vertex(positions[--tokens[0]],
                        texCoords[--tokens[1]],
                        normals[--tokens[2]]));

                    tokens.clear();
                }

                continue;
            }

            if (linetype == "mtllib") // corresponding material file
            {
                std::istringstream file_name(line.substr(line.find(" ") + 1));
                textures = pullTexturesFromMtl(model_path.substr(0, model_path.find_last_of('/') + 1) + file_name.str());

                continue;
            }
        }

        m_LoadedModels.emplace(name, Model(vertices, indices, textures, true));
    }
}

void ResourceManager::loadTexture(const std::string texture_path)
{
    std::string ext = texture_path.substr(texture_path.find_last_of(".") + 1);
    std::string name = texture_path.substr(texture_path.find_last_of("/") + 1, texture_path.substr(texture_path.find_last_of("/") + 1).size() - ext.size() - 1);
    std::cout << name << std::endl;
    // If the same texture was loaded - return it (copying isnt necessary)
    if (m_LoadedTextures.find(name) != m_LoadedTextures.end())
        std::cout << "TEXTURE WITH NAME:: " << name << " ALREADY LOADED" << std::endl;

    else
    {

        int width, height, numComponents;
        unsigned char* image = stbi_load(texture_path.c_str(), &width, &height, &numComponents, 0);

        if (!image)
            std::cerr << "Image \"" << name << "\" failed to load at: " << texture_path << std::endl;

        Texture texture(image, width, height, numComponents);

        stbi_image_free(image);

        m_LoadedTextures.emplace(name, texture);
    }
}

void ResourceManager::loadScreenShaders(const std::string path_to_folder)
{
    std::string vert_path;
    std::vector<std::string> frag_paths;

    for (const auto& entry : std::filesystem::directory_iterator(path_to_folder))
    {
        std::string file_path = entry.path().string();
        std::replace(file_path.begin(), file_path.end(), '\\', '/');

        std::string file_name_with_extension = file_path.substr(file_path.find_last_of("/") + 1, file_path.size());
        std::string extension = file_name_with_extension.substr(file_name_with_extension.find(".") + 1, file_name_with_extension.size());

        if (extension == VERTEX_FILE_EXTENSION)
            vert_path = path_to_folder + "/" + file_name_with_extension;
        else if (extension == FRAGMENT_FILE_EXTENSION)
            frag_paths.push_back(path_to_folder + "/" + file_name_with_extension);
    }

    for (const auto& frag_path : frag_paths)
    {
        std::string file_name = frag_path.substr(frag_path.find_last_of("/") + 1, frag_path.size() - frag_path.find_last_of("/") - (frag_path.size() - frag_path.find(".") + 1));
        m_LoadedScreenShaders.emplace(file_name, makeShaderProgram(vert_path, frag_path));
    }
}

std::vector<std::shared_ptr<Texture>> ResourceManager::pullTexturesFromMtl(const std::string& full_path)
{
    std::ifstream file(full_path);
    std::string line;

    std::vector<std::shared_ptr<Texture>> textures;

    while (std::getline(file, line))
    {
        std::string linetype = line.substr(0, line.find(" "));

        if (linetype == "map_Kd")
        {
            std::string str = line.substr(line.find(" ") + 1);
            std::string file_path = full_path.substr(0, full_path.find_last_of('/') + 1) + str;
            std::string ext = file_path.substr(file_path.find_last_of(".") + 1);
            std::string name = file_path.substr(file_path.find_last_of("/") + 1, file_path.substr(file_path.find_last_of("/") + 1).size() - ext.size() - 1);

            loadTexture(file_path);
            std::shared_ptr<Texture> texture = makeTexture(name);
            texture->m_type = "texture_diffuse";
            textures.push_back(texture);
        }
    }

    return textures;
}

std::shared_ptr<Shader> ResourceManager::makeShaderProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
    if (m_LoadedShaders.find(vertex_shader_path + "||" + fragment_shader_path) != m_LoadedShaders.end())
        return m_LoadedShaders.at(vertex_shader_path + "||" + fragment_shader_path);

    // Getting shader files (.vert and .frag)
    std::string vertex_shader_src = getFileString(vertex_shader_path);
    std::string fragment_shader_src = getFileString(fragment_shader_path);

    auto shader = std::make_shared<Shader>(vertex_shader_src, fragment_shader_src);

    m_LoadedShaders.emplace(vertex_shader_path + "||" + fragment_shader_path, shader);
    return shader;
}

std::shared_ptr<Texture> ResourceManager::makeTexture(const std::string& name)
{
    if (m_LoadedTextures.find(name) != m_LoadedTextures.end())
        return std::make_shared<Texture>(m_LoadedTextures.at(name));

    std::cout << "TEXTURE WITH NAME " << name << " NOT FOUND" << std::endl;
    ASSERT(false);
}

Model ResourceManager::makeModel(const std::string& name)
{
    if (m_LoadedModels.find(name) != m_LoadedModels.end())
        return m_LoadedModels.at(name);

    std::cout << "MODEL WITH NAME " << name << " NOT FOUND" << std::endl;
    ASSERT(false);
}

CylindricalBillboard ResourceManager::makeCylBillboard(glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts)
{
    auto texture = makeTexture(texture_name);
    texture->m_type = "texture_diffuse";

    return CylindricalBillboard(pos, size, texture, verts);
}

SphericalBillboard ResourceManager::makeSphBillboard(glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts)
{
    auto texture = makeTexture(texture_name);
    texture->m_type = "texture_diffuse";

    return SphericalBillboard(pos, size, texture, verts);
}

void ResourceManager::displayLoadedObjects()
{
    for (auto item : m_LoadedObjects)
        std::cout << item.first << std::endl;
}

void ResourceManager::preLoadResources()
{
    std::string model_folder_path = relResPath + MODELS_FOLDER;
    std::string shaders_folder_path = relResPath + SHADERS_FOLDER;
    std::string textures_folder_path = relResPath + TEXTURES_FOLDER;
    std::string screen_shaders_folder_path = relResPath + SCREEN_SHADERS_FOLDER;

    // Loading models
    for (const auto& entry : std::filesystem::directory_iterator(model_folder_path))
    {
        std::string folder_path = entry.path().string();
        std::replace(folder_path.begin(), folder_path.end(), '\\', '/');

        for (const auto& file : std::filesystem::directory_iterator(folder_path))
        {
            std::string file_path = file.path().string();
            std::replace(file_path.begin(), file_path.end(), '\\', '/');

            std::string extension = file_path.substr(file_path.find(".") + 1, file_path.size());
            if (extension == MODELS_MAIN_FILE_EXTENSION)
                loadModel(file_path);
        }
    }

    // Loading textures
    for (const auto& entry : std::filesystem::directory_iterator(textures_folder_path))
    {
        std::string file_path = entry.path().string();
        std::replace(file_path.begin(), file_path.end(), '\\', '/');

        std::string extension = file_path.substr(file_path.find(".") + 1, file_path.size());
        if (std::find(TEXTURES_FILE_EXTENSIONS.begin(), TEXTURES_FILE_EXTENSIONS.end(), extension) != TEXTURES_FILE_EXTENSIONS.end())
            loadTexture(file_path);
    }

    // Loading screen shaders
    loadScreenShaders(screen_shaders_folder_path);
}
