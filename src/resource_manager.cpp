#include "resource_manager.h"

ResourceManager::ResourceManager(const std::string& main_path)
{
	unsigned end_of_string = main_path.find_last_of("/\\");
	relResPath = main_path.substr(0, end_of_string); // saving res path
};

std::string ResourceManager::get_file_string(const std::string& file_rel_path) {
	std::string file_path = relResPath + "/" + file_rel_path.c_str();  // full file path
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

std::vector<std::shared_ptr<Texture>> ResourceManager::pull_textures_from_mtl(const std::string& fullfilepath)
{
    std::string full_m_directory = fullfilepath.substr(0, fullfilepath.find_last_of("/"));
	std::string rel_m_directory = full_m_directory.substr(full_m_directory.find("res/") + 4, full_m_directory.back());


    std::ifstream file(fullfilepath);
    std::string line;

    std::vector<std::shared_ptr<Texture>> m_textures;

    while (std::getline(file, line))
    {
        std::string linetype = line.substr(0, line.find(" "));

        if (linetype == "map_Kd")
        {
            std::string str = line.substr(line.find(" ") + 1);
            std::string jpgfilepath = rel_m_directory + "/" + str.c_str();
            std::shared_ptr<Texture> texture = make_texture_from_jpg(str, "texture_diffusal", jpgfilepath);
            m_textures.push_back(texture);
        }
    }
	return m_textures;
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

std::shared_ptr<Texture> ResourceManager::make_texture_from_jpg(std::string name, std::string type, const std::string& texture_rel_path)
{
	std::string full_path = relResPath + "/" + texture_rel_path;
	/*variables would be taken to a texture object*/
	int width, height, numComponents;
	unsigned char* image = stbi_load(full_path.c_str(), &width, &height, &numComponents, 0);
	/*checking if image loaded*/
	if (!image) {
		std::cerr << "image didnt loaded, name:: " << name << " in path: "<< full_path << std::endl;
		return nullptr;
	}
	/*---------------------------*/

	std::shared_ptr<Texture>& texture = std::make_shared<Texture>(type, image, width, height, numComponents);
	/*deleting image*/
	stbi_image_free(image); 
	textureMap.emplace(name, texture);
	return texture;
}

std::shared_ptr<Model> ResourceManager::make_model(std::string name, const std::string& model_rel_path)
{
	/*working on obj file*/
    std::string modelfilepath = relResPath + "/" + model_rel_path;
    std::string m_directory = modelfilepath.substr(0, modelfilepath.find_last_of('/'));

	/*TODO: WTF*/
    std::ifstream objfile(modelfilepath);
	std::ifstream auxobjfile(modelfilepath);

    std::string line;
    std::vector<std::shared_ptr<Texture>> m_textures;
	std::stringstream buffer;
	buffer << auxobjfile.rdbuf();

	/*searching for mtl file*/
    while (std::getline(objfile, line))
    {
        std::string linetype = line.substr(0, line.find(" "));
        if (linetype == "mtllib") 
        {
            std::istringstream v(line.substr(line.find(" ") + 1));
            std::string matfilepath = m_directory + "/" + v.str();
            m_textures = pull_textures_from_mtl(matfilepath);
        }
    }
	/*-------------------------------------------*/
	std::shared_ptr<Model>& retmodel = std::make_shared<Model>(buffer.str(), m_textures);
	modelMap.emplace(name, retmodel);

	return retmodel;
}
