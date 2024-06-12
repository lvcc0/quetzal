#include "resource_manager.h"

ResourceManager::ResourceManager(const std::string& main_path)
{
	relResPath = main_path.substr(0, main_path.find_last_of("/\\")); // saving res path
};

std::string ResourceManager::get_file_string(const std::string& file_rel_path)
{
	std::string file_path = relResPath + "/" + file_rel_path.c_str();
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

std::vector<std::shared_ptr<Texture>> ResourceManager::pull_textures_from_mtl(const std::string& full_path)
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
            std::shared_ptr<Texture> texture = make_texture(str, "texture_diffuse", (full_path.substr(0, full_path.find_last_of('/') + 1) + str).erase(0, relResPath.length()));
            textures.push_back(texture);
        }
    }

	return textures;
}

std::map<const std::string, std::shared_ptr<Shader>> ResourceManager::make_post_processing_shaders(std::string path_to_folder)
{
    ShaderMap pp_shaderMap;
    std::string vertex_path;
    std::vector<std::string> fragments_paths; 

    std::string path = relResPath + "/" + path_to_folder;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::string filePath = entry.path().string();
        std::replace(filePath.begin(), filePath.end(), '\\', '/');
        std::string file_name_with_extension = filePath.substr(filePath.find_last_of("/") + 1, filePath.size());
        std::string extension = file_name_with_extension.substr(file_name_with_extension.find(".") + 1, file_name_with_extension.size());
        if (extension == VERTEX_FILE_EXTENSION) {
            vertex_path = path_to_folder + "/" + file_name_with_extension;
        }
        else {
            fragments_paths.push_back(path_to_folder + "/" + file_name_with_extension);
        }
    }
    for (GLint i {0}; i < fragments_paths.size(); i++) {
        std::string full_path = fragments_paths[i];
        std::string file_name = full_path.substr(full_path.find_last_of("/") + 1, full_path.size() - full_path.find_last_of("/") -(full_path.size()- full_path.find(".") + 1));
        pp_shaderMap.emplace(file_name, make_shader_program("screen_shader " + file_name, vertex_path, full_path)); // Here shader putting into two maps
    }
    return pp_shaderMap;
}

void ResourceManager::getObjectsInMaps(ObjectType objectType)
{
    switch (objectType)
    {
    case ObjectType::SHADER: {
        std::map<std::string, std::shared_ptr<Shader>>::iterator iter = shaderMap.begin();
        while (iter != shaderMap.end()) {
            std::cout << "shader \"" << iter->first << "\" with id " << iter->second->ID << std::endl;
            iter++;
        }
        break;
    }
    case ObjectType::TEXTURE: {
        std::map<std::string, std::shared_ptr<Texture>>::iterator iter = textureMap.begin();
        while (iter != textureMap.end()) {
            std::cout << "texture \"" << iter->first << "\" with id " << iter->second->ID << std::endl;
            iter++;
        }
        break;
    }
    case ObjectType::MODEL: {
        std::map<std::string, std::shared_ptr<Model>>::iterator iter = modelMap.begin();
        while (iter != modelMap.end()) {
            std::cout << "model \"" << iter->first << "\"" << std::endl;
            iter++;
        }
        break;
    }
    case ObjectType::CYL_BILLBOARD: {
        std::map<std::string, std::shared_ptr<CylindricalBillboard>>::iterator iter = cylBillboardMap.begin();
        while (iter != cylBillboardMap.end()) {
            std::cout << "cylindrical billboard \"" << iter->first << "\"" << std::endl;
            iter++;
        }
        break;
    }
    case ObjectType::SPH_BILLBOARD: {
        std::map<std::string, std::shared_ptr<SphericalBillboard>>::iterator iter = sphBillboardMap.begin();
        while (iter != sphBillboardMap.end()) {
            std::cout << "spherical billboard \"" << iter->first << "\"" << std::endl;
            iter++;
        }
        break;
    }
    default:
        std::cout << "the default output of the switch statement in the getObjectsInMaps() function" << std::endl;
    }
}

std::shared_ptr<Shader> ResourceManager::make_shader_program(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path)
{
    // Getting shader files (.vert and .frag)
    std::string vertex_shader_src = get_file_string(vertex_shader_rel_path);
    std::string fragment_shader_src = get_file_string(fragment_shader_rel_path);

    std::shared_ptr<Shader>& shader_program = std::make_shared<Shader>(vertex_shader_src, fragment_shader_src);
    shaderMap.emplace(name, shader_program);

    return shader_program;
}

std::shared_ptr<Texture> ResourceManager::make_texture(std::string name, std::string type, const std::string& image_rel_path)
{
    std::string full_path = relResPath + "/" + image_rel_path;

	int width, height, numComponents;
	unsigned char* image = stbi_load(full_path.c_str(), &width, &height, &numComponents, 0);
	
	if (!image)
        std::cerr << "Image \"" << name << "\" failed to load at: " << full_path << std::endl;

	std::shared_ptr<Texture>& texture = std::make_shared<Texture>(type, image, width, height, numComponents);

	stbi_image_free(image);

	textureMap.emplace(name, texture);
	return texture;
}

std::shared_ptr<Model> ResourceManager::make_model(std::string name, const std::string& model_rel_path)
{
	std::string full_path = relResPath + "/" + model_rel_path;

	std::ifstream file(full_path);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    std::vector<std::string> triplets; // vector for face vertices ("v/vt/vn")
    glm::vec3 vector3;                 // vec3 for v, vn
    glm::vec2 vector2;                 // vec2 for vt

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
            textures = pull_textures_from_mtl(full_path.substr(0, full_path.find_last_of('/') + 1) + file_name.str());

            continue;
        }
    }

	std::shared_ptr<Model>& model = std::make_shared<Model>(vertices, indices, textures);
    modelMap.emplace(name, model);

	return model;
}

std::shared_ptr<CylindricalBillboard> ResourceManager::make_cyl_billboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    std::shared_ptr<CylindricalBillboard>& cyl_billboard = std::make_shared<CylindricalBillboard>(pos, size, make_texture(name + "_texture", "texture_diffuse", texture_path));
    cylBillboardMap.emplace(name, cyl_billboard);

    return cyl_billboard;
}

std::shared_ptr<SphericalBillboard> ResourceManager::make_sph_billboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path)
{
    std::shared_ptr<SphericalBillboard>& sph_billboard = std::make_shared<SphericalBillboard>(pos, size, make_texture(name + "_texture", "texture_diffuse", texture_path));
    sphBillboardMap.emplace(name, sph_billboard);

    return sph_billboard;
}