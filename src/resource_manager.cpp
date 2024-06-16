#include "resource_manager.h"

std::string ResourceManager::getFileString(const std::string& file_rel_path)
{
    std::string file_path = relResPath + file_rel_path.c_str();
    std::ifstream file(file_path, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "File failed to open at: " << file_path << std::endl;
        return {};
    }
    
    // Returns file contents as a string
    std::stringstream readbuffer;
    readbuffer << file.rdbuf();
    return readbuffer.str();
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
            std::shared_ptr<Texture> texture = makeTexture(str, "texture_diffuse", (full_path.substr(0, full_path.find_last_of('/') + 1) + str).erase(0, relResPath.length()));
            textures.push_back(texture);
        }
    }

    return textures;
}

std::map<const std::string, std::shared_ptr<Shader>> ResourceManager::makePostProcessingShaders(const std::string& path_to_folder)
{
    std::map<const std::string, std::shared_ptr<Shader>> shader_map;
    std::string vert_path;
    std::vector<std::string> frag_paths; 

    for (const auto& entry : std::filesystem::directory_iterator(relResPath + path_to_folder))
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
        shader_map.emplace(file_name, makeShaderProgram("screen_shader " + file_name, vert_path, frag_path));
    }

    return shader_map;
}

std::shared_ptr<Shader> ResourceManager::makeShaderProgram(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path)
{
    // Getting shader files (.vert and .frag)
    std::string vertex_shader_src = getFileString(vertex_shader_rel_path);
    std::string fragment_shader_src = getFileString(fragment_shader_rel_path);

    std::shared_ptr<Shader>& shader_program = std::make_shared<Shader>(vertex_shader_src, fragment_shader_src);

    return shader_program;
}

std::shared_ptr<Texture> ResourceManager::makeTexture(std::string name, std::string type, const std::string& image_rel_path)
{
    std::string full_path = relResPath + image_rel_path;

    int width, height, numComponents;
    unsigned char* image = stbi_load(full_path.c_str(), &width, &height, &numComponents, 0);
    
    if (!image)
        std::cerr << "Image \"" << name << "\" failed to load at: " << full_path << std::endl;

    std::shared_ptr<Texture>& texture = std::make_shared<Texture>(type, image, width, height, numComponents);

    stbi_image_free(image);

    return texture;
}

std::shared_ptr<Model> ResourceManager::makeModel(std::string name, const std::string& model_rel_path)
{
    std::string full_path = relResPath + model_rel_path;

    std::ifstream file(full_path);

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
            textures = pullTexturesFromMtl(full_path.substr(0, full_path.find_last_of('/') + 1) + file_name.str());

            continue;
        }
    }

    std::shared_ptr<Model>& model = std::make_shared<Model>(vertices, indices, textures);

    return model;
}

std::shared_ptr<CylindricalBillboard> ResourceManager::makeCylBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    std::shared_ptr<CylindricalBillboard>& cyl_billboard = std::make_shared<CylindricalBillboard>(pos, size, makeTexture(name + "_texture", "texture_diffuse", texture_path));

    return cyl_billboard;
}

std::shared_ptr<SphericalBillboard> ResourceManager::makeSphBillboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    std::shared_ptr<SphericalBillboard>& sph_billboard = std::make_shared<SphericalBillboard>(pos, size, makeTexture(name + "_texture", "texture_diffuse", texture_path));

    return sph_billboard;
}